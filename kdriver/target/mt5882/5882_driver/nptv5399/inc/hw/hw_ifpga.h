#ifndef _HW_IFPGA_H_
#define _HW_IFPGA_H_

//Page HISTORY
#define IO_IFPGA_BASE (IO_VIRT + 0x61000)

//Page TABLE
#define REG_IFPGA_CFG0 (IO_IFPGA_BASE + 0x500)
    #define FLD_RG_IFIBIAS Fld(2,8,AC_MSKB1)//[9:8]
    #define FLD_RG_IFPGA_TESTE Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_RG_PGATOSIF_EN Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_RG_PGAVOCMSEL Fld(2,4,AC_MSKB0)//[5:4]
    #define FLD_RG_PGAVOCM_EN Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_RG_PGAVOCM_PWD Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_RG_PGA_PWD Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_RG_PGAVIOPEN_EN Fld(1,0,AC_MSKB0)//[0:0]
#define REG_IFPGA_CFG1 (IO_IFPGA_BASE + 0x504)
    #define FLD_RG_IFTQI_MON Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_RG_IFTESTP_EN Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_RG_IFTESTN_EN Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_RG_IFTESTI Fld(2,20,AC_MSKB2)//[21:20]
    #define FLD_RG_IFTESTBUF_PWD Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_RG_IFBG_PWD Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_RG_IFPGASHORT Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_RG_IFPGA_ABIST_EN Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_RG_340U_IFPGA Fld(2,14,AC_MSKB1)//[15:14]
    #define FLD_RG_90U_IFPGA Fld(2,12,AC_MSKB1)//[13:12]
    #define FLD_RG_40U_IFPGA Fld(3,9,AC_MSKB1)//[11:9]
    #define FLD_RG_IFPGAGMSEL Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_RG_20U_IFVCO Fld(2,6,AC_MSKB0)//[7:6]
    #define FLD_RG_20U_IFTH Fld(2,4,AC_MSKB0)//[5:4]
    #define FLD_RG_10U_IFV2V Fld(2,2,AC_MSKB0)//[3:2]
    #define FLD_RG_5U_IFPGABIST Fld(2,0,AC_MSKB0)//[1:0]
#define REG_IFPGA_CFG2 (IO_IFPGA_BASE + 0x508)
    #define FLD_RG_IFPGAREV Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_RG_IFPGA_INPUT_SEL Fld(1,23,AC_MSKB2)//[23:23]

#endif /* _HW_IFPGA_H_ */

