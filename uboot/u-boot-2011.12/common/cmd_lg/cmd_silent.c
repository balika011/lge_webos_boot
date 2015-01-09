//by dhjung LGE
#include <common.h>
#include <command.h>

extern void setenv (char *varname, char *varvalue);
extern int	saveenv(void);
extern char *env_name_spec;

int do_silent (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	if (argc != 2) {
		printf ("Usage:\n%s\n", cmdtp->usage);
		printf ("%s\n", cmdtp->help);
		return 1;
	}

	if (!strcmp(argv[1], "all")) {
		setenv("silent", 	 "y");
		setenv("silent_app", "y");
		setenv("silent_ker", "y");
	} else if (!strcmp(argv[1], "nothing")) {
		setenv("silent", 	 "n");
		setenv("silent_app", "n");
		setenv("silent_ker", "n");
	} else {
		printf ("Usage:\n%s\n", cmdtp->usage);
		printf ("%s\n", cmdtp->help);
		return 1;
	}

	printf("\n");
	printf ("Saving Environment to %s...\n", env_name_spec);

	if(saveenv()) {
		return 1;
	}

	return 0;
}

U_BOOT_CMD(
	silent,	  2,	  0,	  do_silent,
	"silent\t- silent all or nothing\n",
	"silent [all | nothing]\n"
);
