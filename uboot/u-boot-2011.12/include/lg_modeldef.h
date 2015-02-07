#ifndef _LG_MODELDEF_H_
#define _LG_MODELDEF_H_

#define DEBUG_LEVEL					3
#define EVENT_LEVEL					4
#define RELEASE_LEVEL				5

#define NFS_PARAMETER				"nolock,tcp,rsize=4096,wsize=4096"

extern int	DDI_NVM_GetDebugStatus( void );

#define VERIFY_RETRY_MAX    (1)

#define VERIFY_APPS_DONE 		(0x0001)
#define VERIFY_KERNEL_DONE		(0x0010)
#define VERIFY_TZFW_DONE		(0x0100)

extern int	verify_done;

#endif /*_LG_MODELDEF_H_ */

