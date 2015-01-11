#include <stdarg.h>
#include <exports.h>
#include <config.h>
#include <x_typedef.h>
#include "thread.h"
#include <errno.h>

#define get_cpu_id()	get_current_cpu()

static unsigned int timer_id;
static mutex_t mlock;
static cond_t cond;
static int nosleep = 0;
static u32 mRes = 0;
//extern void lg_boot_splash(void);
extern u32 random(u32 limit);

extern u64 arch_counter_get_ms(void);
extern u32 arch_create_timer(TimerCb pTimerCb, u32 ElapseTimeMs, u32 Mode, char*pName, u32 CoreID);
extern BOOL arch_reset_timer(u32 u32TimerId, u32 ElapseTimeMs, u32 Mode);
extern BOOL arch_stop_timer(u32 u32TimerId);
extern BOOL arch_start_timer(u32 u32TimerId);
extern u64 arch_counter_get_ticks(void);
int *thread_test1(void *arg);
int *thread_test0(void *arg);
int *thread_test2(void *arg);


static void oneshot_timer_callback(UINT32 stTimer, UINT32 u32Data)
{
    static int count = 0;
    tlog("oneshot %d count\n", count++);
	/*
	if( !(count % 2 ) )
	arch_reset_timer(timer_id,1000,2);
	else
	arch_reset_timer(timer_id,5000,2);

	if( count == 10 )
		arch_stop_timer(timer_id);
	*/
	//arch_start_timer(timer_id);
	//arch_stop_timer(timer_id);
     //arch_create_timer(arch_oneshot_timer_callback, 1000, 2, "ONE SHOT TIMER");
}
extern void create_thread_test(void);


void *main_init(void *arg)
{
	//tlog("Starting in thread context\n");
	//lg_boot_splash();
	//for (;;) 
		{
		//create_thread_test();
			fast_boot();
		second_main();
			//char c = 0;
			//c = getc();
			//printf ("\n C main_loop C \n");
	#if 0		
		int i;
		char name[16];
		int pri;
		
		thread_t *t;
		static int cpu_id = 0;
		//for( i=0; i < 3;i++ )
			{
			//pri = random(THREAD_MAX_PRIORITY-THREAD_MIN_PRIORITY);
			pri = THREAD_DEFAULT_PRIORITY;
			//cpu_id = (cpu_id+1) % NR_CPUS;
			sprintf(name,"test-%d",0);
			printf("Creating test thread=%s,pri=%d,cpu=%d\n",name,pri,cpu_id);
			t = thread_create_ex(name,thread_test0, NULL, 0,cpu_id,pri,0);
		    sprintf(name,"test-%d",1);
			
			t = thread_create_ex(name,thread_test1, NULL, 0,cpu_id,pri,0);
			sprintf(name,"test-%d",2);
			t = thread_create_ex(name,thread_test2, NULL, 0,cpu_id,pri,0);
		}
		#else
		
		//printf("\n#######main_init#########\n");
		#endif
			
	
	}
}

#define PATTERN_TO_WRITE 7
static int *mem_test(void *arg)
{
    unsigned int i = 0; 
    const unsigned int HEAP_SIZE= (128);
    thread_t *me = get_current_thread(get_cpu_id());

    for(;;)
    {
        char *p = malloc(HEAP_SIZE);
        if(p)
        {
            unsigned int walk=0;

            ++i;
            memset(p, PATTERN_TO_WRITE+get_cpu_id(), HEAP_SIZE);

            for(; walk< (HEAP_SIZE/sizeof(char)); ++walk)
            {
                if(*(p+walk) != (char)(PATTERN_TO_WRITE+get_cpu_id()))
                for(;;)
                    tlog("!!!!!!! DANGEROUS[%d] core:%d address: 0x%08x is 0x%08x (it should be 0x%08x)!!!!!!!\n", i, get_cpu_id(), (p+walk), *(p+walk), PATTERN_TO_WRITE+get_cpu_id());
            }
            
            memset(p, 0, HEAP_SIZE);           
            free(p);

            if((i%(100000-1)) == 0)
            {
                tlog("cpu:%d cnt:%d PA:[0x%08lx]~[0x%08lx]\n", get_cpu_id(), i, p, p+HEAP_SIZE);
                i=0;
            }
        }
        else
        {
            tlog("malloc fail!!! thread=%s count=%d\n", me->name, i);
            return 0;
        }
    }
    tlog("mem test=%s id done [round-44]\n",me->name);

    return 0;
}
static thread_t *join2;
static int *join_test(void *arg)
{
	int cpu_id = get_cpu_id();
	thread_t *me = get_current_thread(cpu_id);
	u32 wakeup_time;
	int *ret;
	me->start_sleep = arch_counter_get_ms();
	//thread_join(join2,(void **)&ret);
	wakeup_time = arch_counter_get_ms() - me->start_sleep;
	tlog("Join Test thread=%s,Time gap=%d,resource=%d\n",me->name,wakeup_time,mRes);

	return NULL;
}

