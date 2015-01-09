
/*
 * cmd_mtdutils.c written by junorion
 * flash_eraseall, nanddump, nandwrite
 */

#include <common.h>
#include <command.h>
#include <environment.h>
#include <net.h>
#include <getopt.h>

#include <nand.h>
#include <mtdinfo.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>

#define MAX_PAGE_SIZE		2048
#define MAX_OOB_SIZE		64

extern int sscanf(char *s, const char *fmt, ...);

extern nand_info_t nand_info[];

extern ulong default_offset;

extern int tftp_get (char *filename);

//static int quiet;

static void show_progress (char * str, uint32_t cur, uint32_t tot)
{
	printf("\r%s @ %x-- %d %% complete.", str, cur, (int) (cur * 100 / tot));
}

static void dump_linebuf(char *buf, int bytes)
{
	u_char *cp;
	int i;
	
	puts ("    ");
	cp = (u_char *)buf;
	for (i=0; i<bytes; i++) {
		if ((*cp < 0x20) || (*cp > 0x7e))
			putc ('.');
		else
			printf("%c", *cp);
		cp++;
	}
}

int do_nanddump(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	struct mtd_info *mtd = &nand_info[0];
	struct mtd_partition_info *mpi = NULL;
	uint8_t datbuf[MAX_PAGE_SIZE];
	uint8_t oobbuf[MAX_OOB_SIZE];
	uint32_t dumpaddr = 0, pos, offset, length;
	int isNand, idx = 0, ret, i;
	int pagesize = mtd->oobblock;
	int oobsize = mtd->oobsize;

//	for(i=0; i<argc; i++)
//		printf("arg[%d] : %s\n", i, argv[i]);
	
	if(argc < 2) {
		printf ("Usage:\n%s\n", cmdtp->usage);
		return (-1);
	}		

	if(!(mtd->type & MTD_NANDFLASH)) {
		printf("mtd device is not nand flash type...!!\n");		
		return (-1);
	}

	// arg 2
	sscanf(argv[1], "mtd%d", &idx);
	mpi = get_mtdpartinfo(idx);
	if(!mpi) {
		printf("unknown partition : %s\n", argv[1]);
		return (-1);
	}

	switch (argc) {
		case 2:
			dumpaddr = offset = 0;
			length = mpi->size;
			break;
		case 3:
			dumpaddr = simple_strtoul(argv[2], NULL, 16);
			offset = 0;
			length = mpi->size;
			break;
		case 4:
			offset = simple_strtoul(argv[2], NULL, 16);
			length = simple_strtoul(argv[3], NULL, 16);
			dumpaddr = 0;
			break;
		case 5:
			offset = simple_strtoul(argv[2], NULL, 16);
			length = simple_strtoul(argv[3], NULL, 16);
			dumpaddr = simple_strtoul(argv[4], NULL, 16);	
		default:
			printf ("Usage:\n%s\n", cmdtp->usage);
			return (-1);			
	}

	// todo check dumpaddr

	isNand = (mtd->type & MTD_NANDFLASH) ? 1 : 0;

	for(pos = offset; pos < length; pos += pagesize) {
		loff_t from;
		size_t retlen;
		
		from = (loff_t)(CFG_FLASH_BASE + (mpi->offset + pos));
		/* read data */
		//memset(datbuf, 0xff, pagesize);
		ret = mtd->read(mtd, from, pagesize, &retlen, datbuf);
		if(ret) {
			printf("nand read failed..--;;\n");
			return ret;
		}

		if(!dumpaddr) {	// print
			for(i=0; i<pagesize; i+=0x10) {
				printf("0x%08x: %02x %02x %02x %02x %02x %02x %02x "
						"%02x %02x %02x %02x %02x %02x %02x %02x %02x",
						(unsigned int) (from + i),
						datbuf[i], datbuf[i+1], datbuf[i+2], datbuf[i+3],
						datbuf[i+4], datbuf[i+5], datbuf[i+6], datbuf[i+7],
						datbuf[i+8], datbuf[i+9], datbuf[i+10], datbuf[i+11],
						datbuf[i+12], datbuf[i+13], datbuf[i+14], datbuf[i+15]);
				dump_linebuf((char*)datbuf, 0x10);
				printf("\n");
			}
		}
		else {	// dump to memory 
			memcpy((void*)dumpaddr, datbuf, pagesize);
			dumpaddr += pagesize;
		}
		
		/* read oob */
		//memset(oobbuf, 0xff, oobsize);
		ret = mtd->read_oob(mtd, from, oobsize, &retlen, oobbuf);
		if(ret) {
			printf("nand read oob failed..--;;\n");
			return ret;
		}

		if(!dumpaddr) {	// print
			if(oobsize < 0x10) {
				printf("  OOB Data: %02x %02x %02x %02x %02x %02x %02x %02x",
						oobbuf[0], oobbuf[1], oobbuf[2], oobbuf[3], 
						oobbuf[4], oobbuf[5], oobbuf[6], oobbuf[7]);
				dump_linebuf((char*)oobbuf, 0x8);
				printf("\n");
				continue;
			}
			
			for(i=0; i<oobsize; i+=0x10) {
				printf("  OOB Data: %02x %02x %02x %02x %02x %02x %02x "
						"%02x %02x %02x %02x %02x %02x %02x %02x %02x",
						oobbuf[i], oobbuf[i+1], oobbuf[i+2], oobbuf[i+3],
						oobbuf[i+4], oobbuf[i+5], oobbuf[i+6], oobbuf[i+7],
						oobbuf[i+8], oobbuf[i+9], oobbuf[i+10], oobbuf[i+11],
						oobbuf[i+12], oobbuf[i+13], oobbuf[i+14], oobbuf[i+15]);
				dump_linebuf((char*)oobbuf, 0x10);
				printf("\n");
			}
			
		}
		else {	// dump to memory 
			memcpy((void*)dumpaddr, oobbuf, oobsize);
			dumpaddr += oobsize;
		}
	}

	mtd->sync(mtd);
	
	return 0;
}

