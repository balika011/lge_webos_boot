/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2010
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/
 
#ifndef _MSDC_H_
#define _MSDC_H_

#include "types.h"
#include "x_assert.h"

extern unsigned int msdc_base;

/*--------------------------------------------------------------------------*/
/* Common Macro                                                             */
/*--------------------------------------------------------------------------*/
/* cause 'base' is common for the name of coding, so modify to msdc_base */
#define REG_ADDR(x)                 ((volatile uint32*)(msdc_base + OFFSET_##x))

/*--------------------------------------------------------------------------*/
/* Common Definition                                                        */
/*--------------------------------------------------------------------------*/
#define MSDC_FIFO_SZ            (128)
#define MSDC_FIFO_THD           (64)

#define MSDC_MS                 (0)
#define MSDC_SDMMC              (1)

#define MSDC_MODE_UNKNOWN       (0)
#define MSDC_MODE_PIO           (1)
#define MSDC_MODE_DMA_BASIC     (2)
#define MSDC_MODE_DMA_DESC      (3)
#define MSDC_MODE_DMA_ENHANCED  (4)
#define MSDC_MODE_MMC_STREAM    (5)

#define MSDC_BUS_1BITS          (0)
#define MSDC_BUS_4BITS          (1)
#define MSDC_BUS_8BITS          (2)

#define MSDC_BRUST_8B           (3)
#define MSDC_BRUST_16B          (4)
#define MSDC_BRUST_32B          (5)
#define MSDC_BRUST_64B          (6)

#define MSDC_PIN_PULL_NONE      (0)
#define MSDC_PIN_PULL_DOWN      (1)
#define MSDC_PIN_PULL_UP        (2)
#define MSDC_PIN_KEEP           (3)

/* FIXME. E1 doesn't support SDR104 (Need ECO) so we limit the max. card clock 
 * to 100MHz, which only allows card in DDR50, SDR50/25/12, HS, ST speed mode.
 */
#define MSDC_OP_SCLK            (200000000)
#define MSDC_MAX_SCLK           (800000000)
#define MSDC_MIN_SCLK           (260000)

#define MSDC_350K_SCLK          (350000)
#define MSDC_400K_SCLK          (400000)
#define MSDC_25M_SCLK           (25000000)
#define MSDC_26M_SCLK           (26000000)
#define MSDC_50M_SCLK           (50000000)
#define MSDC_52M_SCLK           (52000000)
#define MSDC_100M_SCLK			(100000000)
#define MSDC_179M_SCLK			(179000000)
#define MSDC_200M_SCLK			(200000000)
#define MSDC_208M_SCLK			(208000000)
#define MSDC_400M_SCLK			(400000000)
#define MSDC_800M_SCLK			(800000000)

#define MSDC_AUTOCMD12          (0x0001)
#define MSDC_AUTOCMD23          (0x0002)
#define MSDC_AUTOCMD19          (0x0003)

#define TYPE_CMD_RESP_EDGE      (0)
#define TYPE_WRITE_CRC_EDGE     (1)
#define TYPE_READ_DATA_EDGE  (2)
#define TYPE_WRITE_DATA_EDGE (3)

#define START_AT_RISING_EDGE  (0)
#define START_AT_FALLING_EDGE  (1)
#define START_AT_RISING_AND_FALLING_EDGE   (2)
#define START_AT_RISING_OR_FALLING_EDGE    (3)


/*--------------------------------------------------------------------------*/
/* Register Offset                                                          */
/*--------------------------------------------------------------------------*/
#define OFFSET_MSDC_CFG         (0x0)
#define OFFSET_MSDC_IOCON       (0x04)
#define OFFSET_MSDC_PS          (0x08)
#define OFFSET_MSDC_INT         (0x0c)
#define OFFSET_MSDC_INTEN       (0x10)
#define OFFSET_MSDC_FIFOCS      (0x14)
#define OFFSET_MSDC_TXDATA      (0x18)
#define OFFSET_MSDC_RXDATA      (0x1c)
#define OFFSET_SDC_CFG          (0x30)
#define OFFSET_SDC_CMD          (0x34)
#define OFFSET_SDC_ARG          (0x38)
#define OFFSET_SDC_STS          (0x3c)
#define OFFSET_SDC_RESP0        (0x40)
#define OFFSET_SDC_RESP1        (0x44)
#define OFFSET_SDC_RESP2        (0x48)
#define OFFSET_SDC_RESP3        (0x4c)
#define OFFSET_SDC_BLK_NUM      (0x50)
#define OFFSET_SDC_CSTS         (0x58)
#define OFFSET_SDC_CSTS_EN      (0x5c)
#define OFFSET_SDC_DCRC_STS     (0x60)
#define OFFSET_EMMC_CFG0        (0x70)
#define OFFSET_EMMC_CFG1        (0x74)
#define OFFSET_EMMC_STS         (0x78)
#define OFFSET_EMMC_IOCON       (0x7c)
#define OFFSET_SDC_ACMD_RESP    (0x80)
#define OFFSET_SDC_ACMD19_TRG   (0x84)
#define OFFSET_SDC_ACMD19_STS   (0x88)
#define OFFSET_MSDC_DMA_SA      (0x90)
#define OFFSET_MSDC_DMA_CA      (0x94)
#define OFFSET_MSDC_DMA_CTRL    (0x98)
#define OFFSET_MSDC_DMA_CFG     (0x9c)
#define OFFSET_MSDC_DBG_SEL     (0xa0)
#define OFFSET_MSDC_DBG_OUT     (0xa4)
#define OFFSET_MSDC_PATCH_BIT0  (0xb0)
#define OFFSET_MSDC_PATCH_BIT1  (0xb4)
#define OFFSET_MSDC_PAD_CTL0    (0xe0)
#define OFFSET_MSDC_PAD_CTL1    (0xe4)
#define OFFSET_MSDC_PAD_CTL2    (0xe8)
#define OFFSET_MSDC_PAD_TUNE    (0xec)
#define OFFSET_MSDC_DAT_RDDLY0  (0xf0)
#define OFFSET_MSDC_DAT_RDDLY1  (0xf4)
#define OFFSET_MSDC_HW_DBG      (0xf8)
#define OFFSET_MSDC_VERSION     (0x100)
#define OFFSET_MSDC_ECO_VER     (0x104)
#define OFFSET_EMMC50_PAD_CTL0           (0x180)
#define OFFSET_EMMC50_PAD_DS_CTL0        (0x184)
#define OFFSET_EMMC50_PAD_DS_TUNE        (0x188)
#define OFFSET_EMMC50_PAD_CMD_TUNE       (0x18c)
#define OFFSET_EMMC50_PAD_DAT01_TUNE     (0x190)
#define OFFSET_EMMC50_PAD_DAT23_TUNE     (0x194)
#define OFFSET_EMMC50_PAD_DAT45_TUNE     (0x198)
#define OFFSET_EMMC50_PAD_DAT67_TUNE     (0x19c)
#define OFFSET_EMMC50_CFG0               (0x208)
#define OFFSET_EMMC50_CFG1               (0x20c)
#define OFFSET_EMMC50_CFG2               (0x21c)
#define OFFSET_EMMC50_CFG3               (0x220)
#define OFFSET_EMMC50_CFG4               (0x224)

/*--------------------------------------------------------------------------*/
/* Register Address                                                         */
/*--------------------------------------------------------------------------*/

//
// Pad control register definition.
//
#define GPIO_BASE (0x10005000)
#define MSDC0_GPIO_CLK_BASE                   (GPIO_BASE + 0xC00)
#define MSDC0_GPIO_CMD_BASE                   (GPIO_BASE + 0xC10)
#define MSDC0_GPIO_DAT_BASE                   (GPIO_BASE + 0xC20)
#define MSDC0_GPIO_PAD_BASE                   (GPIO_BASE + 0xC30)
#define MSDC0_GPIO_RST_BASE                   (GPIO_BASE + 0xD00)
#define MSDC0_GPIO_DS_BASE                    (GPIO_BASE + 0xD10)
#define MSDC0_GPIO_MODE0_BASE                 (GPIO_BASE + 0x7E0)
#define MSDC0_GPIO_MODE1_BASE                 (GPIO_BASE + 0x7F0)
#define MSDC0_GPIO_MODE2_BASE                 (GPIO_BASE + 0x800)
#define MSDC0_GPIO_MODE3_BASE                 (GPIO_BASE + 0x810)

#define MSDC1_GPIO_CLK_BASE                   (GPIO_BASE + 0xC40)
#define MSDC1_GPIO_CMD_BASE                   (GPIO_BASE + 0xC50)
#define MSDC1_GPIO_DAT_BASE                   (GPIO_BASE + 0xC60)
#define MSDC1_GPIO_PAD_BASE                   (GPIO_BASE + 0xC70)
#define MSDC1_GPIO_DATPUPD_BASE               (GPIO_BASE + 0xD20)

#define MSDC2_GPIO_CLK_BASE                   (GPIO_BASE + 0xC80)
#define MSDC2_GPIO_CMD_BASE                   (GPIO_BASE + 0xC90)
#define MSDC2_GPIO_DAT_BASE                   (GPIO_BASE + 0xCA0)
#define MSDC2_GPIO_PAD_BASE                   (GPIO_BASE + 0xCB0)
#define MSDC2_GPIO_DATPUPD_BASE               (GPIO_BASE + 0xD30)

#define MSDC3_GPIO_CLK_BASE                   (GPIO_BASE + 0xCC0)
#define MSDC3_GPIO_CMD_BASE                   (GPIO_BASE + 0xCD0)
#define MSDC3_GPIO_DAT_BASE                   (GPIO_BASE + 0xCE0)
#define MSDC3_GPIO_PAD_BASE                   (GPIO_BASE + 0xCF0)
#define MSDC3_GPIO_DATPUPD_BASE               (GPIO_BASE + 0xD40)

#define GPIO_PAD_TDSEL_MASK                   (0xFUL  <<  0) 
#define GPIO_PAD_RDSEL_MASK                   (0x3FUL <<  4)
#define GPIO_PAD_BIAS_MASK                    (0xFUL  << 12)

#define GPIO_MSDC_R1R0_MASK                   (0x3UL  <<  0)
#define GPIO_MSDC_PUPD_MASK                   (0x1UL  <<  2)
#define GPIO_MSDC_DRV_MASK                    (0x7UL  <<  8)
#define GPIO_MSDC_SR_MASK                     (0x1UL  << 12)
#define GPIO_MSDC_SMT_MASK                    (0x1UL  << 13)
#define GPIO_MSDC_IES_MASK                    (0x1UL  << 14)

#define GPIO_MSDC_DAT0_R1R0_MASK              (0x3UL  <<  0)
#define GPIO_MSDC_DAT0_PUPD_MASK              (0x1UL  <<  2)
#define GPIO_MSDC_DAT1_R1R0_MASK              (0x3UL  <<  4)
#define GPIO_MSDC_DAT1_PUPD_MASK              (0x1UL  <<  6)
#define GPIO_MSDC_DAT2_R1R0_MASK              (0x3UL  <<  8)
#define GPIO_MSDC_DAT2_PUPD_MASK              (0x1UL  << 10)
#define GPIO_MSDC_DAT3_R1R0_MASK              (0x3UL  << 12)
#define GPIO_MSDC_DAT3_PUPD_MASK              (0x1UL  << 14)

/* add pull down/up mode define */
#define MSDC_GPIO_PULL_UP        (0)
#define MSDC_GPIO_PULL_DOWN      (1)

#define GPIO_CLK_CTRL       (0)
#define GPIO_CMD_CTRL       (1)
#define GPIO_DAT_CTRL       (2)
#define GPIO_RST_CTRL       (3)
#define GPIO_DS_CTRL        (4)
#define GPIO_MODE_CTRL      (5)

