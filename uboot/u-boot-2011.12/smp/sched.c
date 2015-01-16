#include <stdarg.h>
#include <exports.h>
#include <config.h>
#include <errno.h>
#include <x_typedef.h>
#include <thread_info.h>
#include <spinlock.h>
#include <thread.h>
#include <smp_platform.h>
#include <smp.h>

#define get_cpu_id()	get_current_cpu()

spin_lock_t init_done[NR_CPUS] ;
spin_lock_t init_idle[NR_CPUS] ;

spin_lock_t  smp_cpu_released[NR_CPUS];

static unsigned int oneshot_timer_id[NR_CPUS];
cond_t g_sub_cond[NR_CPUS];
static spin_lock_t g_thread_lock = INIT_SPIN_LOCK;

spin_lock_t g_print_lock = INIT_SPIN_LOCK;
spin_lock_t g_sched_lock = INIT_SPIN_LOCK;
scheduler_t g_scheduler;
thread_t *current_thread[NR_CPUS];  /* Current thread */
int active_count = 0;       /* Active thread count */
int megic_number_cleaned = 0;

extern void *main_init(void *arg);
#define readl(addr) (*(volatile unsigned int*)(addr))
#define writel(b,addr) ((*(volatile unsigned int *) (addr)) = (b))

extern u32 random(u32 limit);
extern void secondary_start_uboot_cleanup(void);

const char* thread_state_str[] =
{
	"idle",
	"running",
	"ready",
	"lock-wait",
	"cond-wait",
	"sleeping",
	"zombie",
	"suspended",
	"done"
};
extern int *thread_test1(void *arg);
extern int *thread_test0(void *arg);
volatile int i;
int __release_smp_cpu(void)
{
    if(BOOT_CPU == get_cpu_id())
        return -1;

    for(;;)
    {
        if (megic_number_cleaned)
        {
            smp_cpu_released[get_cpu_id()].lock= 1;
            asm volatile("mcr p15, 0, %0, c14, c2, 1" : : "r" (0));
            dsb();
            wfi();
            if(MAGIC_NUMBER == readl(CONFIG_KERNEL_START_ADDRESS+SMP_DUMMY_MAGIC));
            {
                return jump_to_secondary();
            }
        }
    }
}
static void *sub_init(void *arg)
{
#if 1		
		char name[16];
		int pri;
		
		
		thread_t *t;
		static int cpu_id = 0;
		//for( i=0; i < 3;i++ )
			{
			//pri = random(THREAD_MAX_PRIORITY-THREAD_MIN_PRIORITY);
			pri = THREAD_DEFAULT_PRIORITY;
			//cpu_id = (cpu_id+1) % NR_CPUS;
		#if 0
		
		if(init_idle[1].lock == 1)
			{
				
				sprintf(name,"test-%d",1);
				t = thread_create_ex(name,thread_test0, NULL, 0,1,pri,0);
			}
		if(init_idle[2].lock == 1)
			{
				sprintf(name,"test-%d",2);
				t = thread_create_ex(name,thread_test0, NULL, 0,2,pri,0);
			}
		if(init_idle[3].lock == 1)
			{
				
				sprintf(name,"test-%d",3);
				t = thread_create_ex(name,thread_test0, NULL, 0,3,pri,0);
			}
		#endif
		

		
		}
	#else
		
		//printf("\n#######main_init#########\n");
	#endif

	//printf("Starting in sub thread main context\n");
//	thread_cond_timedwait(&g_sub_cond[get_current_cpu()],NO_TIMEOUT);
//	for(;;)
//		__release_smp_cpu();
}

static void _thread_main(int (*func)(void *),void *arg, thread_t *thread)
{
	if( !(thread->flags & FLAG_PRIMORDIAL) ) {
		//dbg_print("XXX not main thread=%s\n",thread->name);
			spin_unlock_restore(&g_sched_lock,(CPSR_MODE_SVC|CPSR_F_DISABLE));
	}
	dbg_print("XXX thread func=%s,cpsr=%d\n",thread->name,get_cpsr());
	thread->retval = func(arg);
	thread_exit(thread->retval);
}

