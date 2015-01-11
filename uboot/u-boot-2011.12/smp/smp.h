#ifndef __SMP_H
#define __SMP_H

#include "MTIRQ.h"

/* Interrupt api */
extern unsigned int Interrupt_enabled(void);
extern void Interrupt_enable(void);
extern void Interrupt_disable(void);
extern unsigned int Interrupt_save(void);
extern void Interrupt_restore(unsigned int cpsr);
extern void Interrupt_active(InterruptNum irq_num);
extern void Interrupt_deactive(InterruptNum irq_num);
extern void Interrupt_request(InterruptNum irq_num, void* func, void * func_arg);
extern void Interrupt_free(InterruptNum irq_num);

/* IPI functions */
extern void ipi_send(unsigned int cpu_mask, InterruptNum ipi_num);
extern void ipi_broadcast(InterruptNum ipi_num);
extern void ipi_send_target(unsigned int cpu_id, InterruptNum ipi_num);
extern void ipi_send_self(InterruptNum ipi_num);
#endif

