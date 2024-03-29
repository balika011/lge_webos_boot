/*
 * Copyright (C) 2011-2012
 * luis.chen, MediaTek Inc <luis.chen@mediatek.com>

 * (C) Copyright 2004 Texas Insturments
 *
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 *
 * (C) Copyright 2002
 * Gary Jennejohn, DENX Software Engineering, <gj@denx.de>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/*
 * CPU specific code
 */

#include <common.h>
#include <command.h>

#ifdef CONFIG_USE_IRQ
DECLARE_GLOBAL_DATA_PTR;
#endif

extern void HalFlushInvalidateDCache(void);
extern void watchdog_reset(void);

/* read co-processor 15, register #1 (control register) */
static unsigned long read_p15_c1 (void)
{
	unsigned long value;
#if 0
	__asm__ __volatile__(
		"mrc	p15, 0, %0, c1, c0, 0   @ read control reg\n"
		: "=r" (value)
		:
		: "memory");
#endif
#ifdef MMU_DEBUG
	printf ("p15/c1 is = %08lx\n", value);
#endif
	return value;
}

/* write to co-processor 15, register #1 (control register) */
static void write_p15_c1 (unsigned long value)
{
#ifdef MMU_DEBUG
	printf ("write %08lx to p15/c1\n", value);
#endif
	__asm__ __volatile__(
		"mcr	p15, 0, %0, c1, c0, 0   @ write it back\n"
		:
		: "r" (value)
		: "memory");

	read_p15_c1 ();
}
void save_boot_params_default(u32 r0, u32 r1, u32 r2, u32 r3)
{
}
void save_boot_params(u32 r0, u32 r1, u32 r2, u32 r3)
	__attribute__((weak, alias("save_boot_params_default")));

static void cp_delay (void)
{
	volatile int i;

	/* copro seems to need some delay between reading and writing */
	for (i = 0; i < 100; i++);
}

/* See also ARM Ref. Man. */
#define C1_MMU		(1<<0)		/* mmu off/on */
#define C1_ALIGN	(1<<1)		/* alignment faults off/on */
#define C1_DC		(1<<2)		/* dcache off/on */
#define C1_WB		(1<<3)		/* merging write buffer on/off */
#define C1_BIG_ENDIAN	(1<<7)		/* big endian off/on */
#define C1_SYS_PROT	(1<<8)		/* system protection */
#define C1_ROM_PROT	(1<<9)		/* ROM protection */
#define C1_IC		(1<<12)		/* icache off/on */
#define C1_HIGH_VECTORS	(1<<13)		/* location of vectors: low/high addresses */
#define RESERVED_1	(0xf << 3)	/* must be 111b for R/W */

void _mmu_enable (void)
{
	ulong reg;

	reg = read_p15_c1 ();		/* get control reg. */
	cp_delay ();
	write_p15_c1 (reg | C1_MMU);
}

int cpu_init (void)
{
	/*
	 * setup up stacks if necessary
	 */
#ifdef CONFIG_USE_IRQ
	DECLARE_GLOBAL_DATA_PTR;

	IRQ_STACK_START = 0x2d300000;
	IRQ_STACK_START_IN = 0x2d300008;
	//FIQ_STACK_START = IRQ_STACK_START - CONFIG_STACKSIZE_IRQ;
#endif
	return 0;
}
#if defined(CONFIG_MULTICORES_PLATFORM)

extern volatile int LogEnable;
#endif
int cleanup_before_linux (void)
{
	/*
	 * this function is called just before we call linux
	 * it prepares the processor for linux
	 *
	 * we turn off caches etc ...
	 */

	unsigned long i;

	disable_interrupts ();

#ifdef CONFIG_LCD
	{
		extern void lcd_disable(void);
		extern void lcd_panel_disable(void);

		lcd_disable(); /* proper disable of lcd & panel */
		lcd_panel_disable();
	}
#endif

	/* flush I/D-cache */
    HalFlushInvalidateDCache();

	i = 0;
	//asm ("mcr p15, 0, %0, c7, c7, 0": :"r" (i));  /* invalidate both caches and flush btb */
	//asm ("mcr p15, 0, %0, c7, c10, 0": :"r" (i));
	asm ("mcr p15, 0, %0, c7, c10, 4": :"r" (i)); /* mem barrier to sync things */

	asm volatile("mcr p15, 0, %0, c14, c2, 1" : : "r" (0));//clean arch timer enable bit
	/* turn off I/D-cache */
    HalDisableCaches();
    HalDisableMMU();
#if defined(CONFIG_MULTICORES_PLATFORM)
		LogEnable =1;
#endif
	return(0);
}
#if 0
int do_reset (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	disable_interrupts ();
	watchdog_reset();
    //reset_cpu (0);
	/*NOTREACHED*/
	return(0);
}

void icache_enable (void)
{
	ulong reg;

	reg = read_p15_c1 ();		/* get control reg. */
	cp_delay ();
	write_p15_c1 (reg | C1_IC);
}

void icache_disable (void)
{
	ulong reg;

	reg = read_p15_c1 ();
	cp_delay ();
	write_p15_c1 (reg & ~C1_IC);
}

int icache_status (void)
{
	return(read_p15_c1 () & C1_IC) != 0;
}
#endif
