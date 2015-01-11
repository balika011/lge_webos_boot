
#ifndef __ASMARM_SMP_PLATFORM_H
#define __ASMARM_SMP_PLATFORM_H
	#define CONFIG_KERNEL_START_ADDRESS 0x20200000
    #define SMP_RIU_BASE          0x1f000000
    #define SMP_DUMMY_BANK             0xe9e
    #define SMP_DUMMY_MAGIC           0x8000
    #define SMP_DUMMY_PA_START        0x8004
    #define SMP_DUMMY_SP              0x8008
    #define SMP_DUMMY_TTB0            0x800c
    #define SMP_DBG_CACHE_S           0xf001
    #define SMP_DBG_CACHE_E           0xf002
    #define SMP_DBG_RAM_INLOOP        0xf003
    #define SMP_DBG_GET_MAGIC_NUM     0xf004
    #define SMP_DBG_IN_SECOND_INIT    0xf005
    #define SMP_DBG_IN_STACK_READY    0xf006
    #define SMP_DBG_IN_MMU_ON         0xf007
    #define SMP_DBG_IN_C_CODE         0xf008

    #define MAGIC_NUMBER    (0x0000babe)
    #define BOOT_CPU        (0)

#endif
