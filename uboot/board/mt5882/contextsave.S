/*
 * swsusp.S - This file is based on arch/i386/power/swsusp.S;
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 * This may not use any stack, nor any variable that is not "NoSave":
 *
 * Its rewriting one kernel image with another. What is stack in "old"
 * image could very well be data page in "new" image, and overwriting
 * your own stack under you is bad idea.
 */

    .text

#define	LOCAL_WORD(x) \
	.data			; \
	.p2align 2		; \
	.type   x, #object	; \
	.size   x, 4		; \
x:				; \
	.long 1

#define WORD_ADDR(x) \
	.align 2		; \
.L##x:				; \
	.word x

#define	FUNC(x) \
        .text			; \
	.p2align 2		; \
	.globl x		; \
	.type   x, #function	; \
x:

#define	FUNC_END(x) \
	.size	x, .-x

#define CHANGE_MODE(x) \
	mov	r1, r0		; \
	bic	r1, r1, #0x1f	; \
	orr	r1, r1, #0x##x	; \
	msr	cpsr_c, r1

/* nonvolatile int registers */
#ifdef DEBUG
	.globl	saved_context_r0	// for debug
#endif
	LOCAL_WORD(saved_context_r0)
	LOCAL_WORD(saved_context_r1)
	LOCAL_WORD(saved_context_r2)
	LOCAL_WORD(saved_context_r3)
	LOCAL_WORD(saved_context_r4)
	LOCAL_WORD(saved_context_r5)
	LOCAL_WORD(saved_context_r6)
	LOCAL_WORD(saved_context_r7)
	LOCAL_WORD(saved_context_r8)
	LOCAL_WORD(saved_context_r9)
	LOCAL_WORD(saved_context_r10)
	LOCAL_WORD(saved_context_r11)
	LOCAL_WORD(saved_context_r12)
	LOCAL_WORD(saved_context_r13)
	LOCAL_WORD(saved_context_r14)
	LOCAL_WORD(saved_cpsr)

	LOCAL_WORD(saved_context_r8_fiq)
	LOCAL_WORD(saved_context_r9_fiq)
	LOCAL_WORD(saved_context_r10_fiq)
	LOCAL_WORD(saved_context_r11_fiq)
	LOCAL_WORD(saved_context_r12_fiq)
	LOCAL_WORD(saved_context_r13_fiq)
	LOCAL_WORD(saved_context_r14_fiq)
	LOCAL_WORD(saved_spsr_fiq)

	LOCAL_WORD(saved_context_r13_irq)
	LOCAL_WORD(saved_context_r14_irq)
	LOCAL_WORD(saved_spsr_irq)
	LOCAL_WORD(saved_context_r13_svc)
	LOCAL_WORD(saved_context_r14_svc)
	LOCAL_WORD(saved_spsr_svc)

	LOCAL_WORD(saved_context_r13_abt)
	LOCAL_WORD(saved_context_r14_abt)
	LOCAL_WORD(saved_spsr_abt)

	LOCAL_WORD(saved_context_r13_und)
	LOCAL_WORD(saved_context_r14_und)
	LOCAL_WORD(saved_spsr_und)
//	LOCAL_WORD(nr_copy_pages)
//	LOCAL_WORD(pagedir_nosave)