#define MSDC_PULL_0K        (0)
#define MSDC_PULL_10K       (1)
#define MSDC_PULL_50K       (2)
#define MSDC_PULL_8K        (3)


/* MSDC GPIO*/
#define GPIO_MSDC_MODE	1
#define GPIO_EINT_MODE  0

#define PAD_MSDC3_DAT0	GPIO189
#define PAD_MSDC3_DAT1	GPIO190
#define PAD_MSDC3_DAT2	GPIO191
#define PAD_MSDC3_DAT3	GPIO192
#define PAD_MSDC3_CLK	GPIO193
#define PAD_MSDC3_CMD	GPIO194

#define PAD_MSDC2_DAT0	GPIO28
#define PAD_MSDC2_DAT1	GPIO29
#define PAD_MSDC2_DAT2	GPIO30
#define PAD_MSDC2_DAT3	GPIO31
#define PAD_MSDC2_CLK	GPIO32
#define PAD_MSDC2_CMD	GPIO33

#define PAD_MSDC1_CMD	GPIO170
#define PAD_MSDC1_DAT0	GPIO171
#define PAD_MSDC1_DAT1	GPIO172
#define PAD_MSDC1_DAT2	GPIO173
#define PAD_MSDC1_DAT3	GPIO174
#define PAD_MSDC1_CLK	GPIO175
#define PAD_MSDC1_CD	GPIO1


#define PAD_MSDC0_DAT0	GPIO154
#define PAD_MSDC0_DAT1	GPIO155
#define PAD_MSDC0_DAT2	GPIO156
#define PAD_MSDC0_DAT3	GPIO157
#define PAD_MSDC0_DAT4	GPIO158
#define PAD_MSDC0_DAT5	GPIO159
#define PAD_MSDC0_DAT6	GPIO160
#define PAD_MSDC0_DAT7	GPIO161
#define PAD_MSDC0_CMD	GPIO162
#define PAD_MSDC0_CLK	GPIO163
#define PAD_MSDC0_DSL	GPIO164
#define PAD_MSDC0_RSTB	GPIO165

//
// PDN register offset definition.
//
#define TOP_Clk_Ctrl_base (IO_PHYS + 0x0)
#define MSDC_SW_CG                          ((volatile uint32*)(TOP_Clk_Ctrl_base + 0x0024))
#define MSDC_PDN_BIT                        (1 << 17)

/* common register */
#define MSDC_CFG                    REG_ADDR(MSDC_CFG)
#define MSDC_IOCON                  REG_ADDR(MSDC_IOCON)
#define MSDC_PS                     REG_ADDR(MSDC_PS)
#define MSDC_INT                    REG_ADDR(MSDC_INT)
#define MSDC_INTEN                  REG_ADDR(MSDC_INTEN)
#define MSDC_FIFOCS                 REG_ADDR(MSDC_FIFOCS)
#define MSDC_TXDATA                 REG_ADDR(MSDC_TXDATA)
#define MSDC_RXDATA                 REG_ADDR(MSDC_RXDATA)

/* sdmmc register */
#define SDC_CFG                     REG_ADDR(SDC_CFG)
#define SDC_CMD                     REG_ADDR(SDC_CMD)
#define SDC_ARG                     REG_ADDR(SDC_ARG)
#define SDC_STS                     REG_ADDR(SDC_STS)
#define SDC_RESP0                   REG_ADDR(SDC_RESP0)
#define SDC_RESP1                   REG_ADDR(SDC_RESP1)
#define SDC_RESP2                   REG_ADDR(SDC_RESP2)
#define SDC_RESP3                   REG_ADDR(SDC_RESP3)
#define SDC_BLK_NUM                 REG_ADDR(SDC_BLK_NUM)
#define SDC_CSTS                    REG_ADDR(SDC_CSTS)
#define SDC_CSTS_EN                 REG_ADDR(SDC_CSTS_EN)
#define SDC_DCRC_STS                REG_ADDR(SDC_DCRC_STS)

/* emmc register*/
#define EMMC_CFG0                   REG_ADDR(EMMC_CFG0)
#define EMMC_CFG1                   REG_ADDR(EMMC_CFG1)
#define EMMC_STS                    REG_ADDR(EMMC_STS)
#define EMMC_IOCON                  REG_ADDR(EMMC_IOCON)

/* auto command register */
#define SDC_ACMD_RESP               REG_ADDR(SDC_ACMD_RESP)
#define SDC_ACMD19_TRG              REG_ADDR(SDC_ACMD19_TRG)
#define SDC_ACMD19_STS              REG_ADDR(SDC_ACMD19_STS)

/* dma register */
#define MSDC_DMA_SA                 REG_ADDR(MSDC_DMA_SA)
#define MSDC_DMA_CA                 REG_ADDR(MSDC_DMA_CA)
#define MSDC_DMA_CTRL               REG_ADDR(MSDC_DMA_CTRL)
#define MSDC_DMA_CFG                REG_ADDR(MSDC_DMA_CFG)
#define MSDC_DMA_LEN                REG_ADDR(MSDC_DMA_LEN)

/* pad ctrl register */
#define MSDC_PAD_CTL0               REG_ADDR(MSDC_PAD_CTL0)
#define MSDC_PAD_CTL1               REG_ADDR(MSDC_PAD_CTL1)
#define MSDC_PAD_CTL2               REG_ADDR(MSDC_PAD_CTL2)

/* data read delay */
#define MSDC_DAT_RDDLY0             REG_ADDR(MSDC_DAT_RDDLY0)
#define MSDC_DAT_RDDLY1             REG_ADDR(MSDC_DAT_RDDLY1)

/* debug register */
#define MSDC_DBG_SEL                REG_ADDR(MSDC_DBG_SEL)
#define MSDC_DBG_OUT                REG_ADDR(MSDC_DBG_OUT)

/* misc register */
#define MSDC_PATCH_BIT0             REG_ADDR(MSDC_PATCH_BIT0)
#define MSDC_PATCH_BIT1             REG_ADDR(MSDC_PATCH_BIT1)

/* MSDC_PATCH_BIT1 mask */
#define MSDC_CMD_RSP_TA_CNTR   (0x7<<3)
#define MSDC_WRDAT_CRCS_TA_CNTR (0x7<<0)


#define DAT0_TUNE_CRC               REG_ADDR(DAT0_TUNE_CRC)
#define DAT1_TUNE_CRC               REG_ADDR(DAT1_TUNE_CRC)
#define DAT2_TUNE_CRC               REG_ADDR(DAT2_TUNE_CRC)
#define DAT3_TUNE_CRC               REG_ADDR(DAT3_TUNE_CRC)
#define CMD_TUNE_CRC                REG_ADDR(CMD_TUNE_CRC)
#define SDIO_TUNE_WIND              REG_ADDR(SDIO_TUNE_WIND)
//#define SD20_PAD_CTL0_C_TYPE        REG_ADDR(SD20_PAD_CTL0_C_TYPE)
//#define SD20_PAD_CTL0_M60           REG_ADDR(SD20_PAD_CTL0_M60)
//#define SD30_PAD_CTL0_T40           REG_ADDR(SD30_PAD_CTL0_T40)
//#define SD30_PAD_CTL0_T28_18_33V    REG_ADDR(SD30_PAD_CTL0_T28_18_33V)
//#define MSDC_PAD_CTL0                 REG_ADDR(MSDC_PAD_CTL0)
//#define SD20_PAD_CTL1_C_TYPE        REG_ADDR(SD20_PAD_CTL1_C_TYPE)
//#define SD20_PAD_CTL1_M60           REG_ADDR(SD20_PAD_CTL1_M60)
//#define SD30_PAD_CTL1_T40           REG_ADDR(SD30_PAD_CTL1_T40)
//#define SD30_PAD_CTL1_T28_18_33V    REG_ADDR(SD30_PAD_CTL1_T28_18_33V)
//#define MSDC_PAD_CTL1                 REG_ADDR(MSDC_PAD_CTL1)
//#define SD20_PAD_CTL2_C_TYPE        REG_ADDR(SD20_PAD_CTL2_C_TYPE)
//#define SD20_PAD_CTL2_M60           REG_ADDR(SD20_PAD_CTL2_M60)
//#define SD30_PAD_CTL2_T40           REG_ADDR(SD30_PAD_CTL2_T40)
//#define SD30_PAD_CTL2_T28_18_33V    REG_ADDR(SD30_PAD_CTL2_T28_18_33V)
//#define MSDC_PAD_CTL2                 REG_ADDR(MSDC_PAD_CTL2)
#define MSDC_PAD_TUNE               REG_ADDR(MSDC_PAD_TUNE)
#define MSDC_HW_DBG                 REG_ADDR(MSDC_HW_DBG)
#define MSDC_VERSION                REG_ADDR(MSDC_VERSION)
#define MSDC_ECO_VER                REG_ADDR(MSDC_ECO_VER)
/* eMMC 5.0 register */
//#define EMMC50_PAD_CTL0             REG_ADDR(EMMC50_PAD_CTL0)
//#define EMMC50_PAD_DS_CTL0          REG_ADDR(EMMC50_PAD_DS_CTL0)
#define EMMC50_PAD_DS_TUNE          REG_ADDR(EMMC50_PAD_DS_TUNE)
#define EMMC50_PAD_CMD_TUNE         REG_ADDR(EMMC50_PAD_CMD_TUNE)
#define EMMC50_PAD_DAT01_TUNE       REG_ADDR(EMMC50_PAD_DAT01_TUNE)
#define EMMC50_PAD_DAT23_TUNE       REG_ADDR(EMMC50_PAD_DAT23_TUNE)
#define EMMC50_PAD_DAT45_TUNE       REG_ADDR(EMMC50_PAD_DAT45_TUNE)
#define EMMC50_PAD_DAT67_TUNE       REG_ADDR(EMMC50_PAD_DAT67_TUNE)
#define EMMC50_CFG0                 REG_ADDR(EMMC50_CFG0)
#define EMMC50_CFG1                 REG_ADDR(EMMC50_CFG1)
#define EMMC50_CFG2                 REG_ADDR(EMMC50_CFG2)
#define EMMC50_CFG3                 REG_ADDR(EMMC50_CFG3)
#define EMMC50_CFG4                 REG_ADDR(EMMC50_CFG4)

/*--------------------------------------------------------------------------*/
/* Register Mask                                                            */
/*--------------------------------------------------------------------------*/

#define MSDC0_HOST_RESET 		(0x1 << 19)
#define MSDC1_HOST_RESET		(0x1 << 20)
#define MSDC2_HOST_RESET 		(0x1 << 17)
#define MSDC3_HOST_RESET 		(0x1 << 18)

/* MSDC_CFG mask */
#define MSDC_CFG_MODE           (0x1  << 0)     /* RW */
#define MSDC_CFG_CKPDN          (0x1  << 1)     /* RW */
#define MSDC_CFG_RST            (0x1  << 2)     /* A0 */
#define MSDC_CFG_PIO            (0x1  << 3)     /* RW */
#define MSDC_CFG_CKDRVEN        (0x1  << 4)     /* RW */
#define MSDC_CFG_BV18SDT        (0x1  << 5)     /* RW */
#define MSDC_CFG_BV18PSS        (0x1  << 6)     /* R  */
#define MSDC_CFG_CKSTB          (0x1  << 7)     /* R  */
#define MSDC_CFG_CKDIV          (0xff << 8)     /* RW */
#define MSDC_CFG_CKMOD          (0x3  << 16)    /* W1C */
#define MSDC_CFG_CKMOD_HS400    (0x1  << 18)    /* RW */
#define MSDC_CFG_START_BIT      (0x3  << 19)    /* RW */
#define MSDC_CFG_SCLK_STOP_DDR  (0x1  << 21)    /* RW */

