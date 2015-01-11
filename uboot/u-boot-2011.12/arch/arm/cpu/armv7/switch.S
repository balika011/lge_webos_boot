#define ENTRY(name) .globl name; .align 4; name:
#define END(name)   .size name, .-name
#define UNLOCK 0
#define LOCK 1

ENTRY(arch_spin_lock)
	mov		r2, #(LOCK)
spin:
	ldrex	r1, [r0]			// see what state the lock is in
	teq		r1, #(UNLOCK)		// locked already?
	wfene						// wait if it's locked
	strexeq	r1, r2, [r0]		// try to lock it, if it's unlocked
	teqeq	r1, #0x0			// did we fail?
	bne		spin				// if any failure, loop
	dmb							// Required before accessing protected resource
	bx		lr
END(arch_spin_lock)


ENTRY(arch_try_lock)
	mov 	r2, #(LOCK)
	ldrex	r1, [r0]			// see what state the lock is in
	teq 	r1, #(UNLOCK) 		// locked already?
	strexeq r1, r2, [r0]		// try to lock it, if it's unlocked
	teqeq	r1, #0x0			// did we fail?
	movne	r0, #0x0
	bne		end
	mov 	r0, #0x1			// put success status of the lock in r0 for function return
	dmb
end:
	bx		lr
END(arch_try_lock)


ENTRY(arch_spin_unlock)
	mov 	r1, #(UNLOCK)
	dmb							// Ensure accesses to protected resource have completed
	str 	r1, [r0]			// release spin lock
	dsb							// Ensure update of the mutex occurs before other CPUs wake
	sev 						// unlock any spinlock
	bx		lr
END(arch_spin_unlock)

.global __switch_context

__switch_context:
	teq 	r0, #0
	beq 	_next

	/* Store current thread context */
	stmia	r0!, {sp, lr}
	mov		r2, lr
	mrs		r3, cpsr
	stmia	r0!, {r2-r3}
	add		r0, r0, #(4*4)		/* skip r0 - r3 */
	stmia	r0!, {r4-r12}

_next:
	clrex						/*clear local exclusive monitor */
	ldr		sp, [r1], #(4*1)	/* restore sp */
	ldmia	r1!, {r2-r4}		/* load lr, pc, spsr */
	stmfd	sp!, {r2-r3}		/* store lr, pc */
	msr 	spsr, r4
	mov		lr, r1
	ldmia	lr, {r0-r12}		/* restore r0 - r12 */
	ldmfd	sp!, {lr, pc}

	/* ldmfd	sp!, {lr, pc}^ */
	/*	ldr		lr,[sp] */
	/*	add		sp,sp,#4	*/
	/*	ldr		pc,[sp]	*/
