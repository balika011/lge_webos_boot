#include <common.h>
#include <command.h>
#include <environment.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/ctype.h>
#include <exports.h>

#include <partinfo.h>
#ifdef CC_EMMC_BOOT
extern struct partmap_info partinfo;
#undef CFG_ENV_OFFSET
#define CFG_ENV_OFFSET	(off_t)((partinfo.dev.size) - CFG_ENV_SIZE)
#else
#include <nand.h>
#endif
#define KCMD_RAMDISK 	"root=/dev/ram0 rootfstype=squashfs"
#define KCMD_FLASH	 	"root=/dev/mtdblock%d lginit=/dev/mtdblock%d rootfstype=squashfs"
#define KCMD_NFS	 	"root=/dev/nfs rw nfsroot=156.147.69.181:/nfsroot,nolock ip=192.168.0.10::192.168.0.1:255.255.255.0:localhost:eth0:off"

#define CMD_NULL		""
#define CMD_SEMICOLON	"; "
#define	CMD_APPXIP		"xip lgapp"
#define	CMD_FONTXIP		"xip lgfont"

#define ENV_APPXIP		"appxip"
#define ENV_FONTXIP		"fontxip"

char *environ_list[]	= {
	ENV_APPXIP,
	ENV_FONTXIP
};

char *command_list[]	= {
	CMD_NULL,
	CMD_APPXIP	CMD_SEMICOLON,
	CMD_FONTXIP CMD_SEMICOLON,
	CMD_APPXIP	CMD_SEMICOLON	CMD_FONTXIP CMD_SEMICOLON,
};

//extern int setenv (char *varname, char *varvalue);
//extern char *getenv (char *name);
extern char *env_name_spec;
extern env_t *env_ptr;

#ifdef CFG_LG_CHG
extern int storage_write(off_t ofs, size_t len, u_char *buf);
#endif

int do_setboot (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	char *s;
	char bootargs[]	= "bootargs";
	char bootcmd[] 	= "bootcmd";
	char rootenv[] 	= "rootfs";
	char kcmd[512], bcmd[512], rootstr[32];
	int  addcmd = 0, i = 0,	ret = 0;
	struct partition_info * env_partinfo;
	if(argc != 2) {
		printf ("Usage:\n%s\n", cmdtp->usage);
		printf ("%s\n", cmdtp->help);
		return 1;
	}

	for (i = 0; i < 2; i++) {
		s = getenv(environ_list[i]);
		addcmd |= ((s && (*s == 'n')) ? 0 : 1) << i;
	}

#ifndef BLK_TMP
	/* ramdisk | flash | nfs */
	memset(kcmd, 0, 512);
	memset(bcmd, 0, 512);
	memset(rootstr, 0, 32);

	/* case ramdisk */
	if(!strcmp(argv[1], "ramdisk")) {
		sprintf(kcmd, KCMD_RAMDISK);

		/* set bcmd */
		sprintf(bcmd, "%scp2ram kernel 0x%x; cp2ram rootfs 0x%x; bootm 0x%x", command_list[addcmd], CFG_KERLOAD_ADDR, CFG_RAMDISK_ADDR, CFG_KERLOAD_ADDR);
	}
	/* case flash */
	else if(!strcmp(argv[1], "flash")) {
		sprintf(kcmd, KCMD_FLASH, get_partition_idx("rootfs"), get_partition_idx("lginit"));

		/* set bcmd */
		sprintf(bcmd, "%scp2ram kernel 0x%x; bootm 0x%x", command_list[addcmd], CFG_KERLOAD_ADDR, CFG_KERLOAD_ADDR);
	}
	/* case nfsroot */
	else if(!strcmp(argv[1], "nfs")) {
		/* set kcmd */
		sprintf(kcmd, KCMD_NFS);

		/* set bcmd */
		sprintf(bcmd, "%scp2ram kernel 0x%x; bootm 0x%x", command_list[addcmd], CFG_KERLOAD_ADDR, CFG_KERLOAD_ADDR);
	}
	else {
		printf("\n");
		printf("Wrong Type : %s\n", argv[1]);
		printf ("Usage:\n%s\n", cmdtp->usage);
		printf ("%s\n", cmdtp->help);
		return 1;
	}

	/* set rootstr */
	sprintf(rootstr, argv[1]);

	printf("%-9s = %s\n", bootargs, kcmd);
	printf("%-9s = %s\n", bootcmd,  bcmd);
	printf("%-9s = %s\n", rootenv,  rootstr);
#ifndef WEBOS_DEVELOPMENT
	//setenv(bootargs, kcmd);
	setenv(bootargs, "");
#endif
	setenv(bootcmd,  bcmd);
	setenv(rootenv,  rootstr);

	printf("\n");
#endif
	if((env_partinfo = get_partition_by_name("env_nvm")) != NULL)
	{
#if 1
        env_t   env_new;
        char    *res;
        res = (char *)&env_new.data;
        if (hexport_r(&env_htab, '\0', &res, ENV_SIZE, 0, NULL) < 0) {
            error("Cannot export environment\n");           
            return 1;
        }
        env_new.crc = crc32(0, env_new.data, ENV_SIZE);
        printf("[%s] env_part: %s\n", __func__, env_partinfo->name);
        printf("write to %x (len=%d,ptr = 0x%p) \n", (ulong)env_partinfo->offset, (ulong)env_partinfo->size, env_ptr);
        //ret = storage_write((ulong)env_partinfo->offset, (ulong)env_partinfo->size, (u_char *)env_ptr);
        ret = storage_write((ulong)env_partinfo->offset, (ulong)env_partinfo->size, (u_char *)&env_new);
#else
		printf("Saving Environment to %s...\n", env_name_spec);
		printf("[%s] env_part: %s\n", __func__, env_partinfo->name);
		printf("write to %x (len=%d,ptr = 0x%p) \n", (ulong)env_partinfo->offset, (ulong)env_partinfo->size, env_ptr);
		ret = storage_write((ulong)env_partinfo->offset, (ulong)env_partinfo->size, (u_char *)env_ptr);
#endif		
		if (ret) {
			printf("env save failed\n");
			return 1;
		}
	}
	else
	{
		printf("env_nvm partition not found.\n");
		return 1;	
	}

	 

	save_partinfo();

	return 0;
}