/* MSDC_IOCON mask */
#define MSDC_IOCON_SDR104CKS    (0x1  << 0)     /* RW */
#define MSDC_IOCON_RSPL         (0x1  << 1)     /* RW */
#define MSDC_IOCON_DSPL         (0x1  << 2)     /* RW */
#define MSDC_IOCON_DDLSEL       (0x1  << 3)     /* RW */
#define MSDC_IOCON_DDR50CKD     (0x1  << 4)     /* RW */
#define MSDC_IOCON_R_D_SMPL_SEL      (0x1  << 5)     /* RW */
#define MSDC_IOCON_W_D_SMPL     (0x1  << 8)     /* RW */
#define MSDC_IOCON_W_D_SMPL_SEL (0x1  << 9)     /* RW */
#define MSDC_IOCON_W_D0SPL      (0x1  << 10)     /* RW */
#define MSDC_IOCON_W_D1SPL      (0x1  << 11)     /* RW */
#define MSDC_IOCON_W_D2SPL      (0x1  << 12)     /* RW */
#define MSDC_IOCON_W_D3SPL      (0x1  << 13)     /* RW */

#define MSDC_IOCON_R_D0SPL        (0x1  << 16)    /* RW */
#define MSDC_IOCON_R_D1SPL        (0x1  << 17)    /* RW */
#define MSDC_IOCON_R_D2SPL        (0x1  << 18)    /* RW */
#define MSDC_IOCON_R_D3SPL        (0x1  << 19)    /* RW */
#define MSDC_IOCON_R_D4SPL        (0x1  << 20)    /* RW */
#define MSDC_IOCON_R_D5SPL        (0x1  << 21)    /* RW */
#define MSDC_IOCON_R_D6SPL        (0x1  << 22)    /* RW */
#define MSDC_IOCON_R_D7SPL        (0x1  << 23)    /* RW */
#define MSDC_IOCON_RISCSZ       (0x3  << 24)    /* RW */

/* MSDC_PS mask */
#define MSDC_PS_CDEN            (0x1  << 0)     /* RW */
#define MSDC_PS_CDSTS           (0x1  << 1)     /* R  */
#define MSDC_PS_CDDEBOUNCE      (0xf  << 12)    /* RW */
#define MSDC_PS_DAT             (0xff << 16)    /* R  */
#define MSDC_PS_CMD             (0x1  << 24)    /* R  */
#define MSDC_PS_WP              (0x1UL<< 31)    /* R  */

/* MSDC_INT mask */
#define MSDC_INT_MMCIRQ         (0x1  << 0)     /* W1C */
#define MSDC_INT_CDSC           (0x1  << 1)     /* W1C */
#define MSDC_INT_ACMDRDY        (0x1  << 3)     /* W1C */
#define MSDC_INT_ACMDTMO        (0x1  << 4)     /* W1C */
#define MSDC_INT_ACMDCRCERR     (0x1  << 5)     /* W1C */
#define MSDC_INT_DMAQ_EMPTY     (0x1  << 6)     /* W1C */
#define MSDC_INT_SDIOIRQ        (0x1  << 7)     /* W1C */
#define MSDC_INT_CMDRDY         (0x1  << 8)     /* W1C */
#define MSDC_INT_CMDTMO         (0x1  << 9)     /* W1C */
#define MSDC_INT_RSPCRCERR      (0x1  << 10)    /* W1C */
#define MSDC_INT_CSTA           (0x1  << 11)    /* R */
#define MSDC_INT_XFER_COMPL     (0x1  << 12)    /* W1C */
#define MSDC_INT_DXFER_DONE     (0x1  << 13)    /* W1C */
#define MSDC_INT_DATTMO         (0x1  << 14)    /* W1C */
#define MSDC_INT_DATCRCERR      (0x1  << 15)    /* W1C */
#define MSDC_INT_ACMD19_DONE    (0x1  << 16)    /* W1C */
#define MSDC_INT_BDCSERR        (0x1  << 17)    /* W1C */
#define MSDC_INT_GPDCSERR       (0x1  << 18)    /* W1C */
#define MSDC_INT_DMAPRO         (0x1  << 19)    /* W1C */
#define MSDC_INT_GOBOUND        (0x1  << 20)    /* W1C */
#define MSDC_INT_ACMD53_DONE    (0x1  << 21)    /* W1C */
#define MSDC_INT_ACMD53_FAIL    (0x1  << 22)    /* W1C */
#define MSDC_INT_AXI_RESP_ERR   (0x1  << 23)    /* W1C */

/* MSDC_INTEN mask */
#define MSDC_INTEN_MMCIRQ       (0x1  << 0)     /* RW */
#define MSDC_INTEN_CDSC         (0x1  << 1)     /* RW */
#define MSDC_INTEN_ACMDRDY      (0x1  << 3)     /* RW */
#define MSDC_INTEN_ACMDTMO      (0x1  << 4)     /* RW */
#define MSDC_INTEN_ACMDCRCERR   (0x1  << 5)     /* RW */
#define MSDC_INTEN_DMAQ_EMPTY   (0x1  << 6)     /* RW */
#define MSDC_INTEN_SDIOIRQ      (0x1  << 7)     /* RW */
#define MSDC_INTEN_CMDRDY       (0x1  << 8)     /* RW */
#define MSDC_INTEN_CMDTMO       (0x1  << 9)     /* RW */
#define MSDC_INTEN_RSPCRCERR    (0x1  << 10)    /* RW */
#define MSDC_INTEN_CSTA         (0x1  << 11)    /* RW */
#define MSDC_INTEN_XFER_COMPL   (0x1  << 12)    /* RW */
#define MSDC_INTEN_DXFER_DONE   (0x1  << 13)    /* RW */
#define MSDC_INTEN_DATTMO       (0x1  << 14)    /* RW */
#define MSDC_INTEN_DATCRCERR    (0x1  << 15)    /* RW */
#define MSDC_INTEN_ACMD19_DONE  (0x1  << 16)    /* RW */
#define MSDC_INTEN_BDCSERR      (0x1  << 17)    /* RW */
#define MSDC_INTEN_GPDCSERR     (0x1  << 18)    /* RW */
#define MSDC_INTEN_DMAPRO       (0x1  << 19)    /* RW */
#define MSDC_INTEN_GOBOUND      (0x1  << 20)    /* RW */
#define MSDC_INTEN_ACMD53_DONE  (0x1  << 21)    /* RW */
#define MSDC_INTEN_ACMD53_FAIL  (0x1  << 22)    /* RW */
#define MSDC_INTEN_AXI_RESP_ERR   (0x1  << 23)    /* RW */

/* MSDC_FIFOCS mask */
#define MSDC_FIFOCS_RXCNT       (0xff << 0)     /* R */
#define MSDC_FIFOCS_TXCNT       (0xff << 16)    /* R */
#define MSDC_FIFOCS_CLR         (0x1UL<< 31)    /* RW */

/* SDC_CFG mask */
#define SDC_CFG_SDIOINTWKUP     (0x1  << 0)     /* RW */
#define SDC_CFG_INSWKUP         (0x1  << 1)     /* RW */
#define SDC_CFG_BUSWIDTH        (0x3  << 16)    /* RW */
#define SDC_CFG_SDIO            (0x1  << 19)    /* RW */
#define SDC_CFG_SDIOIDE         (0x1  << 20)    /* RW */      
#define SDC_CFG_INTATGAP        (0x1  << 21)    /* RW */
#define SDC_CFG_DTOC            (0xffUL << 24)  /* RW */

/* SDC_CMD mask */
#define SDC_CMD_OPC             (0x3f << 0)     /* RW */
#define SDC_CMD_BRK             (0x1  << 6)     /* RW */
#define SDC_CMD_RSPTYP          (0x7  << 7)     /* RW */
#define SDC_CMD_DTYP            (0x3  << 11)    /* RW */
#define SDC_CMD_RW              (0x1  << 13)    /* RW */
#define SDC_CMD_STOP            (0x1  << 14)    /* RW */
#define SDC_CMD_GOIRQ           (0x1  << 15)    /* RW */
#define SDC_CMD_BLKLEN          (0xfff<< 16)    /* RW */
#define SDC_CMD_AUTOCMD         (0x3  << 28)    /* RW */
#define SDC_CMD_VOLSWTH         (0x1  << 30)    /* RW */
#define SDC_CMD_ACMD53          (0x1  << 31)    /* RW */ 

/* SDC_STS mask */
#define SDC_STS_SDCBUSY         (0x1  << 0)     /* RW */
#define SDC_STS_CMDBUSY         (0x1  << 1)     /* RW */
#define SDC_STS_SWR_COMPL       (0x1UL<< 31)    /* RW */

/* SDC_DCRC_STS mask */
#define SDC_DCRC_STS_NEG        (0xff << 8)     /* RO */
#define SDC_DCRC_STS_POS        (0xff << 0)     /* RO */

/* EMMC_CFG0 mask */
#define EMMC_CFG0_BOOTSTART     (0x1  << 0)     /* W */
#define EMMC_CFG0_BOOTSTOP      (0x1  << 1)     /* W */
#define EMMC_CFG0_BOOTMODE      (0x1  << 2)     /* RW */
#define EMMC_CFG0_BOOTACKDIS    (0x1  << 3)     /* RW */
#define EMMC_CFG0_BOOTWDLY      (0x7  << 12)    /* RW */
#define EMMC_CFG0_BOOTSUPP      (0x1  << 15)    /* RW */

/* EMMC_CFG1 mask */
#define EMMC_CFG1_BOOTDATTMC    (0xfffff << 0)  /* RW */
#define EMMC_CFG1_BOOTACKTMC    (0xfffUL << 20) /* RW */

/* EMMC_STS mask */
#define EMMC_STS_BOOTCRCERR     (0x1  << 0)     /* W1C */
#define EMMC_STS_BOOTACKERR     (0x1  << 1)     /* W1C */
#define EMMC_STS_BOOTDATTMO     (0x1  << 2)     /* W1C */
#define EMMC_STS_BOOTACKTMO     (0x1  << 3)     /* W1C */
#define EMMC_STS_BOOTUPSTATE    (0x1  << 4)     /* R */
#define EMMC_STS_BOOTACKRCV     (0x1  << 5)     /* W1C */
#define EMMC_STS_BOOTDATRCV     (0x1  << 6)     /* R */

/* EMMC_IOCON mask */
#define EMMC_IOCON_BOOTRST      (0x1  << 0)     /* RW */

/* SDC_ACMD19_TRG mask */
#define SDC_ACMD19_TRG_TUNESEL  (0xf  << 0)     /* RW */

/* MSDC_DMA_CTRL mask */
#define MSDC_DMA_CTRL_START     (0x1  << 0)     /* W */
#define MSDC_DMA_CTRL_STOP      (0x1  << 1)     /* W */
#define MSDC_DMA_CTRL_RESUME    (0x1  << 2)     /* W */
#define MSDC_DMA_CTRL_MODE      (0x1  << 8)     /* RW */
#define MSDC_DMA_CTRL_LASTBUF   (0x1  << 10)    /* RW */
#define MSDC_DMA_CTRL_BRUSTSZ   (0x7  << 12)    /* RW */

/* MSDC_DMA_CFG mask */
#define MSDC_DMA_CFG_STS        (0x1  << 0)     /* R */
#define MSDC_DMA_CFG_DECSEN     (0x1  << 1)     /* RW */
#define MSDC_DMA_CFG_AHBEN      (0x3  << 8)     /* RW */
#define MSDC_DMA_CFG_ACTEN      (0x3  << 12)    /* RW */
#define MSDC_DMA_CFG_CS12B      (0x1  << 16)    /* RW */
#define MSDC_DMA_CFG_OUTB_STOP  (0x1  << 17)    /* RW */

