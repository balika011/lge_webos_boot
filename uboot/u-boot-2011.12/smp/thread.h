#ifndef _LG_THREAD_H_
#define _LG_THREAD_H_
#include <x_typedef.h>

#include "thread_info.h"
#include "smp.h"
typedef void ( *TimerCb ) (UINT32 u32StTimer, UINT32 u32TimerID);  ///< Timer callback function  u32StTimer: not used; u32TimerID: Timer ID

u64 arch_counter_get_ms(void);
BOOL arch_reset_timer(u32 u32TimerId, u32 ElapseTimeMs, u32 Mode);
u32 arch_create_timer(TimerCb pTimerCb, u32 ElapseTimeMs, u32 Mode, char*pName, u32 CoreID);

thread_t *thread_create_ex(const char *name,void *(*start)(void *arg), void *arg, int stk_size,int cpu_id,int priority,int join);
thread_t *thread_create(const char *name,void *(*start)(void *arg), void *arg);

thread_t *thread_init(void);
thread_t *thread_init_per_cpu(void);

void thread_start(void);
void thread_exit(void *retval);
int thread_join(thread_t *thread, void **retvalp);
int thread_cond_new(cond_t *cvar);
//int thread_cond_destroy(cond_t *cvar);
int thread_cond_timedwait(cond_t *cvar, utime_t timeout);
int thread_cond_signal(cond_t *cvar);

//void mutex_new(mutex_t *lock);
int mutex_destroy(mutex_t *lock);
int mutex_lock(mutex_t *lock);
int mutex_unlock(mutex_t *lock);
int mutex_trylock(mutex_t *lock);


void add_sleep_q(thread_t *thread, utime_t timeout);
void del_sleep_q(thread_t *thread);

void create_thread_test(void);
void display_run_threads(void);
void display_all_threads(void);

int lg_sleep(int secs);
int lg_msleep(unsigned int msecs);
#endif