U_BOOT_CMD(
	nanddump,	5,	0,	do_nanddump,
	"nanddump\t- dump the contents of raw NAND chips\n",
	"\t<mtd[:part]> <offset> <length> <mem addr>\n"
	"ex) # nanddump mtdXX \n"
	"    # nanddump mtdXX <mem addr>\n"
	"    # nanddump mtdXX <offset> <length>\n"	
	"    # nanddump mtdXX <offset> <length> <mem addr>\n"
);

int do_eraseall (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	struct mtd_info *mtd = &nand_info[0];
	struct erase_info instr;
	struct mtd_partition_info *mpi = NULL;
	uint32_t pos;
	int isNand, idx = -1;
	
	if (argc != 2) {
		printf ("Usage:\n%s\n", cmdtp->usage);
		return (-1);
	}

	sscanf(argv[1], "mtd%d", &idx);
	mpi = get_mtdpartinfo(idx);
	if(!mpi) {
		printf("unknown partition : %s\n", argv[1]);
		return (-1);
	}

	isNand = (mtd->type & MTD_NANDFLASH) ? 1 : 0;

	memset((void*)&instr, 0, sizeof(struct erase_info));
	instr.mtd = mtd;
	instr.len = (ulong)mtd->erasesize;
	
	for(pos = 0; pos < mpi->size; pos += mtd->erasesize) {
		loff_t ofs;
		
		show_progress("\t eraseall :", pos, mpi->size);
		if(isNand) {
			// bad check
			ofs = (loff_t)(CFG_FLASH_BASE + (mpi->offset + pos));
			if(mtd->block_isbad(mtd, ofs)) {
				printf("\t-> bad block at 0x%08x\n", ofs);
				return (-1);
			}
		}
		
		instr.addr = (mpi->offset + CFG_FLASH_BASE) + pos;
		if (mtd->erase(mtd, &instr)) {
			printf("mtd->erase error.. --;;\n");
			return (-1);
		}
	}

	show_progress("\t eraseall :", pos, mpi->size);
	printf("\n");

//	mpi->filesize = 0;
//	save_mtdinfo();
	
	mtd->sync(mtd);
	
	return 0;
}

