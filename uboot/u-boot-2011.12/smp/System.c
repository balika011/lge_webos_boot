#include <stdarg.h>
#include <exports.h>
#include <config.h>
#include <x_typedef.h>
#include <thread.h>
#include <errno.h>
#include "smp_platform.h"
#include <serial.h>

#define get_cpu_id()	get_current_cpu()
typedef void (*CoreWakeupFunc ) (void);
volatile unsigned int GlobalActive_Area[5] = {0, 0, 0, 0, 0};
volatile CoreWakeupFunc ActiveFuncPointer[5] = {0, 0, 0, 0, 0};

#if 1//defined(CONFIG_MULTICORES_PLATFORM)
#define SMP_WAIT_NON_BOOT_CPU_RELEASE_COUNT 10000
extern thread_t *thread[];
#endif
#if defined(CONFIG_MULTICORES_PLATFORM)
extern void init_secondary_cpu(void);
#endif
void flush_cache_all(void)
{
	HalFlushDCache();

}
void mt53xx_Core1StubSimple(void)
{
#if 1//defined(CONFIG_ARCH_MT5890)||defined(CONFIG_ARCH_MT5882)
    unsigned int addr = 0xf0008068, magic = 0xffffb007;

    __asm__ ("isb");

    __asm__ ("LOOP:");
    __asm__ ("wfe");
    __asm__ ("MOV     r0, %0" : : "r" (addr));
    __asm__ ("MOV     r1, %0" : : "r" (magic));
    __asm__ ("ldr     r2, [r0]");
    __asm__ ("cmp     r2, r1");
    __asm__ ("bne     LOOP");

    addr = 0xf0008188;

    __asm__ ("MOV     r0, %0" : : "r" (addr));
    __asm__ ("ldr     r1, [r0]");
    __asm__ ("blx     r1");
#endif // defined(CONFIG_ARCH_MT5890)
}
static inline u32 __bim_readl(u32 regaddr32)
{
    return __raw_readl(0xf0008000 + regaddr32);
}

static inline void __bim_writel(u32 regval32, u32 regaddr32)
{
    __raw_writel(regval32, 0xf00008000 + regaddr32);
}

// CPU Power Control
#define REG_CA15CLK_CTL		(0x0400)	// CA15L CLK CONTROL
    #define ARMPLL_CK_OFF	(1U << 0)
    #define PLL1_CK_OFF		(1U << 1)
    #define ARM_K1			(0xffU << 2)
    #define MUX1_SEL		(3U << 10)
    #define DCM_ENABLE		(1U << 12)
    #define ARMWFI_DCM_EN	(1U << 13)
    #define ARMWFE_DCM_EN	(1U << 14)
    #define FCA9_CLK_OFF	(1U << 15)
    #define SC_ARMCK_OFF_EN	(1U << 16)
    #define OCC_CTL			(0x3fU << 17)
#define REG_CKDIV			(0x0404)	// CKDIV
#define REG_GIC_CTL			(0x0408)	// GIC CONTROL
#define REG_CPU1_PWR_CTL	(0x0414)	// CPU1 POWER CONTROL
#define REG_CPU2_PWR_CTL	(0x0418)	// CPU2 POWER CONTROL
#define REG_CPU3_PWR_CTL	(0x041c)	// CPU3 POWER CONTROL
    #define MEM_PD  	    (1U << 0)
	#define MEM_ISOINTB  	(1U << 1)
	#define MEM_CKISO  	        (1U << 2)
	#define MEM_PD_ACK  	    (1U << 3)
	#define PWR_CLK_DIS  	(1U << 4)
    #define PWR_RST_EN  	(1U << 5)
    #define PWR_ON			(1U << 6)
    #define PWR_ON_2ND		(1U << 7)
    #define PWR_ON_ACK		(1U << 8)
    #define PWR_ON_2ND_ACK	(1U << 9)
	#define CLAMP       	(1U << 10)


