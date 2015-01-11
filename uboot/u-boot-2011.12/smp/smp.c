#include "smp.h"
#include "thread_info.h"
#include "thread.h"
#include "../../drv_lib/inc/x_bim.h"
typedef unsigned long UINT32;
typedef void (*x_os_drv_isr_fct) (unsigned long ui2_vector_id);

#define GIC_DIST_SOFTINT        0xf00

#define ARCH_TIMER_CTRL_ENABLE		(1 << 0)
#define ARCH_TIMER_CTRL_IT_MASK		(1 << 1)
#define ARCH_TIMER_CTRL_IT_STAT		(1 << 2)

#define ARCH_TIMER_REG_CTRL		0
#define ARCH_TIMER_REG_TVAL		1

#define ARCH_TIMER_PHYS_ACCESS		0
#define ARCH_TIMER_VIRT_ACCESS		1

static inline void arch_timer_reg_write(const int access, const int reg, u32 val)
{
	if (access == ARCH_TIMER_PHYS_ACCESS) {
		switch (reg) {
		case ARCH_TIMER_REG_CTRL:
			asm volatile("mcr p15, 0, %0, c14, c2, 1" : : "r" (val));
			break;
		case ARCH_TIMER_REG_TVAL:
			asm volatile("mcr p15, 0, %0, c14, c2, 0" : : "r" (val));
			break;
		}
	}

	if (access == ARCH_TIMER_VIRT_ACCESS) {
		switch (reg) {
		case ARCH_TIMER_REG_CTRL:
			asm volatile("mcr p15, 0, %0, c14, c3, 1" : : "r" (val));
			break;
		case ARCH_TIMER_REG_TVAL:
			asm volatile("mcr p15, 0, %0, c14, c3, 0" : : "r" (val));
			break;
		}
	}

	isb();
}

static inline u32 arch_timer_reg_read(const int access, const int reg)
{
	u32 val = 0;

	if (access == ARCH_TIMER_PHYS_ACCESS) {
		switch (reg) {
		case ARCH_TIMER_REG_CTRL:
			asm volatile("mrc p15, 0, %0, c14, c2, 1" : "=r" (val));
			break;
		case ARCH_TIMER_REG_TVAL:
			asm volatile("mrc p15, 0, %0, c14, c2, 0" : "=r" (val));
			break;
		}
	}

	if (access == ARCH_TIMER_VIRT_ACCESS) {
		switch (reg) {
		case ARCH_TIMER_REG_CTRL:
			asm volatile("mrc p15, 0, %0, c14, c3, 1" : "=r" (val));
			break;
		case ARCH_TIMER_REG_TVAL:
			asm volatile("mrc p15, 0, %0, c14, c3, 0" : "=r" (val));
			break;
		}
	}

	return val;
}
typedef struct
{
    TimerCb    pTimerCb;
    void*               pvStack;
    UINT16              u2Vector;
	char *pname;
} OS_timer;
OS_timer     _arTimer;

enum ppi_nr {
	PHYS_SECURE_PPI = 28,
	PHYS_NONSECURE_PPI,
	VIRT_PPI,
	HYP_PPI,
	MAX_TIMER_PPI
};

static int arch_timer_ppi[MAX_TIMER_PPI];
// ISR control block of OS driver
typedef struct
{
    x_os_drv_isr_fct    pfIsr;
    void*               pvStack;
    UINT16              u2Vector;
} OS_DRV_ISR_T;