U_BOOT_CMD(
	setboot,	  2,	  0,	  do_setboot,
	"setboot\t- set boot type(root filesystem)\n",
	"setboot [ramdisk | flash | nfs]\n"
);

int do_bootmode (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	char *bootmode;
	char *bootmode_old;

	if(argc != 2) {
		printf ("Usage:\n%s\n", cmdtp->usage);
		printf ("%s\n", cmdtp->help);
		bootmode = getenv("bootmode");
		printf ("current bootmode = %s \n",bootmode);
		return 1;
	}

	if( !strcmp(argv[1],"auto") || !strcmp(argv[1],"webos") || !strcmp(argv[1],"user"))
	{
		bootmode_old = getenv("bootmode");
		printf ("bootmode is changed to %s from to %s \n",bootmode_old, argv[1]);
		setenv ("bootmode", argv[1]);

		saveenv();
	}
	else
	{
		printf ("Usage:\n%s\n", cmdtp->usage);
		printf ("%s\n", cmdtp->help);
		bootmode = getenv("bootmode");
		printf ("current bootmode = %s \n",bootmode);
		return 1;
	}

	return 0;
}

U_BOOT_CMD(
	bootmode,	  2,	  0,	  do_bootmode,
	"bootmode\t- set boot type( auto or webos or user )\n",
	"bootmode [auto|webos|user]\n"
);

int do_nset(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
//	extern int setenv (char *varname, char *varvalue);
	char	*address[] = {"ipaddr",	"serverip", "gatewayip","netmask",
					#if 0
						  "ethaddr",
					#endif
						  NULL};
	char	buf[200];
	int		i, isChanged = 0;
	char	*dhcp;
	int		dhcp_mode = 0;

#if 0
	if ((dhcp = getenv(ENV_DHCP)) != NULL && (dhcp[0] == 'o' && dhcp[1] == 'n'))
	{
		NetLoop(DHCP);
		dhcp_mode = 1;
	}
#endif
	for (i = 0; address[i] != NULL; i++)
	{
		char	*tmp    = getenv(address[i]);
		char	old_ip[40];
		int		isInput = 1;

		if(tmp == NULL) old_ip[0] = '\0';
		else            strcpy(old_ip, tmp);
		printf("\t- %11s : %-15s => ", address[i], old_ip);
		gets(&buf[0]);
		printf("\n");

		if (strlen(&buf[0]) == 0)
		{
			isInput = 0;
			strcpy(&buf[0], old_ip);
		}

		if (isInput)
		{
			setenv(address[i], &buf[0]);
		#ifdef CONFIG_GP_PLATFORM
			if(strcmp(address[i], "ethaddr") == 0)
			{
				write_mac(buf);
			}
		#endif
		}

		if(strcmp(&buf[0], old_ip) != 0)
		{
			isChanged = 1;
#if 0
			if (dhcp_mode)
			{
				extern void setDhcpConnectState(int value);
				setenv (ENV_DHCP, "off");
				setDhcpConnectState(0/*off*/);
			}
#endif
		}
	}

	if (isChanged)
	{
		saveenv();
	}
	return 0;
}

U_BOOT_CMD(
	nset, 3, 0, do_nset,
	"nset- set network address\n",
	""
);
#ifdef CC_LGE_PROTO_PCBA
int do_nfsroot_cmd(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    int		cmd_argc = 0;
    char	buf[128];
    int		isChanged = 0;
    int		i;
    const char	*item[] = {"nfsserver", "nfsroot", NULL};

    for(i=0; item[i] != NULL; i++) {
		char *v;
        int is_input = 1;
		const char *old_value = getenv(item[i]);

		do {
		    printf("\t- %11s : %-18s => ", item[i], (old_value) ? old_value : "");
		    gets(buf);
		    v = strtrim(buf);
		    printf("\n");

		    if(strlen(v) == 0) {
				is_input = 0;
				break;
		    }

		    if(!strcmp(item[i], "nfsserver")) {
				if(is_valid_ip(v)) break;
		    } else break;
		    printf("Invalid format. Input again !!!\n");
		} while(1);

		if(!is_input) continue;
		
		if(strcmp(old_value, v)) {
		    setenv(item[i], v);
		    isChanged = 1;
		}
    }
	if(isChanged)
	    saveenv();

	return 0;
}

U_BOOT_CMD(
	nfsroot, 3, 0, do_nfsroot_cmd,
	"nfsroot- set nfsroot\n",
	""
);
#endif

