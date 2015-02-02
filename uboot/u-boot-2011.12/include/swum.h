#include <command.h>
#include <partinfo.h>
#include <environment.h>
#include <cmd_resume.h>

#define MAGIC(a,b,c,d)	((a) | (b) << 8 | (c) << 16 | (d) << 24)
#define SWUM_MAGIC		MAGIC('S', 'W', 'U', 'M')

extern int get_swumode(void);