static int *join_test2(void *arg)
{
	thread_t *me = get_current_thread(get_cpu_id());
	u32 wakeup_time;
	me->start_sleep = arch_counter_get_ms();
	lg_sleep(5);
	wakeup_time = arch_counter_get_ms() - me->start_sleep;
	tlog("Join Test thread=%s,Time gap=%d,resource=%d\n",me->name,wakeup_time,mRes);

	return NULL;
}

static int *cond_test(void *arg)
{
	thread_t *me = get_current_thread(get_cpu_id());
	u32 wakeup_time;
	me->start_sleep = arch_counter_get_ms();
	thread_cond_timedwait(&cond,3);
	wakeup_time = arch_counter_get_ms() - me->start_sleep;
	tlog("Condition Test thread=%s,Time gap=%d,resource=%d\n",me->name,wakeup_time,mRes);

	return NULL;
}

static int *cond_test2(void *arg)
{
	thread_t *me = get_current_thread(get_cpu_id());
	u32 wakeup_time;
	me->start_sleep = arch_counter_get_ms();
	lg_sleep(1);
	thread_cond_signal(&cond);
	wakeup_time = arch_counter_get_ms() - me->start_sleep;
	tlog("Condition Test thread=%s,Time gap=%d,resource=%d\n",me->name,wakeup_time,mRes);

	return NULL;
}
static int *mutex_test(void *arg)
{
	thread_t *me = get_current_thread(get_cpu_id());
	u32 wakeup_time;
	me->start_sleep = arch_counter_get_ms();
	mutex_lock(&mlock);
	lg_sleep(2);
	mRes++;
	mutex_unlock(&mlock);
	wakeup_time = arch_counter_get_ms() - me->start_sleep;
	tlog("Mutex Test thread=%s,Time gap=%d,resource=%d\n",me->name,wakeup_time,mRes);

	return NULL;
}

