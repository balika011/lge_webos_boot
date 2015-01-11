#include <stdarg.h>
#include <exports.h>
#include <config.h>
#include <x_typedef.h>
#include "thread.h"
#include <errno.h>

#define get_cpu_id()	get_current_cpu()

#if 1//defined(CONFIG_MULTICORES_PLATFORM)
#define SMP_WAIT_NON_BOOT_CPU_RELEASE_COUNT 10000
extern thread_t *thread[];
#endif

#if 1//defined(CONFIG_MULTICORES_PLATFORM)
//typedef void ( *TimerCb ) (MS_U32 u32StTimer, MS_U32 u32TimerID);  ///< Timer callback function  u32StTimer: not used; u32TimerID: Timer ID
u64 arch_counter_get_ticks(void);

u64 Tick2MS = 0;
void arch_counter_calibration(void)
{
    unsigned int clock_freq;
    unsigned int SysClock;        
    u64          PrevTick, PostTick;

    //asm volatile("mcr p15, 0, %0, c14, c2, 1" : : "r" (val));
    asm volatile("mrc p15, 0, %0, c14, c0, 0" : "=r" (clock_freq));
    //asm volatile("mrrc p15, 0, %0, %1, c14" : "=r" (cvall), "=r" (cvalh));

  //  printf("[%x] clock_freq\n", clock_freq);    
    //printf("0x[%x][%08x] 64bis Count\n", cvalh, cvall);

    PrevTick = arch_counter_get_ticks();  

    //SysClock = MsOS_GetSystemTime();
    // Check 10 ms
    while(1)
    {
       // if(SysClock+10 <= MsOS_GetSystemTime())
            break;
    }
    //asm volatile("mrrc p15, 0, %0, %1, c14" : "=r" (cvall), "=r" (cvalh)); 
    PostTick = arch_counter_get_ticks();  
    
	
    //printf("==> [%x] SysClock, [%x] Current\n", SysClock, MsOS_GetSystemTime()); 
    //printf("%llu 64bis Count\n", PostTick-PrevTick);   
    Tick2MS = (PostTick-PrevTick)/10;

   // printf("1 ms = %llu ticks\n", Tick2MS);   
    
}

u64 arch_counter_get_ticks(void)
{
	u64 cval;

	isb();
	asm volatile("mrrc p15, 0, %Q0, %R0, c14" : "=r" (cval));
	return cval;
}

/* clock source */
u64 arch_counter_get_ms(void)
{
	u64 cval;
	unsigned int clock_freq;
	u64 Time_ms;

   cval = arch_counter_get_ticks();
   asm volatile("mrc p15, 0, %0, c14, c0, 0" : "=r" (clock_freq));
	return cval*1000 / clock_freq ;
}


#define PERIOD_TIMER            1
#define ONE_SHOT_TIMER         2

void tputs(const char *s)
{
    if(s)
        tlog("%s", s);
}
extern UINT32 _TimerInit();
extern void _TimeReset(void);


/* clock event */
u32 arch_create_timer(TimerCb pTimerCb, u32 ElapseTimeMs, u32 Mode, char*pName, u32 CoreID)
{
#if 1
    if(Mode == ONE_SHOT_TIMER)
        if(CoreID == 0)
           return MsOS_CreateTimer(pTimerCb, ElapseTimeMs, 0xF5F5F5F5, TRUE, pName);
        else
           return MsOS_CreateTimer_core(pTimerCb, ElapseTimeMs, 0xF5F5F5F5, TRUE, pName, CoreID);
    else
        if(CoreID == 0)        
            return MsOS_CreateTimer(pTimerCb, ElapseTimeMs, ElapseTimeMs, TRUE, pName);
        else
            return MsOS_CreateTimer_core(pTimerCb, ElapseTimeMs, ElapseTimeMs, TRUE, pName, CoreID);
#else
	
  	_TimerInit();
	//Interrupt_request(2, pTimerCb, NULL);

	return 0;
#endif
}

BOOL arch_reset_timer(u32 u32TimerId, u32 ElapseTimeMs, u32 Mode)
{
    u32 CoreID;
    CoreID =0;
#if 1

    if(Mode == ONE_SHOT_TIMER)
        if(CoreID == 0)
            return MsOS_ResetTimer(ElapseTimeMs);
        else
            return 1;//MsOS_ResetTimer_core(u32TimerId, ElapseTimeMs, 0xF5F5F5F5, TRUE, CoreID);
    else
    	{
    	}
    //    if(CoreID == 0)    
           // return MsOS_ResetTimer(u32TimerId, ElapseTimeMs, ElapseTimeMs, TRUE);
     //   else
           // return MsOS_ResetTimer_core(u32TimerId, ElapseTimeMs, ElapseTimeMs, TRUE, CoreID);
		
#else
//_TimerInit(pTimerCb);

#endif
}

BOOL arch_stop_timer(u32 u32TimerId)
{
	return 0;//unuse
}

BOOL arch_start_timer(u32 u32TimerId)
{
	return 0;//unuse
}

BOOL arch_del_timer(u32 u32TimerId)
{
	return 0;//unuse
}

BOOL arch_thread_create(const char *name,void *(*start)(void *arg), void *arg, int stk_size,int cpu_id,int priority,int join)
{
    return (UINT32) thread_create_ex(name, start, arg, stk_size, cpu_id, priority, join);
}

BOOL arch_mutex_new(void)
{
    mutex_t  *pMutex;  
    pMutex = (mutex_t*) malloc(sizeof(mutex_t), 0);
    mutex_new(pMutex);
    return pMutex;
}
BOOL arch_mutex_lock(UINT32 mutexAdr)
{
    return mutex_lock((mutex_t*)mutexAdr);
}

BOOL arch_mutex_unlock(UINT32 mutexAdr)
{    
    return mutex_unlock((mutex_t*)mutexAdr);
}

u32 Time_Core[4] = {0, 0, 0, 0};

static void arch_period_timer_callback(UINT32 stTimer, UINT32 u32Data)
{
    static int count = 0;
    static int sec = 0;
    unsigned int CPUID;

    CPUID = get_cpu_id();
    Time_Core[CPUID]++;
    
}

#endif