#if 0
/* SD20_PAD_CTL0_C_TYPE mask */
#define MSDC_PAD_CTL0_C_TYPE_CLK_E4     (0x1 << 0)
#define MSDC_PAD_CTL0_C_TYPE_CLK_E8     (0x1 << 1)
#define MSDC_PAD_CTL0_C_TYPE_CLK_SR     (0x1 << 8)
#define MSDC_PAD_CTL0_C_TYPE_CLK_PUPD   (0x1 << 15)
#define MSDC_PAD_CTL0_C_TYPE_CLK_R1     (0x1 << 16)
#define MSDC_PAD_CTL0_C_TYPE_CLK_R0     (0x1 << 17)
#define MSDC_PAD_CTL0_C_TYPE_CLK_SMT    (0x1 << 18)
#define MSDC_PAD_CTL0_C_TYPE_CLK_IES    (0x1 << 19)
#define MSDC_PAD_CTL0_C_TYPE_CLK_TDSEL  (0xf << 20)
#define MSDC_PAD_CTL0_C_TYPE_CLK_RDSEL  (0x3 << 24)

/* SD20_PAD_CTL0_M60 */
#define MSDC_PAD_CTL0_M60_CLKDRV_2_0    (0x7 << 0)
#define MSDC_PAD_CTL0_M60_CLKDRV_5_3    (0x7 << 4)
#define MSDC_PAD_CTL0_M60_CLK_SR0        (0x1 << 8)
#define MSDC_PAD_CTL0_M60_CLK_SR1        (0x1 << 9)
#define MSDC_PAD_CTL0_M60_CLK_SR2        (0x1 << 10)
#define MSDC_PAD_CTL0_M60_CLK_SR3        (0x1 << 11)
#define MSDC_PAD_CTL0_M60_CLK_PUPD       (0x1 << 15)
#define MSDC_PAD_CTL0_M60_CLK_R1         (0x1 << 16)
#define MSDC_PAD_CTL0_M60_CLK_R0         (0x1 << 17)
#define MSDC_PAD_CTL0_M60_CLK_SMT        (0x1 << 18)
#define MSDC_PAD_CTL0_M60_CLK_IES        (0x1 << 19)
#define MSDC_PAD_CTL0_M60_CLK_TDSEL      (0xf << 20)
#define MSDC_PAD_CTL0_M60_CLK_RDSEL      (0xff << 24)

/* SD20_PAD_CTL0_T40 */
#define MSDC_PAD_CTL0_T40_CLKDRV_2_0    (0x7 << 0)
#define MSDC_PAD_CTL0_T40_CLKDRV_5_3    (0x7 << 4)
#define MSDC_PAD_CTL0_T40_CLK_SR0        (0x1 << 8)
#define MSDC_PAD_CTL0_T40_CLK_SR1        (0x1 << 9)
#define MSDC_PAD_CTL0_T40_CLK_SR2        (0x1 << 10)
#define MSDC_PAD_CTL0_T40_CLK_SR3        (0x1 << 11)
#define MSDC_PAD_CTL0_T40_CLK_PUPD       (0x1 << 15)
#define MSDC_PAD_CTL0_T40_CLK_R1         (0x1 << 16)
#define MSDC_PAD_CTL0_T40_CLK_R0         (0x1 << 17)
#define MSDC_PAD_CTL0_T40_CLK_SMT        (0x1 << 18)
#define MSDC_PAD_CTL0_T40_CLK_IES        (0x1 << 19)
#define MSDC_PAD_CTL0_T40_CLK_TDSEL      (0xf << 20)
#define MSDC_PAD_CTL0_T40_CLK_RDSEL      (0xff << 24)

/* SD30_PAD_CTL0_T28_18_33v */
#define MSDC_PAD_CTL0_T28_CLKDRV_E16E87E4    (0x7 << 0)
#define MSDC_PAD_CTL0_T28_CLK_SR         (0x1 << 8)
#define MSDC_PAD_CTL0_T28_CLK_PUPD       (0x1 << 15)
#define MSDC_PAD_CTL0_T28_CLK_PD         (0x1 << 16)
#define MSDC_PAD_CTL0_T28_CLK_PU         (0x1 << 17)
#define MSDC_PAD_CTL0_T28_CLK_SMT        (0x1 << 18)
#define MSDC_PAD_CTL0_T28_CLK_IES        (0x1 << 19)
#define MSDC_PAD_CTL0_T28_CLK_TDSEL      (0xf << 20)
#define MSDC_PAD_CTL0_T28_CLK_RDSEL      (0xff << 24)

/* MSDC_PAD_CTL0 */
#define MSDC_PAD_CTL0_CLKDRV_E16E87E4    (0x7 << 0)
#define MSDC_PAD_CTL0_CLK_SR         (0x1 << 8)
#define MSDC_PAD_CTL0_CLK_PUPD       (0x1 << 15)
#define MSDC_PAD_CTL0_CLK_PD         (0x1 << 16)
#define MSDC_PAD_CTL0_CLK_PU         (0x1 << 17)
#define MSDC_PAD_CTL0_CLK_SMT        (0x1 << 18)
#define MSDC_PAD_CTL0_CLK_IES        (0x1 << 19)
#define MSDC_PAD_CTL0_CLK_TDSEL      (0xf << 20)
#define MSDC_PAD_CTL0_CLK_RDSEL      (0xff << 24)

/* SD20_PAD_CTL1_C_TYPE mask */
#define MSDC_PAD_CTL1_C_TYPE_CMD_E4     (0x1 << 0)
#define MSDC_PAD_CTL1_C_TYPE_CMD_E8     (0x1 << 1)
#define MSDC_PAD_CTL1_C_TYPE_CMD_SR     (0x1 << 8)
#define MSDC_PAD_CTL1_C_TYPE_CMD_PUPD   (0x1 << 15)
#define MSDC_PAD_CTL1_C_TYPE_CMD_R1     (0x1 << 16)
#define MSDC_PAD_CTL1_C_TYPE_CMD_R0     (0x1 << 17)
#define MSDC_PAD_CTL1_C_TYPE_CMD_SMT    (0x1 << 18)
#define MSDC_PAD_CTL1_C_TYPE_CMD_IES    (0x1 << 19)
#define MSDC_PAD_CTL1_C_TYPE_CMD_TDSEL  (0xf << 20)
#define MSDC_PAD_CTL1_C_TYPE_CMD_RDSEL  (0x3 << 24)

/* SD20_PAD_CTL1_M60 */
#define MSDC_PAD_CTL1_M60_CMDDRV_2_0    (0x7 << 0)
#define MSDC_PAD_CTL1_M60_CMDDRV_5_3    (0x7 << 4)
#define MSDC_PAD_CTL1_M60_CMD_SR0        (0x1 << 8)
#define MSDC_PAD_CTL1_M60_CMD_SR1        (0x1 << 9)
#define MSDC_PAD_CTL1_M60_CMD_SR2        (0x1 << 10)
#define MSDC_PAD_CTL1_M60_CMD_SR3        (0x1 << 11)
#define MSDC_PAD_CTL1_M60_CMD_PUPD       (0x1 << 15)
#define MSDC_PAD_CTL1_M60_CMD_R1         (0x1 << 16)
#define MSDC_PAD_CTL1_M60_CMD_R0         (0x1 << 17)
#define MSDC_PAD_CTL1_M60_CMD_SMT        (0x1 << 18)
#define MSDC_PAD_CTL1_M60_CMD_IES        (0x1 << 19)
#define MSDC_PAD_CTL1_M60_CMD_TDSEL      (0xf << 20)
#define MSDC_PAD_CTL1_M60_CMD_RDSEL      (0xff << 24)

/* SD20_PAD_CTL1_T40 */
#define MSDC_PAD_CTL1_T40_CMDDRV_2_0    (0x7 << 0)
#define MSDC_PAD_CTL1_T40_CMDDRV_5_3    (0x7 << 4)
#define MSDC_PAD_CTL1_T40_CMD_SR0        (0x1 << 8)
#define MSDC_PAD_CTL1_T40_CMD_SR1        (0x1 << 9)
#define MSDC_PAD_CTL1_T40_CMD_SR2        (0x1 << 10)
#define MSDC_PAD_CTL1_T40_CMD_SR3        (0x1 << 11)
#define MSDC_PAD_CTL1_T40_CMD_PUPD       (0x1 << 15)
#define MSDC_PAD_CTL1_T40_CMD_R1         (0x1 << 16)
#define MSDC_PAD_CTL1_T40_CMD_R0         (0x1 << 17)
#define MSDC_PAD_CTL1_T40_CMD_SMT        (0x1 << 18)
#define MSDC_PAD_CTL1_T40_CMD_IES        (0x1 << 19)
#define MSDC_PAD_CTL1_T40_CMD_TDSEL      (0xf << 20)
#define MSDC_PAD_CTL1_T40_CMD_RDSEL      (0xff << 24)

/* SD30_PAD_CTL1_T28_18_33v */
#define MSDC_PAD_CTL1_T28_CMDDRV_E16E87E4    (0x7 << 0)
#define MSDC_PAD_CTL1_T28_CMD_SR         (0x1 << 8)
#define MSDC_PAD_CTL1_T28_CMD_PD         (0x1 << 16)
#define MSDC_PAD_CTL1_T28_CMD_PU         (0x1 << 17)
#define MSDC_PAD_CTL1_T28_CMD_SMT        (0x1 << 18)
#define MSDC_PAD_CTL1_T28_CMD_IES        (0x1 << 19)
#define MSDC_PAD_CTL1_T28_CMD_TDSEL      (0xf << 20)
#define MSDC_PAD_CTL1_T28_CMD_RDSEL      (0xff << 24)

/* MSDC_PAD_CTL1 */
#define MSDC_PAD_CTL1_CMDDRV_E16E87E4    (0x7 << 0)
#define MSDC_PAD_CTL1_CMD_SR         (0x1 << 8)
#define MSDC_PAD_CTL1_CMD_PD         (0x1 << 16)
#define MSDC_PAD_CTL1_CMD_PU         (0x1 << 17)
#define MSDC_PAD_CTL1_CMD_SMT        (0x1 << 18)
#define MSDC_PAD_CTL1_CMD_IES        (0x1 << 19)
#define MSDC_PAD_CTL1_CMD_TDSEL      (0xf << 20)
#define MSDC_PAD_CTL1_CMD_RDSEL      (0xff << 24)

/* SD20_PAD_CTL2_C_TYPE mask */
#define MSDC_PAD_CTL2_C_TYPE_DAT_E4     (0x1 << 0)
#define MSDC_PAD_CTL2_C_TYPE_DAT_E8     (0x1 << 1)
#define MSDC_PAD_CTL2_C_TYPE_DAT_SR     (0x1 << 8)
#define MSDC_PAD_CTL2_C_TYPE_DAT_PUPD   (0x1 << 15)
#define MSDC_PAD_CTL2_C_TYPE_DAT_R1     (0x1 << 16)
#define MSDC_PAD_CTL2_C_TYPE_DAT_R0     (0x1 << 17)
#define MSDC_PAD_CTL2_C_TYPE_DAT_SMT    (0x1 << 18)
#define MSDC_PAD_CTL2_C_TYPE_DAT_IES    (0x1 << 19)
#define MSDC_PAD_CTL2_C_TYPE_DAT_TDSEL  (0xf << 20)
#define MSDC_PAD_CTL2_C_TYPE_DAT_RDSEL  (0x3 << 24)

