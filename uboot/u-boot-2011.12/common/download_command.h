#ifndef __DOWNLOAD_COMMANDS_H
#define __DOWNLOAD_COMMANDS_H

#include <common.h>
#include <watchdog.h>
#include <command.h>
#include <image.h>
#include <malloc.h>
#include <u-boot/zlib.h>
#include <bzlib.h>
#include <environment.h>
#include <lmb.h>
#include <linux/ctype.h>
#include <asm/byteorder.h>
#include <linux/compiler.h>

void download_value_init(void);

void do_resetBootloader(const char *arg,const char *data, unsigned int sz);
void do_continue(const char *arg, const char *data, unsigned int sz);
void do_reboot(const char *arg, const char *data, unsigned int sz);
int do_download(const char *arg, const char *data,unsigned int sz );
int do_update(const char *arg, const char *data,unsigned int sz );
int do_getvar(const char *arg, const char *data, unsigned int sz );
int do_erase_emmc(const char *arg, const char *data, unsigned int sz);

#endif


