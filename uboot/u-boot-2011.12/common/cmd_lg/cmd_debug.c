//by dhjung LGE
#include <common.h>
#include <command.h>
#include "nvm.h"

extern void setenv (char *varname, char *varvalue);
extern int	saveenv(void);
extern char *env_name_spec;
extern void OSA_MD_SetDbgMode(UINT8	nDebugStatus);
extern int 	OSA_MD_GetDbgMode(void);

char * debubStatusStr[] = {
	"DEBUG",
	"EVENT",
	"RELEASE"
};
void set_releaseEnv(void)
{

	setenv("silent",	 	"y");
	setenv("silent_app", 	"y");
	setenv("silent_ker",	"y");
	setenv("lock_time", 	"0");
	setenv("enable_probe", 	"0");
}

void set_eventEnv(void)
{
	setenv("lock_time", "40000");
}

void set_debugEnv(void)
{
	setenv("lock_time", "40000");
	setenv("enable_probe", "1");
}

int do_debug (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	UINT8 nDebugStatus;

	if (argc == 1) {
		
		printf ("Cur : %s\n",debubStatusStr[OSA_MD_GetDbgMode()]);
		return 0;
	}

	if (argc != 2) {
		printf ("Usage:\n%s\n", cmdtp->usage);
		printf ("%s\n", cmdtp->help);
		return 1;
	}

	if (!strcmp(argv[1], "event")) {
		nDebugStatus = EVENT_LEVEL ;
		set_eventEnv();
	} else if (!strcmp(argv[1], "debug")) {
		nDebugStatus = DEBUG_LEVEL ;
		set_debugEnv();
	} else if (!strcmp(argv[1], "release")) {
		nDebugStatus = RELEASE_LEVEL ;
		set_releaseEnv();
	}
	else {
		printf ("Usage:\n%s\n", cmdtp->usage);
		printf ("%s\n", cmdtp->help);
		return 1;
	}

	printf("\n");
	printf ("Saving Environment to %s...\n", env_name_spec);

	if(saveenv()) {
		return 1;
	}
	OSA_MD_SetDbgMode(nDebugStatus);
	return 0;
}

U_BOOT_CMD(
	debug,	  2,	  0,	  do_debug,
	"debug\t- debug release or event or debug\n",
	"debug [release|event|debug]\n"
);