/* SD20_PAD_CTL2_M60 */
#define MSDC_PAD_CTL2_M60_DATDRV_2_0    (0x7 << 0)
#define MSDC_PAD_CTL2_M60_DATDRV_5_3    (0x7 << 4)
#define MSDC_PAD_CTL2_M60_DAT_SR0        (0x1 << 8)
#define MSDC_PAD_CTL2_M60_DAT_SR1        (0x1 << 9)
#define MSDC_PAD_CTL2_M60_DAT_SR2        (0x1 << 10)
#define MSDC_PAD_CTL2_M60_DAT_SR3        (0x1 << 11)
#define MSDC_PAD_CTL2_M60_DAT_PUPD       (0x1 << 15)
#define MSDC_PAD_CTL2_M60_DAT_R1         (0x1 << 16)
#define MSDC_PAD_CTL2_M60_DAT_R0         (0x1 << 17)
#define MSDC_PAD_CTL2_M60_DAT_SMT        (0x1 << 18)
#define MSDC_PAD_CTL2_M60_DAT_IES        (0x1 << 19)
#define MSDC_PAD_CTL2_M60_DAT_TDSEL      (0xf << 20)
#define MSDC_PAD_CTL2_M60_DAT_RDSEL      (0xff << 24)

/* SD20_PAD_CTL2_T40 */
#define MSDC_PAD_CTL2_T40_DATDRV_2_0    (0x7 << 0)
#define MSDC_PAD_CTL2_T40_DATDRV_5_3    (0x7 << 4)
#define MSDC_PAD_CTL2_T40_DAT_SR0        (0x1 << 8)
#define MSDC_PAD_CTL2_T40_DAT_SR1        (0x1 << 9)
#define MSDC_PAD_CTL2_T40_DAT_SR2        (0x1 << 10)
#define MSDC_PAD_CTL2_T40_DAT_SR3        (0x1 << 11)
#define MSDC_PAD_CTL2_T40_DAT_PUPD       (0x1 << 15)
#define MSDC_PAD_CTL2_T40_DAT_R1         (0x1 << 16)
#define MSDC_PAD_CTL2_T40_DAT_R0         (0x1 << 17)
#define MSDC_PAD_CTL2_T40_DAT_SMT        (0x1 << 18)
#define MSDC_PAD_CTL2_T40_DAT_IES        (0x1 << 19)
#define MSDC_PAD_CTL2_T40_DAT_TDSEL      (0xf << 20)
#define MSDC_PAD_CTL2_T40_DAT_RDSEL      (0xff << 24)

/* SD30_PAD_CTL2_T28_18_33v */
#define MSDC_PAD_CTL2_T28_DATDRV_E16E87E4    (0x7 << 0)
#define MSDC_PAD_CTL2_T28_DAT_SR         (0x1 << 8)
#define MSDC_PAD_CTL2_T28_DAT_PD         (0x1 << 16)
#define MSDC_PAD_CTL2_T28_DAT_PU         (0x1 << 17)
#define MSDC_PAD_CTL2_T28_DAT_SMT        (0x1 << 18)
#define MSDC_PAD_CTL2_T28_DAT_IES        (0x1 << 19)
#define MSDC_PAD_CTL2_T28_DAT_TDSEL      (0xf << 20)
#define MSDC_PAD_CTL2_T28_DAT_RDSEL      (0xff << 24)

/* MSDC_PAD_CTL2 */
#define MSDC_PAD_CTL2_DATDRV_E16E87E4    (0x7 << 0)
#define MSDC_PAD_CTL2_DAT_SR         (0x1 << 8)
#define MSDC_PAD_CTL2_DAT_PD         (0x1 << 16)
#define MSDC_PAD_CTL2_DAT_PU         (0x1 << 17)
#define MSDC_PAD_CTL2_DAT_SMT        (0x1 << 18)
#define MSDC_PAD_CTL2_DAT_IES        (0x1 << 19)
#define MSDC_PAD_CTL2_DAT_TDSEL      (0xf << 20)
#define MSDC_PAD_CTL2_DAT_RDSEL      (0xff << 24)
#endif 

/* MSDC_PAD_TUNE mask */
#define MSDC_PAD_TUNE_DATWRDLY  (0x1F << 0)     /* RW */
#define MSDC_PAD_TUNE_DATRRDLY  (0x1F << 8)     /* RW */
#define MSDC_PAD_TUNE_CMDRDLY   (0x1F << 16)    /* RW */
#define MSDC_PAD_TUNE_CMDRRDLY  (0x1F << 22)    /* RW */
#define MSDC_PAD_TUNE_CLKTXDLY  (0x1FUL << 27)  /* RW */

/* MSDC_DAT_RDDLY0/1 mask */
#define MSDC_DAT_RDDLY0_D3      (0x1F << 0)     /* RW */
#define MSDC_DAT_RDDLY0_D2      (0x1F << 8)     /* RW */
#define MSDC_DAT_RDDLY0_D1      (0x1F << 16)    /* RW */
#define MSDC_DAT_RDDLY0_D0      (0x1FUL<< 24)   /* RW */

#define MSDC_DAT_RDDLY1_D7      (0x1F << 0)     /* RW */
#define MSDC_DAT_RDDLY1_D6      (0x1F << 8)     /* RW */
#define MSDC_DAT_RDDLY1_D5      (0x1F << 16)    /* RW */
#define MSDC_DAT_RDDLY1_D4      (0x1FUL << 24)  /* RW */

/* MSDC_PATCH_BIT0 mask */
#define MSDC_PB0_RESV1           (0x1 << 0)
#define MSDC_PB0_EN_8BITSUP      (0x1 << 1)
#define MSDC_PB0_DIS_RECMDWR     (0x1 << 2)
#define MSDC_PB0_RESV2           (0x1 << 3)
#define MSDC_PB0_ACMD53_CRCINTR  (0x1 << 4)
#define MSDC_PB0_ACMD53_ONESHOT  (0x1 << 5)
#define MSDC_PB0_RESV3           (0x1 << 6)
#define MSDC_PB0_INT_DAT_LATCH_CK_SEL (0x7 << 7)
#define MSDC_PAD_CLK_TXDLY              		(0x1F << 27)
#define MSDC_PB0_CKGEN_MSDC_DLY_SEL   (0x1F<<10)
#define MSDC_PB0_FIFORD_DIS      (0x1 << 15)
#define MSDC_PB0_SDIO_DBSSEL     (0x1 << 16)
#define MSDC_PB0_SDIO_INTCSEL    (0x1 << 17)
#define MSDC_PB0_SDIO_BSYDLY     (0xf << 18)
#define MSDC_PB0_SDC_WDOD        (0xf << 22)
#define MSDC_PB0_CMDIDRTSEL      (0x1 << 26)
#define MSDC_PB0_CMDFAILSEL      (0x1 << 27)
#define MSDC_PB0_SDIO_INTDLYSEL  (0x1 << 28)
#define MSDC_PB0_SPCPUSH         (0x1 << 29)
#define MSDC_PB0_DETWR_CRCTMO    (0x1 << 30)
#define MSDC_PB0_EN_DRVRSP       (0x1 << 31)

/* MSDC_PATCH_BIT1 mask */
#define MSDC_PB1_WRDAT_CRCS_TA_CNTR    (0x7 << 0)
#define MSDC_PB1_CMD_RSP_TA_CNTR      (0x7 << 3)
#define MSDC_PB1_RESV3           (0x3 << 6)
#define MSDC_PB1_BIAS_TUNE_28NM  (0xf << 8)
#define MSDC_PB1_BIAS_EN18IO_28NM (0x1 << 12)
#define MSDC_PB1_BIAS_EXT_28NM   (0x1 << 13)
#define MSDC_PB1_RESV2           (0x3 << 14)
#define MSDC_PB1_RESV1           (0x7f << 16)
#define MSDC_PB1_AHBCKEN         (0x1 << 23)
#define MSDC_PB1_CKSPCEN         (0x1 << 24)
#define MSDC_PB1_CKPSCEN         (0x1 << 25)
#define MSDC_PB1_CKVOLDETEN      (0x1 << 26)
#define MSDC_PB1_CKACMDEN        (0x1 << 27)
#define MSDC_PB1_CKSDEN          (0x1 << 28)
#define MSDC_PB1_CKWCTLEN        (0x1 << 29)
#define MSDC_PB1_CKRCTLEN        (0x1 << 30)
#define MSDC_PB1_CKSHBFFEN       (0x1 << 31)

/* MSDC_HW_DBG_SEL mask */
#define MSDC_HW_DBG3_SEL         (0xff << 0)
#define MSDC_HW_DBG2_SEL         (0x3f << 8)
#define MSDC_HW_DBG1_SEL         (0x3f << 16)
#define MSDC_HW_DBG_WRAPTYPE_SEL (0x3 << 22)
#define MSDC_HW_DBG0_SEL         (0x3f << 24)
#define MSDC_HW_DBG_WRAP_SEL     (0x1  << 30)
#if 0
/* MSDC_EMMC50_PAD_CTL0 mask*/
#define MSDC_EMMC50_PAD_CTL0_DCCSEL  (0x1 << 0)
#define MSDC_EMMC50_PAD_CTL0_HLSEL   (0x1 << 1)
#define MSDC_EMMC50_PAD_CTL0_DLP0    (0x3 << 2)
#define MSDC_EMMC50_PAD_CTL0_DLN0    (0x3 << 4)
#define MSDC_EMMC50_PAD_CTL0_DLP1    (0x3 << 6)
#define MSDC_EMMC50_PAD_CTL0_DLN1    (0x3 << 8)

/* MSDC_EMMC50_PAD_DS_CTL0 mask */
#define MSDC_EMMC50_PAD_DS_CTL0_SR    (0x1 << 0)
#define MSDC_EMMC50_PAD_DS_CTL0_R0    (0x1 << 1)
#define MSDC_EMMC50_PAD_DS_CTL0_R1    (0x1 << 2)
#define MSDC_EMMC50_PAD_DS_CTL0_PUPD  (0x1 << 3)
#define MSDC_EMMC50_PAD_DS_CTL0_IES   (0x1 << 4)
#define MSDC_EMMC50_PAD_DS_CTL0_SMT   (0x1 << 5)
#define MSDC_EMMC50_PAD_DS_CTL0_RDSEL (0x3f << 6)
#define MSDC_EMMC50_PAD_DS_CTL0_TDSEL (0xf << 12)
#define MSDC_EMMC50_PAD_DS_CTL0_DRV   (0x7 << 16)
#endif 

/* EMMC50_PAD_DS_TUNE mask */
#define MSDC_EMMC50_PAD_DS_TUNE_DLYSEL  (0x1 << 0)
#define MSDC_EMMC50_PAD_DS_TUNE_DLY2SEL (0x1 << 1)
#define MSDC_EMMC50_PAD_DS_TUNE_DLY1    (0x1f << 2)
#define MSDC_EMMC50_PAD_DS_TUNE_DLY2    (0x1f << 7)
#define MSDC_EMMC50_PAD_DS_TUNE_DLY3    (0x1F << 12)

/* EMMC50_PAD_CMD_TUNE mask */
#define MSDC_EMMC50_PAD_CMD_TUNE_DLY3SEL (0x1 << 0)
#define MSDC_EMMC50_PAD_CMD_TUNE_RXDLY3  (0x1f << 1)
#define MSDC_EMMC50_PAD_CMD_TUNE_TXDLY   (0x1f << 6)

/* EMMC50_PAD_DAT01_TUNE mask */
#define MSDC_EMMC50_PAD_DAT0_RXDLY3SEL   (0x1 << 0)
#define MSDC_EMMC50_PAD_DAT0_RXDLY3      (0x1f << 1)
#define MSDC_EMMC50_PAD_DAT0_TXDLY       (0x1f << 6)
#define MSDC_EMMC50_PAD_DAT1_RXDLY3SEL   (0x1 << 16)
#define MSDC_EMMC50_PAD_DAT1_RXDLY3      (0x1f << 17)
#define MSDC_EMMC50_PAD_DAT1_TXDLY       (0x1f << 22)

