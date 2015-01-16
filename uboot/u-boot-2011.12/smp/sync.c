#include <stdarg.h>
#include <exports.h>
#include <config.h>
#include <x_typedef.h>
#include <thread.h>
#include <errno.h>
#include <thread_info.h>

#define get_cpu_id()	get_current_cpu()

extern spin_lock_t g_sched_lock;
extern void add_run_queue(thread_t *thread);
extern void add_sleep_q(thread_t *thread, utime_t timeout);
#define NR_CPUS 4
extern spin_lock_t init_done[NR_CPUS];
extern int megic_number_cleaned ;
extern spin_lock_t smp_cpu_released[NR_CPUS];
volatile unsigned int waitCount=0;
#if defined(CONFIG_MULTICORES_PLATFORM)

void release_non_boot_core(void)
{
	if(init_done[1].lock && init_done[2].lock && init_done[3].lock)
	{
	
		megic_number_cleaned=1;
		//thread_cond_signal(&g_sub_cond[1]);
		//thread_cond_signal(&g_sub_cond[2]);
		//thread_cond_signal(&g_sub_cond[3]);
		
		//secondary_start_uboot_cleanup();
	}
	else
	{
		tlog("Should not be here!!!\n");
	}

	while((!smp_cpu_released[1].lock) || (!smp_cpu_released[2].lock) || (!smp_cpu_released[3].lock))
	{
		++waitCount;
		//if (!smp_cpu_released[1]) 
		//	tlog("smp_cpu_not_released_1!\n");

		//if (!smp_cpu_released[2]) 
		//	tlog("smp_cpu_not_released_2???!\n");
		//if (!smp_cpu_released[3]) 
		//	tlog("smp_cpu_not_released_3???!\n");

		if (2000<waitCount)
			{
				//tlog("release_non_boot_core smp_cpu_released =%x\n", smp_cpu_released[get_cpu_id()].lock);
			}
	}
}
#else
void release_non_boot_core(void)
{
}

#endif


int thread_cond_new(cond_t *cvar)
{
	if (cvar) {
		INIT_LIST(&(cvar->links));
		spin_lock_init(&cvar->lock);
		return 0;
	}
	return -1;
}

int thread_cond_timedwait(cond_t *cvar, utime_t timeout)
{
	int cpu_id = get_cpu_id();
	thread_t *me = get_current_thread(cpu_id);
	int rv;

	if (me->flags & FLAG_INTERRUPT) {
		me->flags &= ~FLAG_INTERRUPT;
		return -EINTR;
	}

	/* Put caller thread on the condition variable's wait queue */
	me->state = STATE_COND_WAIT;
	me->flags |= FLAG_FIX_CPU;
	spin_lock_save(&cvar->lock,cvar->flags);
	APPEND_LINK(&me->wait_link, &cvar->links);
	spin_unlock_restore(&cvar->lock,cvar->flags);

	if (timeout != NO_TIMEOUT) {
		//timeout *= MILLISECOND;
		add_sleep_q(me, timeout);
	}

	schedule();

	spin_lock_save(&cvar->lock,cvar->flags);
	REMOVE_LINK(&me->wait_link);
	spin_unlock_restore(&cvar->lock,cvar->flags);
	rv = 0;

	if (me->flags & FLAG_TIMEDOUT) {
		me->flags &= ~FLAG_TIMEDOUT;
		rv = -ETIME;
	}
	if (me->flags & FLAG_INTERRUPT) {
		me->flags &= ~FLAG_INTERRUPT;
		rv = -EINTR;
	}

	return rv;
}


int thread_cond_wait(cond_t *cvar)
{
	return thread_cond_timedwait(cvar, NO_TIMEOUT);
}


static int cond_signal(cond_t *cvar, int broadcast)
{
	thread_t *thread;
	list_t *q;

	int cpu_id = get_cpu_id();

	spin_lock_save(&cvar->lock,cvar->flags);
	for (q = cvar->links.next; q != &cvar->links; q = q->next) {
		thread = get_thread_ptr_waitq(q);
		if (thread->state == STATE_COND_WAIT) {
			if (thread->flags & FLAG_ON_SLEEPQ) {
				//TODO : temp code
				thread->cpu_id = 0;
				del_sleep_q(thread);
			}

			spin_lock_irq(&g_sched_lock);
			add_run_queue(thread);
			spin_unlock_irq(&g_sched_lock);
			if (!broadcast) break;
		}
	}
	spin_unlock_restore(&cvar->lock,cvar->flags);

	return 0;
}


int thread_cond_signal(cond_t *cvar)
{
	return cond_signal(cvar, 0);
}


int thread_cond_broadcast(cond_t *cvar)
{
	return cond_signal(cvar, 1);
}


/*****************************************
 * Mutex functions
 */

int mutex_new(mutex_t *lock)
{
	if (lock) {
		INIT_LIST(&(lock->wait.links));
		spin_lock_init(&lock->wait.lock);
		lock->owner = NULL;
		return 0;
	}
	return -EINVAL;
}

int mutex_lock(mutex_t *lock)
{
	int cpu_id = get_cpu_id();
	thread_t *me = get_current_thread(cpu_id);

	if (me->flags & FLAG_INTERRUPT) {
		me->flags &= ~FLAG_INTERRUPT;
		return -EINTR;
	}

	if (lock->owner == NULL) {
		/* Got the mutex */
		lock->owner = me;
		return 0;
	}

	if (lock->owner == me) {
		return -EDEADLK;
	}

	me->state = STATE_LOCK_WAIT;
	me->flags |= FLAG_FIX_CPU;
	spin_lock_save(&lock->wait.lock,lock->wait.flags);
	APPEND_LINK(&me->wait_link, &lock->wait.links);
	spin_unlock_restore(&lock->wait.lock,lock->wait.flags);
	schedule();

	spin_lock_save(&lock->wait.lock,lock->wait.flags);
	REMOVE_LINK(&me->wait_link);
	spin_unlock_restore(&lock->wait.lock,lock->wait.flags);

	if ((me->flags & FLAG_INTERRUPT) && lock->owner != me) {
		me->flags &= ~FLAG_INTERRUPT;
		return -EINTR;
	}

	return 0;
}


int mutex_unlock(mutex_t *lock)
{
	thread_t *thread;
	list_t *q;
	int cpu_id = get_cpu_id();

	if (lock->owner != get_current_thread(cpu_id)) {
		return -EPERM;
	}

	spin_lock_save(&lock->wait.lock,lock->wait.flags);
	for (q = lock->wait.links.next; q != &lock->wait.links; q = q->next) {
		thread = get_thread_ptr_waitq(q);
		if (thread->state == STATE_LOCK_WAIT) {
			lock->owner = thread;
			spin_lock_irq(&g_sched_lock);
			add_run_queue(thread);
			spin_unlock_irq(&g_sched_lock);
			spin_unlock_restore(&lock->wait.lock,lock->wait.flags);
			return 0;
		}
	}
	spin_unlock_restore(&lock->wait.lock,lock->wait.flags);

	lock->owner = NULL;

	return 0;
}


int mutex_trylock(mutex_t *lock)
{
	int cpu_id = get_cpu_id();
	if (lock->owner != NULL) {
		return -EBUSY;
	}

	/* Got the mutex */
	lock->owner = get_current_thread(cpu_id);

	return 0;
}