void BIM_CpuCA17Up(unsigned int cpu)
{
	unsigned int regval32;
    cpu=cpu-1;
    //set pwr on to "1"
    regval32 = __bim_readl(REG_CPU1_PWR_CTL + 4* cpu);
    regval32 |= PWR_ON;
    __bim_writel(regval32, REG_CPU1_PWR_CTL + 4 * cpu);
    do {
        regval32 = __bim_readl(REG_CPU1_PWR_CTL + 4 * cpu);
    }while(!(regval32 & PWR_ON_ACK));

    //set pwr on 2nd to "1"
    regval32 = __bim_readl(REG_CPU1_PWR_CTL + 4 * cpu);
    regval32 |= PWR_ON_2ND;
    __bim_writel(regval32, REG_CPU1_PWR_CTL + 4 * cpu);

    #ifndef CONFIG_MT53_FPGA
    do {
        regval32 = __bim_readl(REG_CPU1_PWR_CTL + 4 * cpu);
    }while(!(regval32 & PWR_ON_2ND_ACK));
    #endif
    udelay(20);

    // set pwr clk dis to "0"
    regval32 = __bim_readl(REG_CPU1_PWR_CTL + 4* cpu);
    regval32 &= ~PWR_CLK_DIS;
    __bim_writel(regval32, REG_CPU1_PWR_CTL + 4 * cpu);
    udelay(20);

    //set pd to "0"
    regval32 = __bim_readl(REG_CPU1_PWR_CTL + 4 * cpu);
    regval32 &= ~MEM_PD;
    __bim_writel(regval32, REG_CPU1_PWR_CTL + 4 * cpu);
    do {
        regval32 = __bim_readl(REG_CPU1_PWR_CTL + 4 * cpu);
    }while((regval32 & MEM_PD_ACK));
    udelay(20);

    //set clamp to "0"
    regval32 = __bim_readl(REG_CPU1_PWR_CTL + 4 * cpu);
    regval32 &= ~CLAMP;
    __bim_writel(regval32, REG_CPU1_PWR_CTL + 4 * cpu);
    udelay(20);

    // set pwr reset en to"0"
    regval32 = __bim_readl(REG_CPU1_PWR_CTL + 4* cpu);
    regval32 &= ~PWR_RST_EN;
    __bim_writel(regval32, REG_CPU1_PWR_CTL + 4 * cpu);
    udelay(20);

   cpu=cpu+1;
}

void BIM_CpuCA17Down(unsigned int cpu)
{
	unsigned int regval32;
    cpu=cpu-1;
	printf("BIM_CpuCA17Down %d \n",cpu);
    //set pd to "1" //ok
    regval32 = __bim_readl(REG_CPU1_PWR_CTL + 4 * cpu);
    regval32 |= MEM_PD;
    __bim_writel(regval32,REG_CPU1_PWR_CTL + 4 * cpu);
    do {
        regval32 = __bim_readl(REG_CPU1_PWR_CTL + 4 * cpu);
    }while(!(regval32 & MEM_PD_ACK));

     // Set clamp bit to "1" //ng
    regval32 = __bim_readl(REG_CPU1_PWR_CTL + 4 * cpu);
    regval32 |= CLAMP;
    __bim_writel(regval32,REG_CPU1_PWR_CTL + 4 * cpu);
    udelay(20);

    // Set pwr clk_dis bit to "1"
    regval32 = __bim_readl(REG_CPU1_PWR_CTL + 4 * cpu);
    regval32 |= PWR_CLK_DIS;
    __bim_writel(regval32,REG_CPU1_PWR_CTL + 4 * cpu);
    udelay(20);

    // Set reset enable bit to "1"
    regval32 = __bim_readl(REG_CPU1_PWR_CTL + 4 * cpu);
    regval32 |= PWR_RST_EN;
    __bim_writel(regval32,REG_CPU1_PWR_CTL + 4 * cpu);
    udelay(20);

    //set pwr on to "0" //ok
    regval32 = __bim_readl(REG_CPU1_PWR_CTL + 4 * cpu);
    regval32 &= ~PWR_ON;
    __bim_writel(regval32,REG_CPU1_PWR_CTL + 4 * cpu);

    do {
        regval32 = __bim_readl(REG_CPU1_PWR_CTL + 4 * cpu);
    }while( regval32 & PWR_ON_ACK);
    udelay(20);

    //set pwr on 2nd to "0" //ng
    regval32 = __bim_readl(REG_CPU1_PWR_CTL + 4 * cpu);
    regval32 &= ~PWR_ON_2ND;
    __bim_writel(regval32,REG_CPU1_PWR_CTL + 4 * cpu);

    do {
        regval32 = __bim_readl(REG_CPU1_PWR_CTL + 4 * cpu);
    }while( regval32 & PWR_ON_2ND_ACK);
    udelay(20);

    cpu =cpu+1;

}
void secondary_start_uboot(void)
{
#if defined(CONFIG_MULTICORES_PLATFORM)

    printf("init_secondary_cpu = %x\n", init_secondary_cpu);
	__raw_writel((init_secondary_cpu),
		 0xf0008188);
	#endif
	#if 0
	__raw_writel(0xffffb007,
		 0xf0008068);
	//memcpy((void *)0, mt53xx_Core1StubSimple, 0x100);
	/* make sure write buffer is drained */
		
	asm volatile("dsb");
		

		/* Wakeup sleepers */
		asm volatile ("sev");
		BIM_CpuCA17Up(1);
		BIM_CpuCA17Up(2);
		BIM_CpuCA17Up(3);
		printf("we turn on CPU \n");
	#endif
   // writel(init_secondary_cpu, CONFIG_KERNEL_START_ADDRESS+SMP_DUMMY_PA_START);
 //   writel(0xbabe, CONFIG_KERNEL_START_ADDRESS+SMP_DUMMY_MAGIC);
  // flush_cache_all();
// while(1);
    return;
}


typedef void (*PFN_IMAGE)(void);