/* EMMC50_PAD_DAT23_TUNE mask */
#define MSDC_EMMC50_PAD_DAT2_RXDLY3SEL   (0x1 << 0)
#define MSDC_EMMC50_PAD_DAT2_RXDLY3      (0x1f << 1)
#define MSDC_EMMC50_PAD_DAT2_TXDLY       (0x1f << 6)
#define MSDC_EMMC50_PAD_DAT3_RXDLY3SEL   (0x1 << 16)
#define MSDC_EMMC50_PAD_DAT3_RXDLY3      (0x1f << 17)
#define MSDC_EMMC50_PAD_DAT3_TXDLY       (0x1f << 22)

/* EMMC50_PAD_DAT45_TUNE mask */
#define MSDC_EMMC50_PAD_DAT4_RXDLY3SEL   (0x1 << 0)
#define MSDC_EMMC50_PAD_DAT4_RXDLY3      (0x1f << 1)
#define MSDC_EMMC50_PAD_DAT4_TXDLY       (0x1f << 6)
#define MSDC_EMMC50_PAD_DAT5_RXDLY3SEL   (0x1 << 16)
#define MSDC_EMMC50_PAD_DAT5_RXDLY3      (0x1f << 17)
#define MSDC_EMMC50_PAD_DAT5_TXDLY       (0x1f << 22)

/* EMMC50_PAD_DAT67_TUNE mask */
#define MSDC_EMMC50_PAD_DAT6_RXDLY3SEL   (0x1 << 0)
#define MSDC_EMMC50_PAD_DAT6_RXDLY3      (0x1f << 1)
#define MSDC_EMMC50_PAD_DAT6_TXDLY       (0x1f << 6)
#define MSDC_EMMC50_PAD_DAT7_RXDLY3SEL   (0x1 << 16)
#define MSDC_EMMC50_PAD_DAT7_RXDLY3      (0x1f << 17)
#define MSDC_EMMC50_PAD_DAT7_TXDLY       (0x1f << 22)

/* EMMC50_CFG0 mask */
#define MSDC_EMMC50_CFG_PADCMD_LATCHCK   (0x1 << 0)
#define MSDC_EMMC50_CFG_CRCSTS_EDGE      (0x1 << 3)
#define MSDC_EMMC50_CFG_CRCSTS_SEL       (0x1 << 4)
#define MSDC_EMMC50_CFG_ENDBIT_CHK_BIT   (0xf << 5)
#define MSDC_EMMC50_CFG_CMDRSP_SEL       (0x1 << 9)
#define MSDC_EMMC50_CFG_CMDEDGE_SEL      (0x1 <<10)
#define MSDC_EMMC50_CFG_ENDBIT_CNT       (0x3ff<<11)
#define MSDC_EMMC50_CFG_RDAT_CNT         (0x7<< 21)
#define MSDC_EMMC50_CFG_MON_SEL          (0x1 <<24)
#define MSDC_EMMC50_CFG_FIFO_WR_VALID    (0x1 <<25)
#define MSDC_EMMC50_CFG_FIFO_RD_VALID    (0x1 <<26)
#define MSDC_EMMC50_CFG_FIFO_WR_VALID_SEL (0x1 <<27)
#define MSDC_EMMC50_CFG_FIFO_RD_VALID_SEL (0x1 <<28)
#define MSDC_EMMC50_CFG_GDMA_RESET       (0x1 <<31)

/* EMMC50_CFG1 mask */
#define MSDC_EMMC50_CFG1_WRPTR_MARGIN    (0xff << 0)
#define MSDC_EMMC50_CFG1_CKSWITCH_CNT    (0x7 << 8)
#define MSDC_EMMC50_CFG1_RDDAT_STOP      (0x1 << 11)
#define MSDC_EMMC50_CFG1_WAITCLK_CNT     (0xf << 12)
#define MSDC_EMMC50_CFG1_DBG_SEL         (0xff << 16)
#define MSDC_EMMC50_CFG1_SPARE1          (0xff << 24)

/* EMMC50_CFG2_mask */
#define MSDC_EMMC50_CFG2_AXI_GPD_UP            (0x1 << 0)
#define MSDC_EMMC50_CFG2_AXI_IOMMU_WR_EMI      (0x1 << 1)
#define MSDC_EMMC50_CFG2_AXI_SHARE_EN_WR_EMI   (0x1 << 2)
#define MSDC_EMMC50_CFG2_AXI_IOMMU_RD_EMI      (0x1 << 7)
#define MSDC_EMMC50_CFG2_AXI_SHARE_EN_RD_EMI   (0x1 << 8)
#define MSDC_EMMC50_CFG2_AXI_BOUND_128B        (0x1 << 13)
#define MSDC_EMMC50_CFG2_AXI_BOUND_256B        (0x1 << 14)
#define MSDC_EMMC50_CFG2_AXI_BOUND_512B        (0x1 << 15)
#define MSDC_EMMC50_CFG2_AXI_BOUND_1K          (0x1 << 16)
#define MSDC_EMMC50_CFG2_AXI_BOUND_2K          (0x1 << 17)
#define MSDC_EMMC50_CFG2_AXI_BOUND_4K          (0x1 << 18)
#define MSDC_EMMC50_CFG2_AXI_RD_OUTS_NUM       (0x1f<< 19)
#define MSDC_EMMC50_CFG2_AXI_BURSTSZ           (0xf << 24)
#define MSDC_EMMC50_CFG2_AXI_RESP_ERR_TYPE     (0x3 << 28)
#define MSDC_EMMC50_CFG2_AXI_BUSY              (0x1 << 30)


/* EMMC50_CFG3_mask */
#define MSDC_EMMC50_CFG3_OUTS_WR               (0x1f << 0)
#define MSDC_EMMC50_CFG3_ULTRA_SET_WR          (0x3f << 5)
#define MSDC_EMMC50_CFG3_PREULTRA_SET_WR       (0x3f << 11)
#define MSDC_EMMC50_CFG3_ULTRA_SET_RD          (0x3f << 17)
#define MSDC_EMMC50_CFG3_PREULTRA_SET_RD       (0x3f <<23)

/* EMMC50_CFG4_mask */
#define MSDC_EMMC50_CFG4_IMPR_ULTRA_SET_WR     (0xff << 0)
#define MSDC_EMMC50_CFG4_IMPR_ULTRA_SET_RD     (0xff < 8)
#define MSDC_EMMC50_CFG4_ULTRA_EN              (0x3 << 16)

/* SDIO_TUNE_WIND mask*/
#define MSDC_SDIO_TUNE_WIND      (0x1f << 0)

/* MSDC_CFG[START_BIT] value */
#define START_AT_RISING    (0x0)
#define START_AT_FALLING   (0x1)
#define START_AT_RISING_AND_FALLING (0x2)
#define START_AT_RISING_OR_FALLING  (0x3)

/*--------------------------------------------------------------------------*/
/* Descriptor Structure                                                     */
/*--------------------------------------------------------------------------*/
typedef struct {
    uint32  hwo:1; /* could be changed by hw */
    uint32  bdp:1;
    uint32  rsv0:6;
    uint32  chksum:8;
    uint32  intr:1;
    uint32  rsv1:15;
    void   *next;
    void   *ptr;
    uint32  buflen:16;
    uint32  extlen:8;
    uint32  rsv2:8;
    uint32  arg;
    uint32  blknum;
    uint32  cmd;
} gpd_t;

typedef struct {
    uint32  eol:1;
    uint32  rsv0:7;
    uint32  chksum:8;
    uint32  rsv1:1;
    uint32  blkpad:1;
    uint32  dwpad:1;
    uint32  rsv2:13;
    void   *next;
    void   *ptr;
    uint32  buflen:16;
    uint32  rsv3:16;
} bd_t;

/*--------------------------------------------------------------------------*/
/* Register Debugging Structure                                             */
/*--------------------------------------------------------------------------*/

