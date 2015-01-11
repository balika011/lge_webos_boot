#ifndef _SPINLOCK_H_
#define _SPINLOCK_H_

#include <asm/types.h>

typedef struct
{
	volatile u32 lock;
} spin_lock_t;

#define UNLOCK 0
#define LOCK 1

/* Current Program Status Register(CPSR) */
#define CPSR_N_FLAG				(0x01 << 31)
#define CPSR_Z_FLAG				(0x01 << 30)
#define CPSR_C_FLAG				(0x01 << 29)
#define CPSR_V_FLAG				(0x01 << 28)
#define CPSR_Q_FLAG				(0x01 << 27)
#define CPSR_IT_1_0				(0x03 << 25)
#define CPSR_JAZELLE			(0x01 << 24)
/* [23:20] RAZ/SBZP		*/
#define CPSR_GE					(0x01 << 16)
#define CPSR_IT_7_2				(0x3F << 10)
#define CPSR_E_STATE			(0x01 << 9)		/* Endianness execution state bit. 0:Little, 1:Big */
#define CPSR_A_DISABLE			(0x01 << 8)		/* Asynchronous abort disable bit */
#define CPSR_I_DISABLE			(0x01 << 7)		/* Interrupt disable bit */
#define CPSR_F_DISABLE			(0x01 << 6)		/* Fast interrupt diable bit */
#define CPSR_T_STATE			(0x01 << 5)		/* Thumb execution state bit */
#define CPSR_MODE				(0x1F << 0)		/* The current mode of the processor */

#define CPSR_MODE_USER			0x10
#define CPSR_MODE_FIQ			0x11
#define CPSR_MODE_IRQ			0x12
#define CPSR_MODE_SVC			0x13
#define CPSR_MODE_ABT			0x17
#define CPSR_MODE_UND			0x1B
#define CPSR_MODE_SYS			0x1F


#define INIT_SPIN_LOCK		{.lock = UNLOCK}

static inline void dsb(void)
{
	 asm volatile("dsb");
}
static inline void wfi(void)
{
	 asm volatile("wfi");
}
static inline void write_thread_id(unsigned int id)
{
	asm volatile("mcr p15, 0, %0, c13, c0, 4" :: "r" (id));
}

static inline unsigned int get_thread_id(void)
{
	unsigned int id;
	asm volatile("mrc p15, 0, %0, c13, c0, 4" : "=r" (id));
	return id;
}

static inline unsigned int get_current_cpu(void)
{
	unsigned int cpu;
	asm volatile("mrc p15, 0, %0, c0, c0, 5" : "=r"(cpu)::);
	cpu &= 0x0F;
	return cpu;
}

static inline unsigned int get_cpsr(void)
{
	unsigned int old;

	asm volatile (
			"   mrs     %0, cpsr\n"
			: "=r"(old) : );

	return old;
}

static inline int is_irq_on()
{
	if( get_cpsr() & CPSR_I_DISABLE ) return 0;
	return 1;
}
static inline unsigned int save_cpsr(void)
{
	unsigned int flag;
	//asm volatile ("mrs     %0, cpsr\n" "   cpsid   i\n" : "=r"(flag) : );
	 asm volatile ("mrs     %0, cpsr\n" "cpsid   i\n" : "=r"(flag) : );
	return flag;
}
static inline void restore_cpsr( unsigned int cpsr )
{
	asm volatile("msr     cpsr_c, %0\n" : : "r" (cpsr));
}
static inline void irq_enable(void)
{
	asm volatile("cpsie i");
}
static inline void irq_disable(void)
{
	asm volatile("cpsid i");
}
#define ENABLE_SMP
#ifdef ENABLE_SMP
/* Below functions should be defined in arch */
void arch_spin_lock(void *lock);
int arch_try_lock(void *lock);
void arch_spin_unlock(void *lock);

static inline void spin_lock_init(spin_lock_t *lock)
{
	lock->lock = UNLOCK;
}

static inline void spin_lock(spin_lock_t *lock)
{
	arch_spin_lock((void*)&lock->lock);
}

static inline int spin_trylock(spin_lock_t *lock)
{
	return arch_try_lock((void*)&lock->lock);
}

static inline void spin_unlock(spin_lock_t *lock)
{
	arch_spin_unlock((void*)&lock->lock);
}

#define spin_lock_irq(lock)		do{ irq_disable(); spin_lock(lock); } while(0)
#define spin_unlock_irq(lock)	do{ spin_unlock(lock); irq_enable(); } while(0)

#define spin_lock_save(lock, flags)			do{ flags = save_cpsr();spin_lock(lock); } while(0)
#define spin_unlock_restore(lock, flags)	do{ spin_unlock(lock); restore_cpsr(flags); } while(0)

#else
#define spin_lock_init(lock)	do{(void)lock;}while(0)
#define spin_lock(lock)			do{(void)lock;}while(0)
#define spin_trylock(lock)		(1)
#define spin_unlock(lock)		do{(void)lock;}while(0)

#define spin_lock_irq(lock)		do{irq_disable(); (void)lock;}while(0)
#define spin_unlock_irq(lock)	do{(void)lock; irq_enable();}while(0)
#define spin_lock_save(lock, flags)			do{flags = save_cpsr();(void)lock;}while(0)
#define spin_unlock_restore(lock, flags)	do{(void)lock; restore_cpsr(flags);}while(0)
#endif
#endif  //_SPINLOCK_H_