FUNC(trustzone_arch_suspend)

	/* save current program status register */
	ldr	r3, .Lsaved_cpsr
	mrs	r1, cpsr
	str	r1, [r3]

	/* hold current mode */
	mrs	r0, cpsr

	CHANGE_MODE(1f)	/* change to system(user) mode */
	/* save nonvolatile int register */
	ldr	r3, .Lsaved_context_r0
	stmia	r3, {r0-r14}

	CHANGE_MODE(11) /* change to fiq mode */
	/* save nonvolatile int register */
	ldr	r3, .Lsaved_context_r8_fiq
	stmia	r3, {r8-r14}
	/* save spsr_fiq register */
	ldr	r3, .Lsaved_spsr_fiq
	mrs	r1, spsr
	str	r1, [r3]

	CHANGE_MODE(12) /* change to irq mode */
	/* save nonvolatile int register */
	ldr	r3, .Lsaved_context_r13_irq
	stmia	r3, {r13-r14}
	/* save spsr_irq register */
	ldr	r3, .Lsaved_spsr_irq
	mrs	r1, spsr
	str	r1, [r3]

	CHANGE_MODE(13) /* change to svc mode */
	/* save nonvolatile int register */
	ldr	r3, .Lsaved_context_r13_svc
	stmia	r3, {r13-r14}
	/* save spsr_svc register */
	ldr	r3, .Lsaved_spsr_svc
	mrs	r1, spsr
	str	r1, [r3]

	CHANGE_MODE(17) /* change to abt mode */
	/* save nonvolatile int register */
	ldr	r3, .Lsaved_context_r13_abt
	stmia	r3, {r13-r14}
	/* save spsr_abt register */
	ldr	r3, .Lsaved_spsr_abt
	mrs	r1, spsr
	str	r1, [r3]

	CHANGE_MODE(1b) /* change to und mode */
	/* save nonvolatile int register */
	ldr	r3, .Lsaved_context_r13_und
	stmia	r3, {r13-r14}
	/* save spsr_und register */
	ldr	r3, .Lsaved_spsr_und
	mrs	r1, spsr
	str	r1, [r3]

	/* go back to original mode */
	msr	cpsr_c, r0

	/* restore return address */
	ldr	r3, .Lsaved_context_r14_svc
	ldr	lr, [r3]
	mov	pc, lr

	WORD_ADDR(saved_context_r0)
	WORD_ADDR(saved_cpsr)
	WORD_ADDR(saved_context_r8_fiq)
	WORD_ADDR(saved_spsr_fiq)
	WORD_ADDR(saved_context_r13_irq)
	WORD_ADDR(saved_spsr_irq)
	WORD_ADDR(saved_context_r13_svc)
	WORD_ADDR(saved_context_r14_svc)
	WORD_ADDR(saved_spsr_svc)
	WORD_ADDR(saved_context_r13_abt)
	WORD_ADDR(saved_spsr_abt)
	WORD_ADDR(saved_context_r13_und)
	WORD_ADDR(saved_spsr_und)

FUNC_END(trustzone_arch_suspend)


FUNC(trustzone_arch_resume)
	/* hold current mode */
	mrs	r0, cpsr

	CHANGE_MODE(1f) /* change to system(user) mode */
	/* restore nonvolatile int register */
	ldr	r3, .Lsaved_context_r0
	ldmia	r3, {r0-r14}
	/* restore current program status register */
	ldr	r3, .Lsaved_cpsr
	ldr	r1, [r3]
	msr	cpsr_cxsf, r1

	CHANGE_MODE(11) /* change to fiq mode */
	/* restore nonvolatile int register */
	ldr	r3, .Lsaved_context_r8_fiq
	ldmia	r3, {r8-r14}
	/* restore spsr_fiq register */
	ldr	r3, .Lsaved_spsr_fiq
	ldr	r1, [r3]
	msr	spsr_cxsf, r1

	CHANGE_MODE(12) /* change to irq mode */
	/* restore nonvolatile int register */
	ldr	r3, .Lsaved_context_r13_irq
	ldmia	r3, {r13-r14}
	/* restore spsr_irq register */
	ldr	r3, .Lsaved_spsr_irq
	ldr	r1, [r3]
	msr	spsr_cxsf, r1

	CHANGE_MODE(13) /* change to svc mode */
	/* restore nonvolatile int register */
	ldr	r3, .Lsaved_context_r13_svc
	ldmia	r3, {r13-r14}
	/* restore spsr_svc register */
	ldr	r3, .Lsaved_spsr_svc
	ldr	r1, [r3]
	msr	spsr_cxsf, r1

	CHANGE_MODE(17) /* change to abt mode */
	/* restore nonvolatile int register */
	ldr	r3, .Lsaved_context_r13_abt
	ldmia	r3, {r13-r14}
	/* restore spsr_abt register */
	ldr	r3, .Lsaved_spsr_abt
	ldr	r1, [r3]
	msr	spsr_cxsf, r1

	CHANGE_MODE(1b) /* change to und mode */
	/* restore nonvolatile int register */
	ldr	r3, .Lsaved_context_r13_und
	ldmia	r3, {r13-r14}
	/* restore spsr_und register */
	ldr	r3, .Lsaved_spsr_und
	ldr	r1, [r3]
	msr	spsr_cxsf, r1

	/* go back to original mode */
	msr	cpsr_c, r0

	/* r0 will be the return value of swsusp_arch_suspend */
	mov	r0, #0
	/* restore return address */
	ldr	r3, .Lsaved_context_r14_svc
	ldr	lr, [r3]
	mov	pc, lr
FUNC_END(trustzone_arch_resume)
