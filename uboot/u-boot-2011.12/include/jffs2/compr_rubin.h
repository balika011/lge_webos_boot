/* Rubin encoder/decoder header       */
/* work started at   : aug   3, 1994  */
/* last modification : aug  15, 1994  */
/* $Id: //DTV/RELEASE_BR/r_DTV_X_IDTV1401_002306_12_001_37_001/boot/uboot/u-boot-2011.12/include/jffs2/compr_rubin.h#1 $ */

#define RUBIN_REG_SIZE   16
#define UPPER_BIT_RUBIN    (((long) 1)<<(RUBIN_REG_SIZE-1))
#define LOWER_BITS_RUBIN   ((((long) 1)<<(RUBIN_REG_SIZE-1))-1)

void dynrubin_decompress(unsigned char *data_in, unsigned char *cpage_out,
		   unsigned long sourcelen, unsigned long dstlen);