extern OS_DRV_ISR_T	 _arIsrList[MAX_IRQ_VECTOR];
void timer_interrupt_handler(void)
{
    UINT32 u32StTimer;
	UINT32 u32TimerID;
	_arTimer.pTimerCb( u32StTimer, u32TimerID);
	return;

} 

 void GIC_Uboot_EnableIrq(UINT32 u4Vector)
{
	UINT32 u4Mask = 1 << (u4Vector % 32);
	GICD_WRITE32(GIC_DIST_ENABLE_SET + (u4Vector / 32) * 4, u4Mask);
}

 void GIC_Uboot_DisableIrq(UINT32 u4Vector)
{
	UINT32 u4Mask = 1 << (u4Vector % 32);

	GICD_WRITE32(GIC_DIST_ENABLE_CLEAR + (u4Vector / 32) * 4, u4Mask);
}

 void GIC_Uboot_ClearIrq(UINT32 u4Vector)
{
	GICC_WRITE32(GIC_CPU_EOI, u4Vector);
}

 BOOL GIC_Uboot_IsIrqEnabled(UINT32 u4Vector)
{
	UINT32 u4Mask = 1 << (u4Vector % 32);

    return ((GICD_READ32(GIC_DIST_ENABLE_SET + (u4Vector / 32) * 4) & u4Mask) != 0)?TRUE:FALSE;
}
u32 MsOS_CreateTimer (TimerCb    pTimerCb,
                         unsigned int     u32FirstTimeMs,
                         unsigned int     u32PeriodTimeMs,
                         unsigned int    bStartTimer,
                         char       *pName)
{
	unsigned long ctrl;
	UINT32 val;
	ctrl = arch_timer_reg_read(ARCH_TIMER_PHYS_ACCESS, ARCH_TIMER_REG_CTRL);
	if(bStartTimer)
	{
	ctrl |= ARCH_TIMER_CTRL_ENABLE;
	ctrl &= ~ARCH_TIMER_CTRL_IT_MASK;
	}
	else
		{
	ctrl &= ~ARCH_TIMER_CTRL_ENABLE;
	ctrl != ARCH_TIMER_CTRL_IT_MASK;
		}
    val = u32FirstTimeMs*6000;
	arch_timer_reg_write(ARCH_TIMER_PHYS_ACCESS, ARCH_TIMER_REG_TVAL, val);
	arch_timer_reg_write(ARCH_TIMER_PHYS_ACCESS, ARCH_TIMER_REG_CTRL, ctrl);
	GIC_Uboot_EnableIrq(PHYS_NONSECURE_PPI);
	_arIsrList[PHYS_NONSECURE_PPI].pfIsr = pTimerCb;
}
u32 MsOS_ResetTimer (unsigned int  u32FirstTimeMs)
{
	unsigned long ctrl;
	UINT32 val;
	ctrl = arch_timer_reg_read(ARCH_TIMER_PHYS_ACCESS, ARCH_TIMER_REG_CTRL);
	ctrl |= ARCH_TIMER_CTRL_ENABLE;
	ctrl &= ~ARCH_TIMER_CTRL_IT_MASK;
    val = u32FirstTimeMs*6000;
	arch_timer_reg_write(ARCH_TIMER_PHYS_ACCESS, ARCH_TIMER_REG_TVAL, val);
	arch_timer_reg_write(ARCH_TIMER_PHYS_ACCESS, ARCH_TIMER_REG_CTRL, ctrl);
	GIC_Uboot_EnableIrq(PHYS_NONSECURE_PPI);
}


u32 MsOS_CreateTimer_core (TimerCb    pTimerCb,
                         unsigned int     u32FirstTimeMs,
                         unsigned int     u32PeriodTimeMs,
                         unsigned int    bStartTimer,
                         char       *pName,                    
                         UINT32	   u32CoreId)
{
	unsigned long ctrl;
	UINT32 val;
	ctrl = arch_timer_reg_read(ARCH_TIMER_PHYS_ACCESS, ARCH_TIMER_REG_CTRL);
	if(bStartTimer)
	{
	ctrl |= ARCH_TIMER_CTRL_ENABLE;
	ctrl &= ~ARCH_TIMER_CTRL_IT_MASK;
	}
	else
		{
	ctrl &= ~ARCH_TIMER_CTRL_ENABLE;
	ctrl != ARCH_TIMER_CTRL_IT_MASK;
		}
	val = u32FirstTimeMs*6000;
	arch_timer_reg_write(ARCH_TIMER_PHYS_ACCESS, ARCH_TIMER_REG_TVAL, val);
	arch_timer_reg_write(ARCH_TIMER_PHYS_ACCESS, ARCH_TIMER_REG_CTRL, ctrl);
	GIC_Uboot_EnableIrq(PHYS_NONSECURE_PPI);
	_arIsrList[PHYS_NONSECURE_PPI].pfIsr = pTimerCb;
}
 void mhal_ipi_attach(unsigned int ipi_num, x_os_drv_isr_fct   pf_isr,
                 x_os_drv_isr_fct   *ppf_old_isr)
{
	x_uboot_reg_isr(ipi_num,pf_isr,&ppf_old_isr);
    return;

}

