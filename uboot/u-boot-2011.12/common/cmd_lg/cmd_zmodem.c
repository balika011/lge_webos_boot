#include <common.h>
#include <command.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/ctype.h>

extern int rz(const char *name, unsigned long addr);

int do_rz (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	ulong size=0;
	ulong address=0;

	address = simple_strtoul(argv[1], NULL, 16);

	size = rz("test.txt", address);

	if(!size) {
		printf("rz() failed...(size:%d)\n", (int)size);
		return -1;
	}

	return 0;
}

U_BOOT_CMD(
	rz,	2,	0,	do_rz,
	"rz\t- downlaod image file,though zmodem\n",
	"[address]\n"
);