typedef struct {
    uint32 msdc:1;
    uint32 ckpwn:1;
    uint32 rst:1;
    uint32 pio:1;
    uint32 ckdrven:1;
    uint32 start18v:1;
    uint32 pass18v:1;
    uint32 ckstb:1;
    uint32 ckdiv:8;
    uint32 ckmod:2;
    uint32 pad:14;		
} msdc_cfg_reg;
typedef struct {
    uint32 sdr104cksel:1;
    uint32 rsmpl:1;
    uint32 rdsmpl:1;
    uint32 ddlysel:1;
    uint32 ddr50ckd:1;
    uint32 dsplsel:1;
    uint32 pad1:10;
    uint32 d0spl:1;
    uint32 d1spl:1;
    uint32 d2spl:1;
    uint32 d3spl:1;
    uint32 d4spl:1;
    uint32 d5spl:1;
    uint32 d6spl:1;
    uint32 d7spl:1;
    uint32 riscsz:1;
    uint32 pad2:7;
} msdc_iocon_reg;
typedef struct {
    uint32 cden:1;
    uint32 cdsts:1;
    uint32 pad1:10;
    uint32 cddebounce:4;
    uint32 dat:8;
    uint32 cmd:1;
    uint32 pad2:6;
    uint32 wp:1;
} msdc_ps_reg;
typedef struct {
    uint32 mmcirq:1;
    uint32 cdsc:1;
    uint32 pad1:1;
    uint32 atocmdrdy:1;
    uint32 atocmdtmo:1;
    uint32 atocmdcrc:1;
    uint32 dmaqempty:1;
    uint32 sdioirq:1;
    uint32 cmdrdy:1;
    uint32 cmdtmo:1;
    uint32 rspcrc:1;
    uint32 csta:1;
    uint32 xfercomp:1;
    uint32 dxferdone:1;
    uint32 dattmo:1;
    uint32 datcrc:1;
    uint32 atocmd19done:1;
    uint32 pad2:15;
} msdc_int_reg;
typedef struct {
    uint32 mmcirq:1;
    uint32 cdsc:1;
    uint32 pad1:1;
    uint32 atocmdrdy:1;
    uint32 atocmdtmo:1;
    uint32 atocmdcrc:1;
    uint32 dmaqempty:1;
    uint32 sdioirq:1;
    uint32 cmdrdy:1;
    uint32 cmdtmo:1;
    uint32 rspcrc:1;
    uint32 csta:1;
    uint32 xfercomp:1;
    uint32 dxferdone:1;
    uint32 dattmo:1;
    uint32 datcrc:1;
    uint32 atocmd19done:1;
    uint32 pad2:15;
} msdc_inten_reg;
typedef struct {
    uint32 rxcnt:8;
    uint32 pad1:8;
    uint32 txcnt:8;
    uint32 pad2:7;
    uint32 clr:1;
} msdc_fifocs_reg;
typedef struct {
    uint32 val;
} msdc_txdat_reg;
typedef struct {
    uint32 val;
} msdc_rxdat_reg;
typedef struct {
    uint32 sdiowkup:1;
    uint32 inswkup:1;
    uint32 pad1:14;
    uint32 buswidth:2;
    uint32 pad2:1;
    uint32 sdio:1;
    uint32 sdioide:1;
    uint32 intblkgap:1;
    uint32 pad4:2;
    uint32 dtoc:8;
} sdc_cfg_reg;
typedef struct {
    uint32 cmd:6;
    uint32 brk:1;
    uint32 rsptyp:3;
    uint32 pad1:1;
    uint32 dtype:2;
    uint32 rw:1;
    uint32 stop:1;
    uint32 goirq:1;    
    uint32 blklen:12;
    uint32 atocmd:2;
    uint32 volswth:1;
    uint32 pad2:1;
} sdc_cmd_reg;
typedef struct {
    uint32 arg;
} sdc_arg_reg;
typedef struct {
    uint32 sdcbusy:1;
    uint32 cmdbusy:1;
    uint32 pad:29;
    uint32 swrcmpl:1;
} sdc_sts_reg;
typedef struct {
    uint32 val;
} sdc_resp0_reg;
typedef struct {
    uint32 val;	
} sdc_resp1_reg;
typedef struct {
    uint32 val;	
} sdc_resp2_reg;
typedef struct {
    uint32 val;	
} sdc_resp3_reg;
typedef struct {
    uint32 num;	
} sdc_blknum_reg;
typedef struct {
    uint32 sts;
} sdc_csts_reg;
typedef struct {
    uint32 sts;
} sdc_cstsen_reg;
typedef struct {
    uint32 datcrcsts:8;
    uint32 ddrcrcsts:4;
    uint32 pad:20;
} sdc_datcrcsts_reg;
typedef struct {
    uint32 bootstart:1;
    uint32 bootstop:1;
    uint32 bootmode:1;
    uint32 pad1:9;
    uint32 bootwaidly:3;
    uint32 bootsupp:1;
    uint32 pad2:16;
} emmc_cfg0_reg;
typedef struct {
    uint32 bootcrctmc:16;
    uint32 pad:4;
    uint32 bootacktmc:12;
} emmc_cfg1_reg;
typedef struct {
    uint32 bootcrcerr:1;
    uint32 bootackerr:1;
    uint32 bootdattmo:1;
    uint32 bootacktmo:1;
    uint32 bootupstate:1;
    uint32 bootackrcv:1;
    uint32 bootdatrcv:1;
    uint32 pad:25;
} emmc_sts_reg;
typedef struct {
    uint32 bootrst:1;
    uint32 pad:31;
} emmc_iocon_reg;
typedef struct {
    uint32 val;
} msdc_acmd_resp_reg;
typedef struct {
    uint32 tunesel:4;
    uint32 pad:28;
} msdc_acmd19_trg_reg;
typedef struct {
    uint32 val;
} msdc_acmd19_sts_reg;
typedef struct {
    uint32 addr;
} msdc_dma_sa_reg;
typedef struct {
    uint32 addr;
} msdc_dma_ca_reg;
typedef struct {
    uint32 start:1;
    uint32 stop:1;
    uint32 resume:1;
    uint32 pad1:5;
    uint32 mode:1;
    uint32 pad2:1;
    uint32 lastbuf:1;
    uint32 pad3:1;
    uint32 brustsz:3;
    uint32 pad4:1;
    uint32 xfersz:16;
} msdc_dma_ctrl_reg;
typedef struct {
    uint32 status:1;
    uint32 decsen:1;
    uint32 pad1:2;
    uint32 bdcsen:1;
    uint32 gpdcsen:1;
    uint32 pad2:26;
} msdc_dma_cfg_reg;
typedef struct {
    uint32 sel:16;
    uint32 pad2:16;
} msdc_dbg_sel_reg;
typedef struct {
    uint32 val;
} msdc_dbg_out_reg;
typedef struct {
    uint32 clkdrvn:3;
    uint32 rsv0:1;
    uint32 clkdrvp:3;
    uint32 rsv1:1;
    uint32 clksr:1;
    uint32 rsv2:7;
    uint32 clkpd:1;    
    uint32 clkpu:1;
    uint32 clksmt:1;
    uint32 clkies:1;
    uint32 clktdsel:4;
    uint32 clkrdsel:8;
} msdc_pad_ctl0_reg;
typedef struct {
    uint32 cmddrvn:3;
    uint32 rsv0:1;    
    uint32 cmddrvp:3;
    uint32 rsv1:1;
    uint32 cmdsr:1;
    uint32 rsv2:7;
    uint32 cmdpd:1;    
    uint32 cmdpu:1;
    uint32 cmdsmt:1;
    uint32 cmdies:1;
    uint32 cmdtdsel:4;
    uint32 cmdrdsel:8;
} msdc_pad_ctl1_reg;
typedef struct {
    uint32 datdrvn:3;
    uint32 rsv0:1;
    uint32 datdrvp:3;
    uint32 rsv1:1;
    uint32 datsr:1;
    uint32 rsv2:7;
    uint32 datpd:1;    
    uint32 datpu:1;
    uint32 datsmt:1;
    uint32 daties:1;
    uint32 dattdsel:4;
    uint32 datrdsel:8;
} msdc_pad_ctl2_reg;
typedef struct {
    uint32 datwrdly:5;
    uint32 pad1:3;
    uint32 datrddly:5;    
    uint32 pad2:3;
    uint32 cmdrxdly:5;
    uint32 pad3:1;
    uint32 cmdrsprxdly:5;
    uint32 clktxdly:5;        
} msdc_pad_tune_reg;
typedef struct {
    uint32 dat0:5;
    uint32 rsv0:3;
    uint32 dat1:5;
    uint32 rsv1:3;
    uint32 dat2:5;
    uint32 rsv2:3;
    uint32 dat3:5;
    uint32 rsv3:3;    
} msdc_dat_rddly0;
typedef struct {
    uint32 dat4:5;
    uint32 rsv4:3;
    uint32 dat5:5;
    uint32 rsv5:3;
    uint32 dat6:5;
    uint32 rsv6:3;
    uint32 dat7:5;
    uint32 rsv7:3;
} msdc_dat_rddly1;
typedef struct {
    uint32 dbg0sel:8;
    uint32 dbg1sel:6;
    uint32 pad1:2;
    uint32 dbg2sel:6;
    uint32 pad2:2;
    uint32 dbg3sel:6;
    uint32 pad3:2;
} msdc_hw_dbg_reg;
typedef struct {
    uint32 val;
} msdc_version_reg;
typedef struct {
    uint32 val;
} msdc_eco_ver_reg;

struct msdc_regs {
    msdc_cfg_reg        msdc_cfg;      /* base+0x00h */
    msdc_iocon_reg      msdc_iocon;    /* base+0x04h */
    msdc_ps_reg         msdc_ps;       /* base+0x08h */
    msdc_int_reg        msdc_int;      /* base+0x0ch */
    msdc_inten_reg      msdc_inten;    /* base+0x10h */
    msdc_fifocs_reg     msdc_fifocs;   /* base+0x14h */
    msdc_txdat_reg      msdc_txdat;    /* base+0x18h */
    msdc_rxdat_reg      msdc_rxdat;    /* base+0x1ch */
    uint32              rsv1[4];
    sdc_cfg_reg         sdc_cfg;       /* base+0x30h */
    sdc_cmd_reg         sdc_cmd;       /* base+0x34h */
    sdc_arg_reg         sdc_arg;       /* base+0x38h */
    sdc_sts_reg         sdc_sts;       /* base+0x3ch */
    sdc_resp0_reg       sdc_resp0;     /* base+0x40h */
    sdc_resp1_reg       sdc_resp1;     /* base+0x44h */
    sdc_resp2_reg       sdc_resp2;     /* base+0x48h */
    sdc_resp3_reg       sdc_resp3;     /* base+0x4ch */
    sdc_blknum_reg      sdc_blknum;    /* base+0x50h */
    uint32              rsv2[1];
    sdc_csts_reg        sdc_csts;      /* base+0x58h */
    sdc_cstsen_reg      sdc_cstsen;    /* base+0x5ch */
    sdc_datcrcsts_reg   sdc_dcrcsta;   /* base+0x60h */
    uint32              rsv3[3];
    emmc_cfg0_reg       emmc_cfg0;     /* base+0x70h */
    emmc_cfg1_reg       emmc_cfg1;     /* base+0x74h */
    emmc_sts_reg        emmc_sts;      /* base+0x78h */
    emmc_iocon_reg      emmc_iocon;    /* base+0x7ch */
    msdc_acmd_resp_reg  acmd_resp;     /* base+0x80h */
    msdc_acmd19_trg_reg acmd19_trg;    /* base+0x84h */
    msdc_acmd19_sts_reg acmd19_sts;    /* base+0x88h */
    uint32              rsv4[1];
    msdc_dma_sa_reg     dma_sa;        /* base+0x90h */
    msdc_dma_ca_reg     dma_ca;        /* base+0x94h */
    msdc_dma_ctrl_reg   dma_ctrl;      /* base+0x98h */
    msdc_dma_cfg_reg    dma_cfg;       /* base+0x9ch */
    msdc_dbg_sel_reg    dbg_sel;       /* base+0xa0h */
    msdc_dbg_out_reg    dbg_out;       /* base+0xa4h */
    uint32              rsv5[2];
    uint32              patch0;        /* base+0xb0h */
    uint32              patch1;        /* base+0xb4h */
    uint32              rsv6[10];
    msdc_pad_ctl0_reg   pad_ctl0;      /* base+0xe0h */
    msdc_pad_ctl1_reg   pad_ctl1;      /* base+0xe4h */
    msdc_pad_ctl2_reg   pad_ctl2;      /* base+0xe8h */
    msdc_pad_tune_reg   pad_tune;      /* base+0xech */
    msdc_dat_rddly0     dat_rddly0;    /* base+0xf0h */
    msdc_dat_rddly1     dat_rddly1;    /* base+0xf4h */
    msdc_hw_dbg_reg     hw_dbg;        /* base+0xf8h */
    uint32              rsv7[1];       
    msdc_version_reg    version;       /* base+0x100h */
    msdc_eco_ver_reg    eco_ver;       /* base+0x104h */
};

struct scatterlist {
    u32 addr;
    u32 len;
};

struct scatterlist_ex {
    u32 cmd;
    u32 arg;
    u32 sglen;
    struct scatterlist *sg;
};

#define DMA_FLAG_NONE       (0x00000000)
#define DMA_FLAG_EN_CHKSUM  (0x00000001)
#define DMA_FLAG_PAD_BLOCK  (0x00000002)
#define DMA_FLAG_PAD_DWORD  (0x00000004)

struct dma_config {
    u32 flags;                   /* flags */
    u32 xfersz;                  /* xfer size in bytes */
    u32 sglen;                   /* size of scatter list */
    u32 blklen;                  /* block size */
    struct scatterlist *sg;      /* I/O scatter list */
    struct scatterlist_ex *esg;  /* extended I/O scatter list */
    u8  mode;                    /* dma mode        */
    u8  burstsz;                 /* burst size      */
    u8  intr;                    /* dma done interrupt */
    u8  padding;                 /* padding */
    u32 cmd;                     /* enhanced mode command */
    u32 arg;                     /* enhanced mode arg */
    u32 rsp;                     /* enhanced mode command response */
    u32 autorsp;                 /* auto command response */
    u8  inboot;                     /* this flag is for Basic DMA in eMMC boot mode*/
};

#define MSDC_USE_REG_OPS_DUMP 0

#if MSDC_USE_REG_OPS_DUMP
static void reg32_write(volatile uint32 *addr, uint32 data)
{
    *addr = (uint32)data;
    printf("[WR32] %x = %x\n", addr, data);
}

static uint32 reg32_read(volatile uint32 *addr)
{
    uint32 data = *(volatile uint32*)(addr);
    printf("[RD32] %x = %x\n", addr, data);
    return data;
}

static void reg16_write(volatile uint32 *addr, uint16 data)
{
    *(volatile uint16*)(addr) = data;
    printf("[WR16] %x = %x\n", addr, data);
}

static uint16 reg16_read(volatile uint32 *addr)
{
    uint16 data = *(volatile uint16*)addr;
    printf("[RD16] %x = %x\n", addr, data);
    return data;
}

static void reg8_write(volatile uint32 *addr, uint8 data)
{
    *(volatile uint8*)(addr) = data;
    printf("[WR8] %x = %x\n", addr, data);
}

static uint8 reg8_read(volatile uint32 *addr)
{
    uint8 data = *(volatile uint8*)addr;
    printf("[RD8] %x = %x\n", addr, data);
    return data;
}