void mhal_ipi_send(unsigned int cpu_mask,unsigned int ipi_num)
{
    int val;
	val = cpu_mask<<16 |ipi_num;
	GICD_WRITE32(GIC_DIST_SOFTINT,val);

}
void mhal_ipi_send_target(unsigned int cpu_id,unsigned int ipi_num)
{
    int val,cpu_mask=0;
	cpu_mask = 1 << cpu_id;
	val = cpu_mask<<16 |ipi_num;
	GICD_WRITE32(GIC_DIST_SOFTINT,val);

}

void mhal_ipi_boradcast(unsigned int ipi_num)
{
    int val,cpu_mask=0;
	cpu_mask = 1 << 24;
	val = cpu_mask |ipi_num;
	GICD_WRITE32(GIC_DIST_SOFTINT,val);

}

void mhal_ipi_send_self(unsigned int ipi_num)
{
    int val,cpu_mask=0;
	cpu_mask = 1 << 25;
	val = cpu_mask |ipi_num;
	GICD_WRITE32(GIC_DIST_SOFTINT,val);

}

unsigned int Interrupt_enabled(void)
{
    unsigned int val;
    asm("mrs %0, cpsr" : "=r" (val) : : "cc");
    val = (val & 0x80) >> 7;

    return val?0:1;
}
void Interrupt_enable(void)
{
    unsigned int val;
    asm("mrs %0, cpsr" : "=r" (val): : "cc");
    val = val & ~0x80;
    asm("msr cpsr, %0" ::"r" (val) : "cc");
}
void Interrupt_disable(void)
{
    unsigned int val;
    asm("mrs %0, cpsr" : "=r" (val) : : "cc");
    val = val | 0x80;
    asm("msr cpsr, %0" :: "r" (val) : "cc");
}
unsigned int Interrupt_save(void)
{
    unsigned int val;
    asm("mrs %0, cpsr" : "=r" (val) : : "cc");
    return val;
}
void Interrupt_restore(unsigned int cpsr)
{
    unsigned int val=cpsr;
    asm("msr cpsr, %0" :: "r" (val) : "cc");
}

void Interrupt_active(InterruptNum irq_num)
{
   GIC_Uboot_EnableIrq(irq_num);
}

void Interrupt_deactive(InterruptNum irq_num)
{
	GIC_Uboot_DisableIrq(irq_num);
}

void Interrupt_request(InterruptNum irq_num, void* func, void * func_arg)
{
if(irq_num <= 15)
	mhal_ipi_attach(irq_num,func,0);
else
	x_uboot_reg_isr(irq_num,func,0);
}

void Interrupt_free(InterruptNum irq_num)
{
  
    GIC_Uboot_ClearIrq(irq_num);
}

void ipi_send(unsigned int cpu_mask, InterruptNum ipi_num)
{
if(ipi_num<=15)
    mhal_ipi_send(cpu_mask,ipi_num);
else
    printf("wrong ipi number %d please refer to msIRQ.h\n",ipi_num);
}

void ipi_broadcast(InterruptNum ipi_num)
{
	if(ipi_num<=15)
		mhal_ipi_boradcast(ipi_num);
	else
		printf("wrong ipi number %d please refer to msIRQ.h\n",ipi_num);

}

void ipi_send_target(unsigned int cpu_id, InterruptNum ipi_num)
{
	if(ipi_num<=15)
    mhal_ipi_send_target(cpu_id,ipi_num);
    else
    printf("wrong ipi number %d please refer to msIRQ.h\n",ipi_num);
	return ;
}

void ipi_send_self(InterruptNum ipi_num)
{
	if(ipi_num<=15)
		mhal_ipi_send_self(ipi_num);
	else
		printf("wrong ipi number %d please refer to msIRQ.h\n",ipi_num);
	return ;

}