static thread_t **heap_insert(thread_t *thread)
{
	int target = thread->heap_index;
	int s = target;
	int cpu_id = thread->cpu_id;
	thread_t **p = &get_sleep_queue(cpu_id);
	int bits = 0;
	int bit;
	int index = 1;

	while (s) {
		s >>= 1;
		bits++;
	}
	for (bit = bits - 2; bit >= 0; bit--) {
		if (thread->due < (*p)->due) {
			thread_t *t = *p;
			thread->left = t->left;
			thread->right = t->right;
			*p = thread;
			thread->heap_index = index;
			thread = t;
		}
		index <<= 1;
		if (target & (1 << bit)) {
			p = &((*p)->right);
			index |= 1;
		} else {
			p = &((*p)->left);
		}
	}
	thread->heap_index = index;
	*p = thread;
	thread->left = thread->right = NULL;
	return p;
}

/*
 * Delete "thread" from the timeout heap.
 */
static void heap_delete(thread_t *thread)
{
	thread_t *t, **p;
	int cpu_id = thread->cpu_id;
	int bits = 0;
	int s, bit;

	/* First find and unlink the last heap element */
	p = &get_sleep_queue(cpu_id);
	s = SLEEPQ_SIZE(cpu_id);
	while (s) {
		s >>= 1;
		bits++;
	}
	for (bit = bits - 2; bit >= 0; bit--) {
		if (SLEEPQ_SIZE(cpu_id) & (1 << bit)) {
			p = &((*p)->right);
		} else {
			p = &((*p)->left);
		}
	}
	t = *p;
	*p = NULL;
	--SLEEPQ_SIZE(cpu_id);
	if (t != thread) {
		/*
		 * Insert the unlinked last element in place of the element we are deleting
		 */
		t->heap_index = thread->heap_index;
		p = heap_insert(t);
		t = *p;
		t->left = thread->left;
		t->right = thread->right;

		/*
		 * Reestablish the heap invariant.
		 */
		for (;;) {
			thread_t *y; /* The younger child */
			int index_tmp;
			if (t->left == NULL)
				break;
			else if (t->right == NULL)
				y = t->left;
			else if (t->left->due < t->right->due)
				y = t->left;
			else
				y = t->right;
			if (t->due > y->due) {
				thread_t *tl = y->left;
				thread_t *tr = y->right;
				*p = y;
				if (y == t->left) {
					y->left = t;
					y->right = t->right;
					p = &y->left;
				} else {
					y->left = t->left;
					y->right = t;
					p = &y->right;
				}
				t->left = tl;
				t->right = tr;
				index_tmp = t->heap_index;
				t->heap_index = y->heap_index;
				y->heap_index = index_tmp;
			} else {
				break;
			}
		}
	}
	thread->left = thread->right = NULL;
}

static void timer_wakeup(void)
{
	thread_t *thread;
	int cpu_id = get_cpu_id();
	u32 now = (u32)arch_counter_get_ms();

	while (get_sleep_queue(cpu_id) != NULL) {
		
		thread = get_sleep_queue(cpu_id);
		dbg_print("AAA wakeup thread =%s,due=%lu,state=%d,now = %lu\n",thread->name,thread->due,thread->state,now);

		if( !(thread->flags & FLAG_ON_SLEEPQ) ) {
			tlog("Error: Thread(%s) status  must be SLEEP\n",thread->name);
			continue;
		}
		if (thread->due > now) {
			
			dbg_print("BBBBBB wakeup thread =%d,thread name =%s,due=%lu,state=%d\n",thread,thread->name,thread->due,thread->state);
			arch_reset_timer(oneshot_timer_id[cpu_id],thread->due-now,2);
			break;
		}
		
		spin_lock(&g_sched_lock);
		del_sleep_q(thread);
		spin_unlock(&g_sched_lock);
			
		if (thread->state == STATE_COND_WAIT)
			thread->flags |= FLAG_TIMEDOUT;

		if( thread->flags & FLAG_IDLE_THREAD ) {
			tlog("Error: IDLE thread should NOT be in SLEEPQ\n");
			continue;
		}

		
		spin_lock(&g_sched_lock);
		dbg_print("XXX wakeup thread =%s,due=%lu,state=%d\n",thread->name,thread->due,thread->state);
		add_run_queue(thread);
		spin_unlock(&g_sched_lock);
	}
}