U_BOOT_CMD(
	eraseall,	2,	0,	do_eraseall,
	"eraseall\t- erase the whole of a MTD partition\n",
	"\t<mtd[:part]>\n"
	"\t ex) # flash_eraseall mtd12\n"
);
/**
static int _checkff(uint8_t *buf, int len)
{
	unsigned int *tbuf = (unsigned int*)buf;
	int i, length = len / sizeof(unsigned int);
	for(i=0; i<length; i++)
		if(tbuf[i] != 0xffffffff)
			return 0;
	return 1;
}

static uint8_t *_fill_oob(struct mtd_info *mtd, struct nand_chip *chip, uint8_t *oob)
{	
	struct nand_oobfree *free = chip->ecc.layout->oobfree;
	uint8_t *oob_poi = chip->oob_poi;
	int i, totlen;

	for(i=0, totlen = 0; totlen < chip->ecc.layout->oobavail; i++) {
		memcpy(&oob_poi[free->offset], &oob[free->offset], free->length);
		totlen += free->length;
		free++;
	}

	return NULL;
}
**/
static int _nandwrite(struct mtd_info *mtd, uint32_t ofs, uint32_t len, void *buf, int oob)
{
//	struct nand_chip * chip = mtd->priv;
	uint8_t *datbuf, *oobbuf, *pagebuf;
	int tpagesize;
	loff_t to;
	int pagesize = mtd->oobblock;
	int oobsize = mtd->oobsize;

	tpagesize = pagesize;
	if(oob)
		tpagesize += oobsize;

	to = (loff_t)(CFG_FLASH_BASE + ofs);
	pagebuf = (uint8_t*)buf;
	
	while(len) {
	#if 1
		size_t retlen;
		if(oob) {
			// write oob
			oobbuf = &pagebuf[pagesize];
			mtd->write_oob(mtd, to, (size_t)oobsize, &retlen, oobbuf);
		}

		datbuf = &pagebuf[0];
		mtd->write_ecc(mtd, to, (size_t)pagesize, &retlen, datbuf, NULL, NULL);

		to += pagesize;
		len -= pagesize;
		pagebuf += tpagesize;
	#else
		chip->cmdfunc(mtd, NAND_CMD_RESET, -1, -1);
		
		if(oob) {
			// write oob
			oobbuf = &pagebuf[pagesize];
			memset(chip->oob_poi, 0xff, mtd->oobsize);
			_fill_oob(mtd, chip, oobbuf);
			chip->cmdfunc(mtd, NAND_CMD_SEQIN, pagesize, page);
			chip->write_buf(mtd, oobbuf, oobsize);
			chip->cmdfunc(mtd, NAND_CMD_PAGEPROG, -1, -1);
			status = chip->waitfunc(mtd, chip);
			if(status & NAND_STATUS_FAIL)
				return status;
		}
		
		// write data
		datbuf = &pagebuf[0];

		page++;
	#endif		
	}
	
	return 0;
}

int do_nandwrite(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	struct mtd_info *mtd = &nand_info[0];
	struct mtd_partition_info *mpi = NULL;
	unsigned int opt_oob, opt_start;
	char *mtd_dev, *filename;
	unsigned int offset, length;
	int idx=-1, opt, imglen;;
	int i;
	
	for(i=0; i<argc; i++)
		printf("arg[%d] : %s\n", i, argv[i]);

	opt_oob = opt_start = 0;
	while((opt = getopt(argc, argv, "os:")) != -1){
		switch(opt) {
		case 'o':
			opt_oob = 1;
			break;
		case 's':
			opt_start = simple_strtoul(optarg, NULL, 16);
			break;
		default:
			break;
		}
	}

	if((argc - optind) != 2) {
		printf ("Usage:\n%s\n", cmdtp->usage);
		return (-1);
	}

	mtd_dev = argv[optind++];
	filename = argv[optind];
	//printf("mtd : %s, file : %s\n", mtd_dev, filename);

	if(!(mtd->type & MTD_NANDFLASH)) {
		printf("mtd device is not nand flash type...!!\n");		
		return (-1);
	}

	sscanf(mtd_dev, "mtd%d", &idx);
	mpi = get_mtdpartinfo(idx);
	if(!mpi) {
		printf("unknown partition : %s\n", argv[1]);
		return (-1);
	}

	offset = (uint32_t)(mpi->offset + opt_start);
	length = (uint32_t) mpi->size;

	imglen = tftp_get(filename);
	if(imglen == 0) {
		printf("size is zero..!!\n");
		return (-1);
	}

	_nandwrite(mtd, offset, imglen, (void*)default_offset, opt_oob);
	
	mtd->sync(mtd);
	
	return 0;
}


U_BOOT_CMD(
	nandwrite,	5,	0,	do_nandwrite,
	"nandwrite\t- write to the specified MTD device with img file\n",
	"\t[option] MTD_DEVICE INPUTFILE\n\n"
	"\t -o       image contains oob data.\n"
	"\t -s addr  set start address\n"
	"\t -p       pad to page size\n"
);