void secondary_start_uboot_cleanup(void)
{
   #if 1 
    unsigned int *mp_start_addr;
    PFN_IMAGE pfnImage;
	
	if(get_cpu_id()==0) return ;
	if(smp_cpu_released[get_cpu_id()].lock ==1) return;
    mp_start_addr=0xfb00bf00;
		__raw_writel(0,
		 0xf0008068);
	__raw_writel(0,
		 0xf0008188);
	__raw_writel((0xffffffff),
		 0xf000806c);
	
	smp_cpu_released[get_cpu_id()].lock =1;
	local_irq_disable();
	
	asm volatile("mcr p15, 0, %0, c14, c2, 1" : : "r" (0));//clean arch timer irq enable bit
	
    pfnImage = (PFN_IMAGE)(mp_start_addr);
	flush_cache_all();
	HalDisableCaches();
    HalDisableMMU();	
	asm volatile("dsb");
    pfnImage();
   // writel(0x0, CONFIG_KERNEL_START_ADDRESS+SMP_DUMMY_PA_START);
   // writel(0x0, CONFIG_KERNEL_START_ADDRESS+SMP_DUMMY_MAGIC);

   #endif
    return;
}


extern void set_irq_sp(ulong);
extern void __init_dic(void);
extern void __init_interrupts(void);
extern u32 get_cpu_mask(u32 cpuid);
void secondary_init_r(unsigned int cpu_id)
{
#if 1
    u32 cpuID;
    //u64 prevous_time;    
   // scu_enable((void *)(0x16004000)); // SCU PA = 0x16004000
    //writel(SMP_DBG_IN_C_CODE, SMP_RIU_BASE + (SMP_DUMMY_BANK<<1) + cpu_id*4);
  GIC_Init();
   // __init_dic();
   // __init_interrupts();
   /// ipi_send(get_cpu_mask(cpu_id),2);
    //mhal_interrupt_unmask(2)
    // enable Timer
   // MAsm_CPU_TimerInit();
   // MAsm_CPU_TimerStart();
   flush_cache_all();

    cpuID = get_cpu_id();
if(cpuID==1)
{
  printf(" secondary_init_r 0x%x\n",cpu_id);
}
    if(cpuID >= 4)
    {
        printf("Error!!, Not correct CPUID %d\n", cpuID);
        while(1);
    }

  //  extern void set_irq_sp(ulong);
    unsigned int sp_addr =*((unsigned int *)(CONFIG_KERNEL_START_ADDRESS+SMP_DUMMY_SP));
    sp_addr = sp_addr - (2+cpuID)*0x4000;
    set_irq_sp(sp_addr);
    enable_interrupts();
    while(1)
    {
   // printf("2 secondary_init_r 0x%x\n",cpu_id);
   if(cpuID==1)
{
  printf(" GlobalActive_Area[cpuID] 0x%x\n",GlobalActive_Area[cpuID]);
  printf(" ActiveFuncPointer[cpuID] 0x%x\n",ActiveFuncPointer[cpuID]);
}
  
        if((GlobalActive_Area[cpuID] == 1) && (ActiveFuncPointer[cpuID] != 0))
            ActiveFuncPointer[cpuID]();
    }
	#endif
  
}


void Core_Wakeup(CoreWakeupFunc __Addr, u32 CoreID)
	{
#if 1
	if(CoreID ==1)
	{
		__raw_writel(0xffffb007,
			 0xf0008068);
		__raw_writel((0x10101010),
		 0xf000806c);
		//memcpy((void *)0, mt53xx_Core1StubSimple, 0x100);
		/* make sure write buffer is drained */
			
		asm volatile("dsb");
			
	
			/* Wakeup sleepers */
			asm volatile ("sev");
	}
			//BIM_CpuCA17Up(CoreID);
		//	printf("we turn on CPU %x\n",CoreID);
		if(CoreID >= 4)
		{
			printf("Error!! in Core_Wakeup, Not correct CPUID %d\n", CoreID);
			while(1);
		}
	
		if(GlobalActive_Area[CoreID] == 1)
		{
			printf("Core %d is active\n", CoreID);
			return;
		}
	
	//printf("###################Wake UP Core %d, PC %x\n", CoreID, (unsigned int)__Addr);
		udelay(100);  // Wait printf finish (Critical Section problem. Need multi core lock)
		ActiveFuncPointer[CoreID] =  __Addr;
		GlobalActive_Area[CoreID] = 1;	  
		//flush_cache_all();	
		
#endif
	}

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
           return MsOS_CreateTimer_core(pTimerCb, ElapseTimeMs, 0xF5F5F5F5, TRUE, pName,0);
        else
           return MsOS_CreateTimer_core(pTimerCb, ElapseTimeMs, 0xF5F5F5F5, TRUE, pName, CoreID);
    else
        if(CoreID == 0)        
            return MsOS_CreateTimer_core(pTimerCb, ElapseTimeMs, ElapseTimeMs, TRUE, pName,0);
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