static int *mutex_test2(void *arg)
{
	thread_t *me = get_current_thread(get_cpu_id());
	u32 wakeup_time;
	int i;
	me->start_sleep = arch_counter_get_ms();
	mutex_lock(&mlock);
	for(i=0; i<1000000;i++) {
		random(1000000);
	}
	mRes++;
	mutex_unlock(&mlock);
	wakeup_time = arch_counter_get_ms() - me->start_sleep;
	tlog("Mutex Test thread=%s,Time gap=%d,resource=%d\n",me->name,wakeup_time,mRes);

	return NULL;
}

 int *thread_test0(void *arg)
{
	int i;
	int cpu_id = get_cpu_id();
	thread_t *me = get_current_thread(cpu_id);
	u32 wakeup_time;
	int rnd,rnd2;
	if( nosleep ) {
		me->start_sleep = arch_counter_get_ms();
		for(i=0; i<1000000;i++) {
			rnd2 = random(1000000);
		}
		wakeup_time = arch_counter_get_ms() - me->start_sleep;
		printf("Test thread=%s sleep=%d,executing,cpsr=%x,cpu=%d,Time gap=%d\n",me->name,rnd,get_cpsr(),get_cpu_id(),wakeup_time);

	} else {
		while(1) {
			rnd = 1;//random(9);
			me->start_sleep = arch_counter_get_ms();
			lg_sleep(rnd);
			wakeup_time = arch_counter_get_ms() - me->start_sleep;

			printf("Test thread=%s sleep=%d,executing,cpsr=%x,cpu=%d,Time gap=%d\n",me->name,rnd,get_cpsr(),get_cpu_id(),wakeup_time);
			
			//schedule();
			
		}
	}

	return NULL;
}
 int *thread_test1(void *arg)
{
	int i;
	int cpu_id = get_cpu_id();
	thread_t *me = get_current_thread(cpu_id);
	u32 wakeup_time;
	int rnd,rnd2;
	if( nosleep ) {
		me->start_sleep = arch_counter_get_ms();
		for(i=0; i<1000000;i++) {
			rnd2 = random(1000000);
		}
		wakeup_time = arch_counter_get_ms() - me->start_sleep;
		printf("Test thread=%s sleep=%d,executing,cpsr=%x,cpu=%d,Time gap=%d\n",me->name,rnd,get_cpsr(),get_cpu_id(),wakeup_time);

	} else {
		while(1) {
			rnd = 2;//random(9);
			me->start_sleep = arch_counter_get_ms();
			lg_sleep(rnd);
			wakeup_time = arch_counter_get_ms() - me->start_sleep;

			printf("Test thread=%s sleep=%d,executing,cpsr=%x,cpu=%d,Time gap=%d\n",me->name,rnd,get_cpsr(),get_cpu_id(),wakeup_time);
			
			//schedule();
			
		}
	}

	return NULL;
}

 int *thread_test2(void *arg)
{
	int i;
	int cpu_id = get_cpu_id();
	thread_t *me = get_current_thread(cpu_id);
	u32 wakeup_time;
	int rnd,rnd2;
	if( nosleep ) {
		me->start_sleep = arch_counter_get_ms();
		for(i=0; i<1000000;i++) {
			rnd2 = random(1000000);
		}
		wakeup_time = arch_counter_get_ms() - me->start_sleep;
		printf("Test thread=%s sleep=%d,executing,cpsr=%x,cpu=%d,Time gap=%d\n",me->name,rnd,get_cpsr(),get_cpu_id(),wakeup_time);

	} else {
		while(1) {
			rnd = random(5);
			
			me->start_sleep = arch_counter_get_ms();
			lg_sleep(rnd);
			wakeup_time = arch_counter_get_ms() - me->start_sleep;

			printf("Test thread=%s sleep=%d,executing,cpsr=%x,cpu=%d,Time gap=%d\n",me->name,rnd,get_cpsr(),get_cpu_id(),wakeup_time);
			//schedule();
			
		}
	}

	return NULL;
}

void create_thread_test(void)
{
	int i;
	char name[16];
	int pri;

	thread_t *t;
	static int cpu_id = 0;
	for( i=0; i < 3;i++ ) {
		sprintf(name,"test-%d",i);
		//pri = random(THREAD_MAX_PRIORITY-THREAD_MIN_PRIORITY);
		pri = THREAD_DEFAULT_PRIORITY;
		//cpu_id = (cpu_id+1) % NR_CPUS;
		printf("Creating test thread=%s,pri=%d,cpu=%d\n",name,pri,cpu_id);
		t = thread_create_ex(name,thread_test0, NULL, 0,cpu_id,pri,0);
		if( t == NULL ) break;
	}
}

