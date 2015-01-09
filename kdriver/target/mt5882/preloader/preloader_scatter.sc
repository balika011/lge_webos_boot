RAM 0x00000000 0x10000000
{
    BOOT 0x00000000
    {
      int.o
    }

    BSP +0
    {
      startup.o startup_ca9.o arm.o chip_boot.o chip_txt.o
    }

    RO +0
    {
      * (+RO)
    }

    RW +0
    {
      * (+RW)
    }

    BSS +0
    {
      * (+ZI)
    }
    
    DRAMTAG 0x00009800 FIXED PADVALUE 0xFFFFFFFF
    {
      *(DRAMTAG)
    }

    GLOBAL 0xFB009C00
    {
      * (GLOBAL)
    }    
    

}