#define MSDC_WRITE32(addr,data)		reg32_write((volatile uint32*)addr, data)
#define MSDC_READ32(addr)		    reg32_read((volatile uint32*)addr)
#define MSDC_WRITE16(addr,data)		reg16_write((volatile uint32*)addr, data)
#define MSDC_READ16(addr)		    reg16_read((volatile uint32*)addr)
#define MSDC_WRITE8(addr, data)     reg8_write((volatile uint32*)addr, data)
#define MSDC_READ8(addr)            reg8_read((volatile uint32*)addr)
#define MSDC_SET_BIT32(addr,mask)	\
    do { \
        (*(volatile uint32*)(addr) |= (mask)); \
        printf("[SET32] %x |= %x\n", addr, mask); \
    }while(0)
#define MSDC_CLR_BIT32(addr,mask)	\
    do { \
        (*(volatile uint32*)(addr) &= ~(mask)); \
        printf("[CLR32] %x &= ~%x\n", addr, mask); \
    }while(0)
#define MSDC_SET_BIT16(addr,mask)	\
    do { \
        (*(volatile uint16*)(addr) |= (mask)); \
        printf("[SET16] %x |= %x\n", addr, mask); \
    }while(0)
#define MSDC_CLR_BIT16(addr,mask)	\
    do { \
        (*(volatile uint16*)(addr) &= ~(mask)); \
        printf("[CLR16] %x &= ~%x\n", addr, mask); \
    }while(0)
#else
#define MSDC_WRITE32(addr,data)		(*(volatile uint32*)(addr) = (uint32)(data))
#define MSDC_READ32(addr)		    (*(volatile uint32*)(addr))
#define MSDC_WRITE16(addr,data)		(*(volatile uint16*)(addr) = (uint16)(data))
#define MSDC_READ16(addr)		    (*(volatile uint16*)(addr))
#define MSDC_WRITE8(addr, data)     (*(volatile uint8*)(addr)  = (uint8)(data))
#define MSDC_READ8(addr)            (*(volatile uint8*)(addr))
#define MSDC_SET_BIT32(addr,mask)	(*(volatile uint32*)(addr) |= (mask))
#define MSDC_CLR_BIT32(addr,mask)	(*(volatile uint32*)(addr) &= ~(mask))
#define MSDC_SET_BIT16(addr,mask)	(*(volatile uint16*)(addr) |= (mask))
#define MSDC_CLR_BIT16(addr,mask)	(*(volatile uint16*)(addr) &= ~(mask))
#endif

static inline unsigned int uffs(unsigned int x)
{
    unsigned int r = 1;

    if (!x)
        return 0;
    if (!(x & 0xffff)) {
        x >>= 16;
        r += 16;
    }
    if (!(x & 0xff)) {
        x >>= 8;
        r += 8;
    }
    if (!(x & 0xf)) {
        x >>= 4;
        r += 4;
    }
    if (!(x & 3)) {
        x >>= 2;
        r += 2;
    }
    if (!(x & 1)) {
        x >>= 1;
        r += 1;
    }
    return r;
}

#define MSDC_SET_FIELD(reg,field,val) \
    do {    \
        volatile uint32 tv = MSDC_READ32(reg); \
        tv &= ~(field); \
        tv |= ((val) << (uffs(field) - 1)); \
        MSDC_WRITE32(reg,tv); \
    } while(0)

#define MSDC_GET_FIELD(reg,field,val) \
    do {    \
        volatile uint32 tv = MSDC_READ32(reg); \
        val = ((tv & (field)) >> (uffs(field) - 1)); \
    } while(0)

#define MSDC_RETRY(expr,retry,cnt) \
    do { \
        uint32 t = cnt; \
        uint32 r = retry; \
        uint32 c = cnt; \
        while (r) { \
            if (!(expr)) break; \
            if (c-- == 0) { \
                r--; udelay(200); c = t; \
            } \
        } \
        if(!r) \
        	ASSERT(0); \
    } while(0)

#define MSDC_RESET() \
    do { \
        MSDC_SET_BIT32(MSDC_CFG, MSDC_CFG_RST); \
        MSDC_RETRY(MSDC_READ32(MSDC_CFG) & MSDC_CFG_RST, 5, 1000); \
    } while(0)

#define MSDC_CLR_INT() \
    do { \
        volatile uint32 val = MSDC_READ32(MSDC_INT); \
        MSDC_WRITE32(MSDC_INT, val); \
        if (MSDC_READ32(MSDC_INT)) { \
            sys_print("[ASSERT] MSDC_INT is NOT clear\n"); \
        } \
    } while(0)
        
#define MSDC_CLR_FIFO() \
    do { \
        MSDC_SET_BIT32(MSDC_FIFOCS, MSDC_FIFOCS_CLR); \
        MSDC_RETRY(MSDC_READ32(MSDC_FIFOCS) & MSDC_FIFOCS_CLR, 5, 1000); \
    } while(0)
   
#define MSDC_FIFO_WRITE32(val)  MSDC_WRITE32(MSDC_TXDATA, val)
#define MSDC_FIFO_READ32()      MSDC_READ32(MSDC_RXDATA)
#define MSDC_FIFO_WRITE16(val)  MSDC_WRITE16(MSDC_TXDATA, val)
#define MSDC_FIFO_READ16()      MSDC_READ16(MSDC_RXDATA)
#define MSDC_FIFO_WRITE8(val)   MSDC_WRITE8(MSDC_TXDATA, val)
#define MSDC_FIFO_READ8()       MSDC_READ8(MSDC_RXDATA)

#define MSDC_FIFO_WRITE(val)	MSDC_FIFO_WRITE32(val)
#define MSDC_FIFO_READ()		MSDC_FIFO_READ32()

#define MSDC_TXFIFOCNT() \
    ((MSDC_READ32(MSDC_FIFOCS) & MSDC_FIFOCS_TXCNT) >> 16)
#define MSDC_RXFIFOCNT() \
    ((MSDC_READ32(MSDC_FIFOCS) & MSDC_FIFOCS_RXCNT) >> 0)
            
#define MSDC_CARD_DETECTION_ON()  MSDC_SET_BIT32(MSDC_PS, MSDC_PS_CDEN)
#define MSDC_CARD_DETECTION_OFF() MSDC_CLR_BIT32(MSDC_PS, MSDC_PS_CDEN)	
    
#define MSDC_DMA_ON()   MSDC_CLR_BIT32(MSDC_CFG, MSDC_CFG_PIO)
#define MSDC_DMA_OFF()  MSDC_SET_BIT32(MSDC_CFG, MSDC_CFG_PIO)

#define SDC_IS_BUSY()	    (MSDC_READ32(SDC_STS) & SDC_STS_SDCBUSY)
#define SDC_IS_CMD_BUSY()	(MSDC_READ32(SDC_STS) & SDC_STS_CMDBUSY)
    
#define SDC_SEND_CMD(cmd,arg) \
    do { \
        MSDC_WRITE32(SDC_ARG, (arg)); \
        MSDC_WRITE32(SDC_CMD, (cmd)); \
    } while(0)

#define MSDC_INIT_GPD_EX(gpd,extlen,cmd,arg,blknum) \
    do { \
        ((gpd_t*)gpd)->extlen = extlen; \
        ((gpd_t*)gpd)->cmd    = cmd; \
        ((gpd_t*)gpd)->arg    = arg; \
        ((gpd_t*)gpd)->blknum = blknum; \
    }while(0)

#define MSDC_INIT_BD(bd, blkpad, dwpad, dptr, dlen) \
    do { \
        ASSERT(dlen <= 0xFFFFUL); \
        ((bd_t*)bd)->blkpad = blkpad; \
        ((bd_t*)bd)->dwpad  = dwpad; \
        ((bd_t*)bd)->ptr    = (void*)dptr; \
        ((bd_t*)bd)->buflen = dlen; \
    }while(0)

typedef enum MT65XX_POWER_VOL_TAG 
{
    VOL_DEFAULT, 
    VOL_0900 = 900,
    VOL_1000 = 1000,
    VOL_1100 = 1100,
    VOL_1200 = 1200,	
    VOL_1300 = 1300,   
    VOL_1350 = 1350,   
    VOL_1500 = 1500,    
    VOL_1800 = 1800,    
    VOL_2000 = 2000,
    VOL_2100 = 2100,
    VOL_2500 = 2500,    
    VOL_2800 = 2800, 
    VOL_3000 = 3000,
    VOL_3300 = 3300,
    VOL_3400 = 3400, 
    VOL_3500 = 3500,
    VOL_3600 = 3600        
} MT65XX_POWER_VOLTAGE;

#ifdef MMC_PROFILING
static inline void msdc_timer_init(void)
{
    /* clear. CLR[1]=1, EN[0]=0 */
    MSDC_WRITE32(GPT_BASE + 0x30, 0x0);
    MSDC_WRITE32(GPT_BASE + 0x30, 0x2);

    MSDC_WRITE32(GPT_BASE + 0x38, 0);
    MSDC_WRITE32(GPT_BASE + 0x3C, 32768);

    /* 32678HZ RTC free run */
    MSDC_WRITE32(GPT_BASE + 0x34, 0x30);
    MSDC_WRITE32(GPT_BASE + 0x30, 0x32);
}
static inline void msdc_timer_start(void)
{
    *(volatile unsigned int*)(GPT_BASE + 0x30) |= (1 << 0);
}
static inline void msdc_timer_stop(void)
{
    *(volatile unsigned int*)(GPT_BASE + 0x30) &= ~(1 << 0);
}
static inline void msdc_timer_stop_clear(void)
{
    *(volatile unsigned int*)(GPT_BASE + 0x30) &= ~(1 << 0); /* stop  */
    *(volatile unsigned int*)(GPT_BASE + 0x30) |= (1 << 1);  /* clear */
}
static inline unsigned int msdc_timer_get_count(void)
{
    return MSDC_READ32(GPT_BASE + 0x38);
}
#else
#define msdc_timer_init()       do{}while(0)
#define msdc_timer_start()      do{}while(0)
#define msdc_timer_stop()       do{}while(0)
#define msdc_timer_stop_clear() do{}while(0)
#define msdc_timer_get_count()  0
#endif


typedef struct msdc_reg_control {
    u32 addr;
    u32 mask; 
    u32 value;
    u32 default_value; 
    int (*restore_func)(int restore);
}MSDC_REG_CONTROL_T;


extern int msdc_reg_test(int id);
extern int msdc_init(int id, struct mmc_host *host, int clksrc, int mode);
extern int msdc_pio_bread(struct mmc_host *host, uchar *dst, ulong src, ulong nblks);
extern int msdc_pio_bwrite(struct mmc_host *host, ulong dst, uchar *src, ulong nblks);
extern int msdc_dma_bread(struct mmc_host *host, uchar *dst, ulong src, ulong nblks);
extern int msdc_dma_bwrite(struct mmc_host *host, ulong dst, uchar *src, ulong nblks);
extern int msdc_stream_bread(struct mmc_host *host, uchar *dst, ulong src, ulong nblks);
extern int msdc_stream_bwrite(struct mmc_host *host, ulong dst, uchar *src, ulong nblks);
extern int msdc_tune_bwrite(struct mmc_host *host, ulong dst, uchar *src, ulong nblks);
extern int msdc_tune_bread(struct mmc_host *host, uchar *dst, ulong src, ulong nblks);
extern int msdc_tune_cmdrsp(struct mmc_host *host, struct mmc_command *cmd);
extern void msdc_intr_sdio(struct mmc_host *host, int enable);
extern void msdc_intr_sdio_gap(struct mmc_host * host, int enable);
extern void msdc_config_clock(struct mmc_host *host, int ddr, u32 hz);
extern void msdc_set_startbit(struct mmc_host *host, u8 start_bit);
extern int mmc_rw_test(struct mmc_host *host, struct mmc_card *card, struct mmc_command *cmd, int dat_line);
#endif /* end of _MSDC_H_ */