void display_all_threads(void)
{
	thread_t *thread;
	list_t *q = get_thread_queue().next;
	int i = 0;
	tlog("## Display all threads(%d)\n",active_count);
	for( ; q != &get_thread_queue(); q = q->next ) {

		thread = get_thread_ptr_threadq(q);
		tlog("%d : thread(%p) name=%s,priority=%d,state=%s,cpu=%d\n",i++,thread,thread->name,thread->priority,thread_state_str[thread->state],thread->cpu_id);
	}
}
int do_thread_test(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	if(argc < 2) {
		tlog ("Usage:\n%s\n", cmdtp->usage);
		//tlog ("%s\n", cmdtp->help);
		return 1;
	}

	if( !strcmp(argv[1],"create") ) {
		if( argc > 2 )  {
			if( !strcmp(argv[2],"nosleep") ) {
				nosleep = 1;
				create_thread_test();
			}
		} else {
			nosleep = 0;
			create_thread_test();
		}
	} else if( !strcmp(argv[1],"on") ) {
	} else if( !strcmp(argv[1],"timer")) {
        timer_id = arch_create_timer(oneshot_timer_callback, 10, 2, "ONE SHOT TIMER", 0);
       		tlog("XXX oneshot timer=%d\n",timer_id);
	} else if( !strcmp(argv[1],"start")) {
		arch_start_timer(timer_id);
	} else if( !strcmp(argv[1],"reset")) {
       		arch_reset_timer(timer_id,2000,2);
	} else if( !strcmp(argv[1],"sleep") ) {
		int rnd = random(5);
		lg_sleep(rnd);
	} else if( !strcmp(argv[1],"spinlock") ) {
		int  x = 0;
		int ret;
		spin_lock_t lock;
		spin_lock_init(&lock);
		spin_lock(&lock);
		for(x;x>1000000;x++) {
		}
		ret = spin_trylock(&lock);
		tlog("XXX 2nd trylcok=%d\n",ret);
		spin_unlock(&lock);

	} else if( !strcmp(argv[1],"display") ) {
		display_all_threads();
	} else if( !strcmp(argv[1],"mutex") ) {
		mutex_new(&mlock);
		thread_create("mutex",mutex_test, NULL);
		thread_create("mutex2",mutex_test2, NULL);
	} else if( !strcmp(argv[1],"cond") ) {
		thread_cond_new(&cond);
		thread_create("cond",cond_test, NULL);
		thread_create("cond2",cond_test2, NULL);
	} else if( !strcmp(argv[1],"join") ) {
		//join2 = thread_create("join2",join_test2, NULL);
		thread_t *join = thread_create_ex("join",join_test, NULL,0,1,THREAD_DEFAULT_PRIORITY,1);
		lg_sleep(2);
		thread_join(join,NULL);
	} else if( !strcmp(argv[1],"ipi") ) {
		ipi_send_self(IPI_NEED_RESCHEDULE);
	} else if( !strcmp(argv[1],"broad") ) {
		ipi_broadcast(IPI_TEST_1);
	} else if( !strcmp(argv[1],"target") ) {
		int cpu = simple_strtoul(argv[2], NULL, NULL);
		ipi_send_target(cpu,IPI_TEST_2);
	} else if( !strcmp(argv[1],"mem") ) {
		//char * p =malloc(300);
		//p = realloc(p,400);
		//free(p);
		//mem_test(NULL);
		tlog("start mem test\n");
		thread_create_ex("mem_1",mem_test, NULL,0,1,THREAD_DEFAULT_PRIORITY,0);
		thread_create_ex("mem_2",mem_test, NULL,0,2,THREAD_DEFAULT_PRIORITY,0);
		thread_create_ex("mem_3",mem_test, NULL,0,3,THREAD_DEFAULT_PRIORITY,0);
	} else {
		tlog("Unknown command for thread\n");
	}

	return 0;
}

u32 random(u32 limit)
{
	static u32 seed = 0;
	u32 next, res;

	if( seed == 0 ) seed = (arch_counter_get_ticks()+51284);
	next = seed;

	next *= 1103515245;
	next += 12345;
	res = (unsigned int) (next / 65536) % 2048;

	next *= 1103515245;
	next += 12345;
	res <<= 10;
	res ^= (unsigned int) (next / 65536) % 1024;

	next *= 1103515245;
	next += 12345;
	res <<= 10;
	res ^= (unsigned int) (next / 65536) % 1024;

	seed = next;

	return res % limit;
}

U_BOOT_CMD(
	thread,	  4,	  0,	  do_thread_test,
	"thread create\n",
	"thread display \n"
);