static void send_ipi(thread_t *new)
{
	int cpu_id = new->cpu_id;
	thread_t *me = get_current_thread(cpu_id);
	if( init_done[cpu_id].lock> 0 ) {
		if( new->priority > me->priority ) {
			dbg_print("Send IPI thread=%s,pri=%d,new=%s,pri=%d,target cpu=%d\n",me->name,me->priority,new->name,new->priority,cpu_id);
			ipi_send_target(cpu_id,IPI_NEED_RESCHEDULE);
		}
	}
}

static thread_t * allocate_thread(const char *name,void *(*start)(void *arg), void *arg, int stk_size,int cpu_id,int priority)
{
	thread_t *thread = NULL;
	unsigned char *stack;
	int stack_size;
	u32 flags;
	unsigned char *data;

	if( stk_size == 0 ) {
		stack_size = DEFAULT_STACK_SIZE;
	} else if( stk_size < MIN_STACK_SIZE ) {
		stack_size = MIN_STACK_SIZE;
	} else {
		stack_size = stk_size;
	}
	data = (unsigned char *)malloc(sizeof(thread_t) + stack_size + (STACK_BUFFER_SIZE*9));
	if(data == NULL){
		dbg_print("Failed to allocate thread\n");
		return NULL;
        }
	thread = (thread_t*)data;
	stack = (unsigned char *)(data + sizeof(thread_t) + (STACK_BUFFER_SIZE*7) + stack_size);

	memset(thread, 0, sizeof(thread_t));

	thread->reg.sp		= (unsigned int)stack;
	thread->reg.pc		= (unsigned int)_thread_main;
	thread->reg.spsr	= (unsigned int)(CPSR_MODE_SVC | CPSR_F_DISABLE | CPSR_I_DISABLE); //0xd3

	thread->reg.r[0] = (unsigned int)start;
	thread->reg.r[1] = (unsigned int)arg;
	thread->reg.r[2] = (unsigned int)thread;
	thread->start = start;
	thread->cpu_id = cpu_id;
	thread->priority = priority;
	thread->stack_top = stack;

	sprintf(thread->name,"%s",name);

	spin_lock(&g_thread_lock);
	ADD_THREADQ(thread);
	active_count++;
	spin_unlock(&g_thread_lock);

	spin_lock_save(&g_sched_lock,flags);
	add_run_queue(thread);
	spin_unlock_restore(&g_sched_lock,flags);

	return thread;
}

int jump_to_secondary(void)
{
    unsigned int *mp_start_addr;
    mp_start_addr = (CONFIG_KERNEL_START_ADDRESS+SMP_DUMMY_PA_START);
    void (*mp_start_func)(void) = (void (*)(void)) (*mp_start_addr);

    if(mp_start_func)
    {
        mp_start_func();
    }
    return -1;
}


//extern void display_all_threads(void);

static void *_idle_thread_start(void *arg)
{
	//thread_t *me = get_current_thread(0);
	while( 1 ) {
        // prepare for Kernel SMP pen_releases
                if(BOOT_CPU != get_cpu_id() && megic_number_cleaned==1 && smp_cpu_released[get_cpu_id()].lock== 0)
                {
                	
					tlog("_idle_thread_start smp_cpu_released =%x,cpu id =%d \n", smp_cpu_released[get_cpu_id()].lock,get_cpu_id());
					secondary_start_uboot_cleanup();
        }

	}
	return NULL;
}

static int assign_cpu(thread_t *thread)
{
	int i;
	int cpu_id = -1;

	if( thread->flags & FLAG_FIX_CPU ) {
		return thread->cpu_id;
	}
	for( i=0; i<NR_CPUS; i++ ) {
		if( current_thread[i]->flags & FLAG_IDLE_THREAD ) {
			cpu_id = i;
			break;
		}
	}

#ifdef SUPPORT_PREEMPTION
	if( cpu_id < 0 ) {
		for( i=0; i<NR_CPUS; i++ ) {
			if( thread->priority > current_thread[i] ) {
				cpu_id = i;
				break;
			}
		}
	}
#endif
	if( cpu_id < 0 ) {
		int rnd = radom(NR_CPUS-1);
		cpu_id = rnd;
	}
	return cpu_id;
}


static thread_t * get_proper_thread(thread_t *current)
{
	thread_t *thread = NULL;
	int cpu_id = get_cpu_id();

#ifdef SUPPORT_PREEMPTION
	if (get_run_queue(cpu_id).next != &get_run_queue(cpu_id)) {
		thread = get_thread_ptr(get_run_queue(cpu_id).next);
		dbg_print("XXX scheduler exec running next=%s,pri=%d,current name=%s,pri=%dstate=%x,flag=%x\n", thread->name,thread->priority,current->name,current->priority,current->state,current->flags);
#if 0
		if( !(current->flags & FLAG_PRIMORDIAL) &&
				(current->state == STATE_RUNNING) ) {
#else
		if( (current->state == STATE_RUNNING) ) {
#endif

#if 0
			//dbg_print("XXX running thread=%s\n",current->name);
			if( current->priority < thread->priority ) {
				//dbg_print("Next thread has preemption\n");
				DEL_RUNQ(thread);
			} else
				thread = NULL;
#else
			thread = get_thread_ptr(get_run_queue(cpu_id).next);
			DEL_RUNQ(thread);
#endif
		} else {
			thread = get_thread_ptr(get_run_queue(cpu_id).next);
			//dbg_print("XXX next thread to be executed=%s\n",thread->name);
			DEL_RUNQ(thread);
		}
	} else 
	{
		thread = get_idle_thread(cpu_id);
	}
#else
	if (get_run_queue(cpu_id).next != &get_run_queue(cpu_id)) {
		/* Pull thread off of the run queue */
		thread = get_thread_ptr(get_run_queue().next);
		//dbg_print("XXX scheduler exec running thread =%p\n",thread);
		DEL_RUNQ(thread);
	} else {
		/* If there are no threads to run, switch to the idle thread */
		thread = get_idle_thread(cpu_id);
		//dbg_print("XXX scheduler exec idle=%p\n",thread);
	}
#endif

	return thread;
}

void add_run_queue(thread_t *thread)
{
	int cpu_id = thread->cpu_id;
#ifdef SUPPORT_PREEMPTION
	thread_t *n;
	list_t *q = get_run_queue(cpu_id).next;

	thread->state = STATE_RUNNABLE;
	for( ; q != &get_run_queue(cpu_id); q = q->next ) {

		n = get_thread_ptr(q);
		dbg_print("Scanning priority thread cpu=%d,name = %s,prio=%d,new=%s,prio=%d\n",cpu_id,n->name,n->priority,thread->name,thread->priority);
		if( thread->priority > n->priority ) {
			//dbg_print("Higher priority thread name = %s,prio=%d,new=%s,prio=%d\n",n->name,n->priority,thread->name,thread->priority);
			INSERT_BEFORE(&thread->links,q);
			send_ipi(thread);
			return;
		}
	}

	if( q == &get_run_queue(cpu_id) ) {
		dbg_print("Added new thread=%s,cpu=%d\n",thread->name,cpu_id);
		ADD_RUNQ(thread,cpu_id);
		send_ipi(thread);
	}
#else
	thread->state = STATE_RUNNABLE;
	ADD_RUNQ(thread,cpu_id);
	ipi_send_target(cpu_id,IPI_NEED_RESCHEDULE);
#endif
}

void reschedule_irq()
{
	int cpu_id = get_cpu_id();
	g_scheduler.need_schedule[cpu_id] = 0;
	schedule();
}
//thread_t *threadtest;

int schedule()
{
	thread_t *thread;
	unsigned int flags = 0;
	spin_lock_save(&g_sched_lock,flags);
	int cpu_id = get_cpu_id();
	thread_t *me = get_current_thread(cpu_id);

	thread = get_proper_thread(me);
	#if 0
	//printf("thread=%x, me=%x threadtest=%x\n",thread,me,threadtest);

	if((UINT32)me ==0xB01Fe0 && (UINT32)thread == 0xB121B0 )
		{
			threadtest = me;
		}
	if((UINT32)me ==0xB121B0 && (UINT32)threadtest == 0xB01Fe0  && (UINT32)thread == 0xaf92f8 )
		{
			printf("system will hal \n");
		}
	#endif

/*
	if( me->prev ) {
		free(me->prev);
		me->prev = NULL;
	}
*/
	if( thread == NULL ) {
		dbg_print("Next is NULL\n");
	} else if( me == thread ) {
		dbg_print("Current is same with next=%s\n",thread->name);
		thread->state = STATE_RUNNING;
	} else {
		if( me->state == STATE_RUNNING ) {
			dbg_print("This (%s) is still running ,added it into runQ\n",me->name);
			add_run_queue(me);
		}
		if( thread->state != STATE_IDLE )
			if( thread->state != STATE_RUNNABLE ) {
				dbg_print("Error: Thread(%s) status must be RUNNABLE\n");
			}
		/* Resume the thread */
		if( !(thread->flags & FLAG_IDLE_THREAD) )
			thread->state = STATE_RUNNING;
		set_current_thread(thread,cpu_id);

		dbg_print("CPU=%d,CURRENT cpsr=%x ,me=%s,pc=%x,spsr=%x,NEXT=%s,thread addr =%x ,state=%d,pc=%x,spsr=%x\n",cpu_id,get_cpsr(),me->name,me->reg.pc,me->reg.spsr,thread->name,thread,thread->state,thread->reg.pc,thread->reg.spsr);
		if( (me->state == STATE_DONE) &&
				!(me->flags & FLAG_PRIMORDIAL)) {
			dbg_print("XXX to be freed %s\n",me->name);
			//thread->prev = me;
			me = NULL;
		}


		__switch_context(me,thread);
		

		if( (thread->state == STATE_DONE) &&
				!(thread->flags & FLAG_PRIMORDIAL)) {
			free(thread);
		}
	}

	spin_unlock_restore(&g_sched_lock,flags);
	return 0;
}

static void oneshot_timer_callback(UINT32 stTimer, UINT32 u32Data)
{
    //tlog("oneshot oneshot_timer_callback count\n");
	timer_wakeup();
}

static void arch_period_timer_callback(UINT32 stTimer, UINT32 u32Data)
{
	timer_wakeup();
}
void Timecallback(void)
{
	arch_period_timer_callback( 0,0);
}


 void ipi_handler(unsigned int ipi_num)
{
	int cpu_id = get_cpu_id();
	dbg_print("XXX recv cpu=%d,ipi num=%d\n",get_cpu_id(),ipi_num);
	switch(ipi_num) {
		case IPI_NEED_RESCHEDULE:
			g_scheduler.need_schedule[cpu_id] = 1;
			dbg_print("XXX need resched\n");
			break;
		default:
			dbg_print("Unknown IPI number\n");
			
	}
}

thread_t *thread_init_per_cpu(void)
{
	thread_t *thread;
	unsigned char *data;
	int cpu_id = get_cpu_id();
	char tmp[16];

	if( init_done[cpu_id].lock > 0 ) {
		dbg_print("XXX already init\n");
		return NULL;
	}

	INIT_LIST(&get_run_queue(cpu_id));
	//INIT_LIST(&get_zombie_queue(cpu_id));
	/*
	 * Create idle thread
	 */
	sprintf(tmp,"idle[%d]",cpu_id);
	g_scheduler.idle_thread[cpu_id] = thread_create_ex(tmp,_idle_thread_start,NULL,MIN_STACK_SIZE,cpu_id,THREAD_MIN_PRIORITY,0);
	if (!get_idle_thread(cpu_id))
		return -1;
	get_idle_thread(cpu_id)->flags = FLAG_IDLE_THREAD;
	get_idle_thread(cpu_id)->state = STATE_IDLE;
	active_count--;
	DEL_RUNQ(get_idle_thread(cpu_id));
	init_idle[cpu_id].lock = 1;

	sprintf(tmp,"sub_init[%d]",cpu_id);
	thread = thread_create_ex(tmp,sub_init,NULL,0,cpu_id,THREAD_DEFAULT_PRIORITY-1,0);

	thread->state = STATE_RUNNING;
	thread->flags = FLAG_PRIMORDIAL;
	set_current_thread(thread,cpu_id);
	DEL_RUNQ(thread);
	thread_cond_new(&g_sub_cond[cpu_id]);
    Interrupt_request(IPI_NEED_RESCHEDULE, ipi_handler, NULL);
    //Interrupt_request(IPI_TEST_2, ipi_handler, NULL);
    oneshot_timer_id[cpu_id] = arch_create_timer(oneshot_timer_callback,1,2,"oneshot", cpu_id);
	dbg_print("LG thread per cpu init done,current cpu=%d\n",cpu_id);

	init_done[cpu_id].lock= 1;
	return thread;
}

void smp_thread(void)
{
	thread_t *thread = thread_init_per_cpu();
	asm volatile (
				"ldr	sp ,[%0]\n\t"
				"ldr	r4,[%0,#8]\n\t"
				"add	r5,r0,#16\n\t"
				"ldmia	r5,{r0,r2}\n\t"
				"blx	r4\n\t"
				: :"r"(thread)
				:"memory"
				);

	//if( thread )
		//_thread_main(thread->start,NULL,thread);
}

thread_t *thread_init(void)
{
	thread_t *thread;
	unsigned char *data;
	int cpu_id = get_cpu_id();
	char tmp[16];

	if( init_done[cpu_id].lock > 0 ) {
		dbg_print("XXX already init\n");
		return 0;
	}

	memset(&g_scheduler, 0, sizeof(scheduler_t));

	INIT_LIST(&get_run_queue(cpu_id));
	//INIT_LIST(&get_zombie_queue(cpu_id));
	INIT_LIST(&get_thread_queue());

	/*
	 * Create idle thread
	 */
	sprintf(tmp,"idle[%d]",cpu_id);
	g_scheduler.idle_thread[cpu_id] = thread_create_ex("idle",_idle_thread_start, NULL,MIN_STACK_SIZE,0,THREAD_IDLE_PRIORITY,0);
	if (!get_idle_thread(cpu_id))
		return -1;
	get_idle_thread(cpu_id)->flags = FLAG_IDLE_THREAD;
	get_idle_thread(cpu_id)->state = STATE_IDLE;
	active_count--;
	DEL_RUNQ(get_idle_thread(cpu_id));
	thread = thread_create_ex("init",main_init,NULL,0,0,THREAD_DEFAULT_PRIORITY-1,0);

	thread->state = STATE_RUNNING;
	thread->flags = FLAG_PRIMORDIAL;
	set_current_thread(thread,cpu_id);
	DEL_RUNQ(thread);
    Interrupt_request(IPI_NEED_RESCHEDULE, ipi_handler, NULL);
    //Interrupt_request(IPI_TEST_2, ipi_handler, NULL);
   // arch_counter_calibration();
    oneshot_timer_id[0] = arch_create_timer(oneshot_timer_callback,1,2,"oneshot", 0);
    //arch_create_timer(arch_period_timer_callback, 1, 1, "PERIOD TIMER",0);

#if defined(CONFIG_MULTICORES_PLATFORM)

#ifdef ENABLE_SMP
	Core_Wakeup(smp_thread, 1);
	Core_Wakeup(smp_thread, 2);
	Core_Wakeup(smp_thread, 3);
#endif
#endif
	init_done[cpu_id].lock= 1;
	return thread;
}


void thread_start(void)
{
	thread_t *thread = thread_init();
	#if 0
	asm volatile (
				"ldr	sp ,[%0]\n\t"
				"ldr	r4,[%0,#8]\n\t"
				"add	r5,r0,#16\n\t"
				"ldmia	r5,{r0,r2}\n\t"
				"blx	r4\n\t"
				: :"r"(thread)
				:"memory"
				);
	#endif
	if( thread )
	_thread_main(thread->start,NULL,thread);
}


thread_t *thread_create_ex(const char *name,void *(*start)(void *arg), void *arg, int stk_size,int cpu_id,int priority,int join)
{
	thread_t *thread = allocate_thread(name,start,arg,stk_size,cpu_id,priority);
	if( join ) {
		cond_t *term = calloc(1,sizeof(cond_t));
		thread_cond_new(term);
		thread->term = term;
	}
	if( thread ) return thread;
	return NULL;
}

thread_t *thread_create(const char *name,void *(*start)(void *arg), void *arg)
{
	thread_t *thread;
	int cpu_id = get_cpu_id();
	thread = allocate_thread(name,start,arg,DEFAULT_STACK_SIZE,cpu_id,THREAD_DEFAULT_PRIORITY);
	if( thread ) return thread;
	return NULL;
}

void thread_exit(void *retval)
{
	int cpu_id = get_cpu_id();
	thread_t *thread = get_current_thread(cpu_id);

	thread->retval = retval;
	active_count--;
	if( thread->term ) {
		thread->state = STATE_ZOMBIE;
		//ADD_ZOMBIEQ(thread,cpu_id);
		thread_cond_signal(thread->term);
		schedule();

		free(thread->term);
		thread->term = NULL;
	}

	if( !(thread->flags & FLAG_ON_SLEEPQ) ) {
		thread->state = STATE_DONE;
	}

	spin_lock(&g_thread_lock);
	DEL_THREADQ(thread);
	spin_unlock(&g_thread_lock);
	schedule();
}

int thread_join(thread_t *thread, void **retvalp)
{
	int cpu_id = get_cpu_id();
	if (thread->term == NULL) {
		return -EINVAL;
	}
	if (get_current_thread(cpu_id) == thread) {
		return -EDEADLK;
	}

	if( thread->state == STATE_ZOMBIE ) {
		goto EXIT;
	}
	while (thread->state != STATE_ZOMBIE) {
		if (thread_cond_timedwait(thread->term, NO_TIMEOUT) != 0)
			return -1;
	}

	if (retvalp)
		*retvalp = thread->retval;

	/*
	 * Remove target thread from the zombie queue and make it runnable.
	 * When it gets scheduled later, it will do the clean up.
	 */
EXIT:
	//DEL_ZOMBIEQ(thread);
	spin_lock_irq(&g_sched_lock);
	add_run_queue(thread);
	spin_unlock_irq(&g_sched_lock);
	return 0;
}

void add_sleep_q(thread_t *thread, utime_t timeout)
{
	thread_t *root;
	u32 now = (u32)arch_counter_get_ms();
	thread->due = now + timeout;
	dbg_print("XXX sleepQ name =%s,timeout=%d,due=%d,cpu=%d\n",thread->name,timeout,thread->due,thread->cpu_id);
	thread->flags |= FLAG_ON_SLEEPQ;
	thread->heap_index = ++SLEEPQ_SIZE(thread->cpu_id);
	heap_insert(thread);

	root = get_sleep_queue(thread->cpu_id);
	if( root ) {
		int gap = root->due-now;
		
		dbg_print("XXX sleepQ root =%s,due=%d,gap=%d\n",root->name,root->due,gap);
		if( gap <= 0 )
			arch_reset_timer(oneshot_timer_id[thread->cpu_id],1,2);
		else
			arch_reset_timer(oneshot_timer_id[thread->cpu_id],gap,2);
	}
}

void del_sleep_q(thread_t *thread)
{
	thread->flags &= ~FLAG_ON_SLEEPQ;
	heap_delete(thread);
}

int mtk_msleep(unsigned int msecs)
{
	int cpu_id = get_cpu_id();
	thread_t *me = get_current_thread(cpu_id);

	if( msecs == 0 ) return 0;

	if( !init_done[cpu_id].lock || !is_irq_on() ) {
		udelay(msecs*1000);
		return -EINTR;
	}

	me->flags |= FLAG_FIX_CPU;
	irq_disable();
	if (msecs != NO_TIMEOUT) {
		me->state = STATE_SLEEPING;
		add_sleep_q(me, msecs);
	} else
		me->state = STATE_SUSPENDED;
	irq_enable();

	schedule();
	return 0;
}

int need_to_reschedule(void)
{
	//if( g_scheduler.need_schedule[0] > 0 )
		//dbg_print("XXX need to reschedule=%d\n",g_scheduler.need_schedule[0]);
	return g_scheduler.need_schedule[get_cpu_id()];
}

int mtk_sleep(int secs)
{
	return mtk_msleep((secs >= 0)?secs*(utime_t)MILLISECOND:NO_TIMEOUT);
}
