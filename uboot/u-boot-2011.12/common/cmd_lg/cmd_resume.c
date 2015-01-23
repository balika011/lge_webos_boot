#include <common.h>
#include <command.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/ctype.h>
#include <exports.h>
#include <net.h>
#include <usb.h>
#include <xyzModem.h>
#include <environment.h>
#include <mmc.h>
#include <partinfo.h>
#include <errno.h>
#include "cmd_polltimer.h"
#include <cmd_resume.h>
#include <thread.h>

#include <lg_modeldef.h>
#ifdef SIGN_USE_PARTIAL
#include "x_ldr_env.h"
#endif
#if defined(CONFIG_MULTICORES_PLATFORM)
#include <smp_platform.h>
#endif

static spin_lock_t decomp_processing_lock = INIT_SPIN_LOCK;
static u32 decomp_elapsed[NR_CPUS] = {0,};
static int decomp_do[NR_CPUS] = {0,};
static u32 emmc_elapsed = 0;
//static cond_t g_decomp_cond;
static int wait_cnt = 0;
static u32 g_read_bytes = 0;
static loff_t g_snapshot_offset = 0;
static int decomp_done[NR_CPUS] = {0,};
static int use_single_core = 0;
static int decomp_error_cpumask = 0;

typedef struct _thread_arg
{
	loff_t offset;
	unsigned long image_size;
}thread_arg_t;

extern void DDI_NVM_SetMakeHib( unsigned char mode );
extern unsigned char DDI_NVM_GetSnapShot_Support( void );
extern unsigned char DDI_NVM_GetSnapShotART( void );
extern void DDI_NVM_SetSnapShotART( unsigned char mode );
extern void DDI_NVM_SetSnapShot_Support( unsigned char mode );
extern int storage_read(uint32_t offset, size_t len, void* buf);
extern int storage_erase(unsigned long long offset, unsigned long long len);
extern void enable_console(void);
extern void disable_console(void);
#if defined(CONFIG_MULTICORES_PLATFORM)
extern void release_non_boot_core(void);
#endif
//extern void display_boottime_log(void);


#define TIME_CHECK() \
{ unsigned int u32Timer_b, u32Timer_c; \
	u32Timer_c = (*((volatile unsigned int *)0x1f006090)) | ((*((volatile unsigned int *)0x1f006094)) << 16); \
	u32Timer_b = (*((volatile unsigned int *)0x1f206708)) | ((*((volatile unsigned int *)0x1f20670c)) << 16); \
	printf("\033[0;31m[time] mboot takes %d.%dms \033[0m\n", (u32Timer_c - u32Timer_b)/12000, ((u32Timer_c - u32Timer_b)/12)%1000); \
}

void remake_hib(void)
{
	struct partition_info *partition  = NULL;
	int idx  = -1;
	DDI_NVM_SetMakeHib(1);
	idx = get_partition_idx(SNAP_PART_NAME);
	if(0 < idx)
	{
		partition = GET_PART_INFO(idx);
	}
	if(NULL != partition)
	{
		storage_erase(partition->offset, 0x1000);
	}
}


#define MiB(x) (x/(1024 *1024))
static void print_snapshot_header(const struct swsusp_info *si)
{
	struct snapshot_header *header = GET_SNAP_HEADER(si);
	struct dep_parts *deps = &header->deps;

	printf("/***************************************************************/\n");
	printf("UTS sys name = %s\n", si->uts.sysname);
	printf("UTS node name = %s\n", si->uts.nodename);
	printf("UTS release = %s\n", si->uts.release);
	printf("UTS version = %s\n", si->uts.version);
	printf("UTS machine = %s\n", si->uts.machine);
	printf("UTS domainname = %s\n", si->uts.domainname);

	printf("Kernel %d.%d.%d\n",
			((si->version_code) >> 16) & 0xff,
			((si->version_code) >> 8) & 0xff,
			(si->version_code) & 0xff);

	printf("num_phyuspages = %lu\n", si->num_physpages);
	printf("cpus = %d\n", si->cpus);
	printf("image_pages = %lu\n", si->image_pages);
	printf("pages = %lu\n", si->pages);
	printf("size = %lu\n", si->size);

	if(header->compress_algo)
		printf("compressed(%s) image size = %luMB(%lu)\n", (header->compress_algo == LZO) ?
		"lzo" : ((header->compress_algo == LZ4) ? "lz4" : ((header->compress_algo == LZ4HC) ? "lz4hc" : "unknown")),
		MiB(header->image_size), header->image_size);
	else
		printf("uncompressed image size = %luMB(%lu)\n", MiB(header->image_size), header->image_size);

	if(header->crc != ~0)
		printf("crc = %u\n", header->crc);
	else
		printf("no crc image\n");
	printf("pfn merge info entry count = %u\n", header->pfn_mi_cnt);
	printf("metadata start offset for compressed image = %lu\n", header->metadata_start_offset_for_comp);
	printf("physical address resume func addr %x\n", header->pa_resume_func);
	printf("snapshot magic number : 0x%lx\n", header->magic);
	if (deps->nr_dep_parts) {
		int i;
		struct dep_part_info *dep_info = &deps->dep_part_info[0];

		printf("snapshot dependency %d partitions\n", deps->nr_dep_parts);
		printf(" name\t partnum\t %s\t %s\n", deps->value_name[0], deps->value_name[1]);

		for (i = 0; i < deps->nr_dep_parts; i++) {
			struct partition_info *partition =
				&partinfo.partition[dep_info[i].partnum];
			printf(" %s\t  %4d\t\t 0x%-8x\t  0x%x\n",
			        partition->name,
					dep_info[i].partnum,
					dep_info[i].value[0],
					dep_info[i].value[1]);
		}
	} else {
		printf("snapshot has no dependency partitions\n");
	}
	printf("/***************************************************************/\n");

	return;
}

static unsigned int get_signed_image_size(struct snapshot_header *header)
{
	unsigned int size;

	size =  ALIGN(header->image_size, 16);

#if defined (CONFIG_SECURITY_BOOT)
	size += SIGNATURE_SIZE * (NUMBER_OF_FRAGMENT+1);
#endif
	return size;
}

struct swsusp_info *info = (struct swsusp_info *)SNAPSHOT_IMAGE_LOAD_ADDR;
void *meta_page_ptr;
void (*kernel_resume_func)(void);
static char *decomp_buf;
static struct pfn_merge_info *pfn_merge_info;
static unsigned int processed_pfn_mi_index;

#ifdef CONFIG_SECURITY_BOOT

int verify_snapshot_image(struct snapshot_header *header)
{
	int index = 0;
	unsigned char *signature_offset = decomp_buf - HEADER_SIZE + header->image_size ;
	unsigned char signature[32];
	static unsigned char sign_area[FRAGMENT_UNIT_SIZE];

	if((NUMBER_OF_FRAGMENT * FRAGMENT_UNIT_SIZE) > (header->image_size - HEADER_SIZE))
	{
		printf("boundary check error %s (%d)\n", __FUNCTION__, __LINE__);
		return -1;
	}

	//sign_area = (char *) malloc(FRAGMENT_UNIT_SIZE * sizeof(char));
	index = (get_timer(0) % NUMBER_OF_FRAGMENT);
	memcpy(sign_area,decomp_buf + (index * FRAGMENT_UNIT_SIZE),FRAGMENT_UNIT_SIZE);
	memcpy(signature,signature_offset + ((index + 1) * SIGNATURE_SIZE),SIGNATURE_SIZE);

	if( !snapshot_image_verify(signature, sign_area, FRAGMENT_UNIT_SIZE) )
	{
		printf("snapshot partial verification successed! (index:%d)\n", index);
	}
	else
	{
		printf("snapshot partial verification failed! (index:%d)\n", index);
		return -1;
	}
	return 0;
#if 0 //def SNAPSHOT_DEBUG
    printf("\n");
    for(i=0; i<32; ++i)
        printf("%02x ", 0xFF & ((char*)signature)[i]);
    printf("\n");
#endif
#if 0
	printf("[%d] full verify snapshot image\n", readMsTicks());
	sign_area = decomp_buf;
	signature = signature_offset;
	if(0 == snapshot_image_verify(signature, sign_area, hib_size - HEADER_SIZE))
	{
		printf("snapshot full verification successed!\n");
		}
	else
	{
		enable_console();
		printf("snapshot full verification failed!\n");
		if((DDI_NVM_GetDebugStatus() == RELEASE_LEVEL) || (DDI_NVM_GetDebugStatus() == EVENT_LEVEL) || (DDI_NVM_GetDebugStatus() == DEBUG_LEVEL))
		{
			char cmd[] = "reset";
			remake_hib();
			run_command(cmd, 0);
		}
		else
		{
			while (1); /* halt */
		}
	}
#endif
}
#endif


#if defined(CONFIG_MULTICORES_PLATFORM)
extern u64 arch_counter_get_ms(void);

static int emmc_user_callback(int err, int bytes)
{
	g_read_bytes += bytes;
}
static int read_emmc(void *arg)
{
	u32 start_time;
	thread_arg_t *p = (thread_arg_t*)arg;
	thread_arg_t a;
	a.offset = p->offset;
	a.image_size = p->image_size;

	//eMMC_register_callback(emmc_user_callback);

	g_read_bytes = 0;
	start_time = (u32)arch_counter_get_ms();
	if (storage_read(a.offset, a.image_size, (void*)(decomp_buf)) < 0)
	{
		printf("Can't read compressed snapshot image image\n");
		return -1;
	}
	if( g_read_bytes < a.image_size )
		tlog("eMMC error : wrong read size in callback request size=%u, read=%u\n",a.image_size,g_read_bytes);
	emmc_elapsed = (u32)arch_counter_get_ms() - start_time;
	decomp_done[1] = 1;
	return 0;
}

void storage_dma_read_wait(int len)
{
	while( 1 ) {
		if( len > g_read_bytes  ) {
			udelay(5);
			wait_cnt++;
		} else
			break;
	}
}
#endif


static int decompress_routine(void *print)
{
	unsigned int local_pfn_mi_index, pre_pfn_mi_index = 0;
	unsigned int start_pfn;
	unsigned long compblock_len;
	size_t output_len;
	size_t local_start_offset = 0;
	int i;
	int cpuid = 0;
	struct snapshot_header *header = GET_SNAP_HEADER(info);

#if defined(CONFIG_MULTICORES_PLATFORM)
	u32 start_time;
	cpuid =  get_cpu_id();
	thread_t *thread = get_current_thread(cpuid);
	start_time = (u32)arch_counter_get_ms();
	//decomp_do[cpuid] = 0;
#endif

	while(1) {
#if defined(CONFIG_MULTICORES_PLATFORM)
		spin_lock(&decomp_processing_lock);
		local_pfn_mi_index = processed_pfn_mi_index;
		processed_pfn_mi_index++;
		spin_unlock(&decomp_processing_lock);
		//decomp_do[cpuid]++;
#else
		local_pfn_mi_index = processed_pfn_mi_index;
		processed_pfn_mi_index++;
#endif
		if (local_pfn_mi_index >= header->pfn_mi_cnt)
			break;

		compblock_len = pfn_merge_info[local_pfn_mi_index].info.compblock_len;
		start_pfn = pfn_merge_info[local_pfn_mi_index].start_pfn;

		for (i = pre_pfn_mi_index; i < local_pfn_mi_index; i++) {
			local_start_offset += pfn_merge_info[i].info.compblock_len;
		}

		pre_pfn_mi_index = local_pfn_mi_index;

#if defined(CONFIG_MULTICORES_PLATFORM)
		if( !use_single_core )
			storage_dma_read_wait(local_start_offset + compblock_len);
#endif

        if (pfn_merge_info[local_pfn_mi_index].compressed == 1)
        {

            if (header->compress_algo == LZ4HC || header->compress_algo == LZ4)
            {
                output_len = LZ4_uncompress_unknownOutputSize((const void *)(decomp_buf + local_start_offset), \
                (void *)(start_pfn << PAGE_SHIFT), compblock_len, pfn_merge_info[i].info.merged_cnt * PAGE_SIZE);
            }
            else if (header->compress_algo == LZO)
            {
                lzo1x_decompress((const void *)(decomp_buf + local_start_offset), (size_t)compblock_len,\
                (void *)(start_pfn << PAGE_SHIFT), &output_len);
            }
            else
            {
                printf("%s : Unknown decompress type!\n");
            }

            if( print ) tlog("cpu[%d] [%4d] decomp -> in : %p, in_len : 0x%x, out : 0x%x, out_len : 0x%x\n",cpuid,local_pfn_mi_index, decomp_buf + local_start_offset, compblock_len, start_pfn << PAGE_SHIFT, output_len);


            if (output_len != pfn_merge_info[i].info.merged_cnt * PAGE_SIZE)
            {
                tlog("snapshot image decompress cpu=%d,index : %d fail offset = 0x%lx, pfn = 0x%x, outlen=%u,orglen=%u\n",cpuid, local_pfn_mi_index, local_start_offset, start_pfn,output_len,pfn_merge_info[i].info.merged_cnt * PAGE_SIZE);
				decomp_error_cpumask |= 1 << cpuid;
				decomp_done[cpuid] = 1;
                return -1;
            }
        }
        else
        {
			if( print ) tlog("cpu[%d] [%4d] memcpy -> in : 0x%x, in_len : 0x%x, out : 0x%x\n",cpuid, local_pfn_mi_index, SNAPSHOT_IMAGE_LOAD_ADDR + local_start_offset, compblock_len, start_pfn << PAGE_SHIFT);

			memcpy((void *)(start_pfn << PAGE_SHIFT), (void *)(decomp_buf + local_start_offset), compblock_len);
		}
	}
#if defined(CONFIG_MULTICORES_PLATFORM)
	//decomp_elapsed[cpuid] = (u32)arch_counter_get_ms() - start_time;
	decomp_done[cpuid] = 1;
	//if( !use_single_core ) thread_cond_signal(&g_decomp_cond);
#endif

	return 0;
}

static int uncompressed_snapshot_image_restore(unsigned long nr_meta_pages, loff_t offset_cur)
{
	uint32_t *pfn_ptr;
	unsigned int read_pages = 0;

	printf("[%4d]Loading snapshot image...\n", readMsTicks());

	pfn_ptr = (uint32_t *)(meta_page_ptr);

#if 1
	while (read_pages < info->image_pages) {
#else
	while (read_pages != info->image_pages) {
#endif
		if (storage_read(offset_cur, PAGE_SIZE, (void *)((*pfn_ptr) << PAGE_SHIFT)) < 0) {
			printf("Can't read snapshot image\n");
			return -1;
		}
		pfn_ptr++;
		read_pages++;
		offset_cur += PAGE_SIZE;
	}

	printf("[%4d]Completed... [%dKBytes]\n", readMsTicks(), (read_pages << 2));

	return 0;
}

static int compressed_snapshot_image_restore(loff_t offset_cur, int verify, int desc_print, int decomp_print)
{
#if defined(CONFIG_MULTICORES_PLATFORM)
	/* Multithread support */
	char thread_name[15];
	thread_t *decomp_2,*decomp_3,*emmc_thread;
	thread_arg_t arg;
	u32 start_time;
	int cnt = 0;
#endif

	// =================================================================
	// 1. get snapshot header
	// =================================================================
	struct snapshot_header *header = GET_SNAP_HEADER(info);

	/* compressed payload size = total size - header size - metadata size */
	unsigned long snapshot_image_payload_size = get_signed_image_size(header) - PAGE_SIZE;

	pfn_merge_info = (struct pfn_merge_info *)meta_page_ptr;
	processed_pfn_mi_index = 0;

#if defined(CONFIG_MULTICORES_PLATFORM)
	start_time = (u32)arch_counter_get_ms();
	memset(decomp_done,0,sizeof(decomp_done));
#endif

#ifdef SNAPSHOT_VERIFY
	// =================================================================
	// 2. snapshot verify
	// =================================================================
    // signature offset = current offset(offset + PAGE_SIZE) - PAGE_SIZE(header) + compressed image size
	const loff_t singature_offset = offset_cur - HEADER_SIZE + header->image_size;
	const unsigned long hib_size = header->image_size;
	unsigned char signature[32] = {0,};
	unsigned int index = 0;
	char *sign_area = NULL;

#ifdef SNAPSHOT_PARTIAL_VERIFY
	// partial verification
	if((NUMBER_OF_FRAGMENT * FRAGMENT_UNIT_SIZE) > (hib_size - HEADER_SIZE))
	{
		printf("boundary check error %s (%d)\n", __FUNCTION__, __LINE__);
		return -1;
	}

	sign_area = (char *) malloc(FRAGMENT_UNIT_SIZE * sizeof(char));
	if(NULL == sign_area)
	{
		printf("can not allocate memory %s (%d)\n", __FUNCTION__, __LINE__);
		return -1;
	}

	index=(get_timer(0) % NUMBER_OF_FRAGMENT);
	if (-1 == storage_read(offset_cur + (index * FRAGMENT_UNIT_SIZE), FRAGMENT_UNIT_SIZE, sign_area))
	{
		printf("io read error %s (%d)\n", __FUNCTION__, __LINE__);
		free(sign_area);
		return -1;
	}

	if(-1 == storage_read(singature_offset + ((index + 1) * sizeof(signature)), sizeof(signature), signature))
	{
		printf("io read error %s (%d)\n", __FUNCTION__, __LINE__);
		free(sign_area);
		return -1;
	}

#if 0 //def SNAPSHOT_DEBUG
	printf("\n");
	for(i=0; i<32; ++i)
		printf("%02x ", 0xFF & ((char*)signature)[i]);
	printf("\n");
#endif

	printf("[%d] partial verify snapshot image\n", readMsTicks());
	if(0 == snapshot_image_verify(signature, sign_area, FRAGMENT_UNIT_SIZE))
		{
		printf("snapshot partial verification successed! (index:%d)\n", index);
		goto skip_full_verification;
		}
	else
	{
		printf("snapshot partial verification failed! (index:%d)\n", index);
		free(sign_area);
		sign_area = NULL;
	}
#endif

	// full verification
	sign_area = (char *) malloc((hib_size - HEADER_SIZE) * sizeof(char));
	if(NULL == sign_area)
	{
	    printf("can not allocate memory %s (%d)\n", __FUNCTION__, __LINE__);
		return -1;
	}
	printf("[%d] snapshot data read, image_leghth = %d\n", readMsTicks(), singature_offset - offset_cur);
	if (-1 == storage_read(offset_cur, (hib_size - HEADER_SIZE), sign_area))
	{
	    printf("io read error %s (%d)\n", __FUNCTION__, __LINE__);
	    free(sign_area);
	    return -1;
	}

	if(-1 == storage_read(singature_offset, sizeof(signature), signature))
	{
	    printf("io read error %s (%d)\n", __FUNCTION__, __LINE__);
	    free(sign_area);
	    return -1;
	}

#if 0 //def SNAPSHOT_DEBUG
    printf("\n");
    for(i=0; i<32; ++i)
        printf("%02x ", 0xFF & ((char*)signature)[i]);
    printf("\n");
#endif

	printf("[%d] full verify snapshot image\n", readMsTicks());
	if(0 == snapshot_image_verify(signature, sign_area, hib_size - HEADER_SIZE))
	{
		printf("snapshot full verification successed!\n");
		}
	else
	{
		enable_console();
		printf("snapshot full verification failed!\n");
		if((DDI_NVM_GetDebugStatus() == RELEASE_LEVEL) || (DDI_NVM_GetDebugStatus() == EVENT_LEVEL) || (DDI_NVM_GetDebugStatus() == DEBUG_LEVEL))
		{
			char cmd[] = "reset";
			remake_hib();
			run_command(cmd, 0);
		}
		else
		{
			while (1); /* halt */
	}
	}
skip_full_verification:
	free(sign_area);
	sign_area = NULL;
	printf("[%d] verify completed\n", readMsTicks());
#endif

	// =================================================================
	// 3. snapshot decompress
	// =================================================================
	decomp_buf = (char *)(SNAPSHOT_IMAGE_LOAD_ADDR + PAGE_SIZE);

#if defined(CONFIG_MULTICORES_PLATFORM)

	//flush_cache_all();
	start_time = (u32)arch_counter_get_ms();

	if( use_single_core ) {
		if (storage_read(offset_cur, snapshot_image_payload_size, (void*)(decomp_buf)) < 0)
		{
			printf("Can't read compressed snapshot image image\n");
			return -1;
		}
		decompress_routine((void *)decomp_print);
		printf("single thread decomp time = %u\n",(u32)arch_counter_get_ms()-start_time);
	} else {
		arg.offset = offset_cur;
		arg.image_size = snapshot_image_payload_size;

		//thread_cond_new(&g_decomp_cond);

		sprintf(thread_name,sizeof(thread_name),"read_emmc");
		emmc_thread = thread_create_ex(thread_name,read_emmc,&arg,0,1,THREAD_DEFAULT_PRIORITY,0);
		sprintf(thread_name,sizeof(thread_name),"decomp[2]");
		decomp_2 = thread_create_ex(thread_name,decompress_routine,(void *)decomp_print,0,2,THREAD_DEFAULT_PRIORITY,0);
		sprintf(thread_name,sizeof(thread_name),"decomp[3]");
		decomp_3 = thread_create_ex(thread_name,decompress_routine,(void *)decomp_print,0,3,THREAD_DEFAULT_PRIORITY,0);
		decompress_routine((void *)decomp_print);

		if( !decomp_done[0] || !decomp_done[1] || !decomp_done[2] || !decomp_done[3] )
			while( 1 ) {
				//ret = thread_cond_timedwait(&g_decomp_cond,5);
				mtk_msleep(2);
				cnt++;
				printf("Waiting for decompression done cnt=%d,CPU0=%d,CPU1=%d,CPU2=%d,CPU3=%d\n",cnt,decomp_done[0],decomp_done[1],decomp_done[2],decomp_done[3]);
				if( decomp_done[0] && decomp_done[1] && decomp_done[2] && decomp_done[3] ) {
					break;
				}
			}
#if 0
		tlog("Decomp time CPU0=%u ms,cnt=%d | CPU2=%u ms,cnt=%d | CPU3=%u ms,cnt=%d | eMMC=%u ms,wait=%d,waiting for eMMC time=%u ms\n",decomp_elapsed[0],decomp_do[0],decomp_elapsed[2],decomp_do[2],decomp_elapsed[3],decomp_do[3],emmc_elapsed,wait_cnt,(5*wait_cnt)/1000);
		tlog("multi thread decomp time = %u\n",(u32)arch_counter_get_ms()-start_time);
#endif
		if( desc_print ) {
			u32 speed;
			if( emmc_elapsed < 1000 ) {
				speed = (snapshot_image_payload_size>>10) / (emmc_elapsed);
				speed *= 1000;
			} else {
				speed = (snapshot_image_payload_size>>10) / (emmc_elapsed/1000);
			}
			tlog("eMMC size=%u,read speed=%u KB(%u MB) per second\n",snapshot_image_payload_size,speed,speed >> 10);
		}
	}
#else

	printf("[%d] read snapshot image\n", readMsTicks());
	if (storage_read(offset_cur, snapshot_image_payload_size, (void*)(decomp_buf)) < 0)
	{
		printf("Can't read compressed snapshot image image\n");
		return -1;
	}
	printf("[%d] decompress snapshot image\n", readMsTicks());
	decompress_routine((void *)0);
#endif

#ifdef CONFIG_SECURITY_BOOT
	if( verify )
		if( verify_snapshot_image(header) < 0 )
			return -1;
#endif

	TIME_CHECK();

	if(decomp_error_cpumask) {
		printf("Snapshot booting failed!! : Decomp error in cpu=0x%x\n",decomp_error_cpumask);
		return -1;
	}

	return 0;
}

int storage_get_partition(const char* name, storage_partition_t* info)
{
	struct partition_info *pi;

	if (((pi = get_used_partition(name)) == NULL) &&
		((pi = get_unused_partition(name)) == NULL))
	{
		return -1;
	}

//	strcpy(info->name, pi->name);
	info->offset = pi->offset;
	info->size = pi->size;
	info->filesize = pi->filesize;
	info->used = pi->used;
	info->valid = pi->valid;

	return 0;
}

static inline int is_valid_snapshot_header(const struct swsusp_info *si)
{
	struct snapshot_header *header = GET_SNAP_HEADER(si);

	if (header->magic == LG_SNAPSHOT_MAGIC_CODE)
		return 1;

	return 0;
}


static int check_snapshot_image(void)
{
	storage_partition_t partition;

	/* Get partition which has snapshot boot image(hibernation image) */
	if (storage_get_partition(SNAP_PART_NAME, &partition) < 0) {
		printf("Invalid snapshot image partition\n");
		return -1;
	}
	g_snapshot_offset = partition.offset;

	/* Get header */
	if (storage_read(g_snapshot_offset , sizeof(struct swsusp_info), info) < 0) {
		printf("Can't read snapshot image header\n");
		return -1;
	}

	return is_valid_snapshot_header(info);
}
static int is_factory_mode(void)
{
	if( MICOM_IsPowerOnly() || !DDI_NVM_GetInstopStatus() )
		return 1;

	return 0;
}

int check_snapshot_mode(void)
{
	char *bootmode;
	char *snapshot_mode;
	int ret;


	bootmode = getenv("bootmode");

	printf("boot mode : %s\n", bootmode);
	printf("debug status : %d\n", DDI_NVM_GetDebugStatus());
	printf("swu mode : %d\n", DDI_NVM_GetSWUMode());
	printf("snapshot support : %d\n", DDI_NVM_GetSnapShot_Support());


	if(!strcmp(bootmode, "webos") || DDI_NVM_GetSWUMode() || (DDI_NVM_GetSnapShot_Support() == 0) || is_factory_mode() )
	{
		ret = LGSNAP_COLDBOOT;
		snapshot_mode = "cold";
	}
	else if( !check_snapshot_image() )
	{
		ret = LGSNAP_MAKING_IMAGE;
		snapshot_mode = "making";
	}
	else
	{
		ret = LGSNAP_RESUME;
		snapshot_mode = "resume";
	}

	printf("check_snapshot_mode : %s\n", snapshot_mode);
	return ret;
}

/* 0 if ok, positive value if mismatched partitions found */
static int verify_dep_parts(const struct swsusp_info *si)
{
	int nr_dep_parts, i;
	int part_mismatch = 0;
	struct snapshot_header *header;
	struct dep_parts *deps;
	struct dep_part_info *dep_part_info;

	header = GET_SNAP_HEADER(si);
	deps = &header->deps;
	dep_part_info = &deps->dep_part_info[0];

	/* check it's valid header */
#if 0
	if (!is_valid_snapshot_header(si)) {
		printf("invalid snapshot header\n");
		return -1;
	}
#endif
	nr_dep_parts = deps->nr_dep_parts;
	/* sanity check */
	if (nr_dep_parts > MAX_DEP_PARTS || nr_dep_parts > PARTITION_MAX) {
		printf("number of dependent partition %d exceeds max %d\n",
		       nr_dep_parts, MAX_DEP_PARTS);
		return -1;
	}

	dep_part_info = &deps->dep_part_info[0];


	// FIXME : assuming value0 is filesize and value1 is sw_ver.
	for (i = 0; i < nr_dep_parts; i++) {

		int partnum = dep_part_info[i].partnum;
		int value0 = dep_part_info[i].value[0];
		int value1 = dep_part_info[i].value[1];
		struct partition_info *partition;

		if (partnum > partinfo.npartition - 1)
			return -1;

		partition = &partinfo.partition[partnum];

		if (value0 != partition->filesize) {
			printf("partition %s "
			       "doesn't have the identical filesize (%d,%d)",
			       partition->name,
			       partition->filesize,
			       value0);
			part_mismatch++;
		}

		if (value1 != partition->sw_ver) {
			printf("partition %s "
			       "doesn't have the identical sw version (%d,%d)",
			       partition->name,
			       partition->sw_ver,
			       value1);
			part_mismatch++;
		}
	}

	return part_mismatch;
}


int snapshot_boot(int verify, int load_only, int header_print, int desc_print, int decomp_print)
{
	storage_partition_t partition;
	loff_t offset, offset_datapage;
	unsigned long nr_meta_pages;
	int part_mismatch = 0;
	struct snapshot_header *header;

#if defined(LG_CHG)
#ifdef CONFIG_SECURITY_BOOT
    verify_apps(BOOT_SNAPSHOT);
#endif
#endif

	offset = g_snapshot_offset;
	TIME_CHECK();
	if( !offset ) {
		/* Get partition which has snapshot boot image(hibernation image) */
		if (storage_get_partition(SNAP_PART_NAME, &partition) < 0) {
			printf("Invalid snapshot image partition\n");
			return -1;
		}
		offset = partition.offset;

		/* Get header */
		if (storage_read(offset, sizeof(struct swsusp_info), info) < 0) {
			printf("Can't read snapshot image header\n");
			return -1;
		}
	}
	header = GET_SNAP_HEADER(info);

	if(header_print)
		print_snapshot_header(info);

	part_mismatch = verify_dep_parts(info);
	if(part_mismatch) {
		printf("%d dep parts are mismatched!\n", part_mismatch);
		return -1;
	}

	/* Restore snapshot image into memory */
	switch(header->compress_algo) {
	case UNCOMPRESSED:
		/* Read metadata pages */
		nr_meta_pages = info->pages - info->image_pages - 1;
		meta_page_ptr = malloc(nr_meta_pages * PAGE_SIZE);
		if (meta_page_ptr == NULL) {
			printf("Can`t alloc buffer for metadata pages\n");
			printf("nr metadata pages : %d\n", nr_meta_pages);
			return -1;
		}

		if (storage_read(offset + PAGE_SIZE, nr_meta_pages * PAGE_SIZE, meta_page_ptr) < 0) {
			printf("Can't read snapshot image metadata\n");
			free(meta_page_ptr);
			return -1;
		}

		/* add header and metadata pages */
		offset_datapage = offset + ((1 + nr_meta_pages) * PAGE_SIZE);

		if (uncompressed_snapshot_image_restore(nr_meta_pages, offset_datapage) < 0) {
			free(meta_page_ptr);
			return -1;
		}
		break;

	case LZO:
		// Fall through
	case LZ4HC:
	case LZ4:
		/* Read metadata pages */
		meta_page_ptr = malloc(sizeof(struct pfn_merge_info) * header->pfn_mi_cnt);
		if (meta_page_ptr == NULL) {
			printf("Can`t alloc buffer for metadata pages\n");
			return -1;
		}

		if (storage_read(offset + header->metadata_start_offset_for_comp, sizeof(struct pfn_merge_info) * header->pfn_mi_cnt, meta_page_ptr) < 0) {
			printf("Can't read snapshot image metadata\n");
			free(meta_page_ptr);
			return -1;
		}

		/* add header */
		offset_datapage = offset + PAGE_SIZE;

		if (compressed_snapshot_image_restore(offset_datapage, verify, desc_print, decomp_print) < 0) {
			free(meta_page_ptr);
			return -1;
		}

		break;

	default:
		printf("Unsupported compress type\n");
		return -1;
	}

	/* Cleaning before exit bootloader */
	free(meta_page_ptr);

	printf("Welcome to snapshot world!\n");

	//wait_oneshot_timer();

	if(load_only)
		return 0;

	//display_boottime_log();

#if defined(CONFIG_MULTICORES_PLATFORM)
	release_non_boot_core();
#endif

	if(before_start_linux() < 0)
		return -1;

	kernel_resume_func = (void (*))header->pa_resume_func;
#ifdef CONFIG_SECURITY_BOOT
	wait_tee_ready();
#endif

	kernel_resume_func();

	return 0;
}

#ifdef CONFIG_USB_PL2303
extern struct usb_device *pPL2303Dev;
#endif
int before_start_linux(void)
{
	eth_halt();

#ifdef CONFIG_USB_PL2303
	pPL2303Dev = NULL;
#endif

	usb_stop();

	flush_cache_all();

	cleanup_before_linux();

	return 0;
}

int do_hib(void)
{
	int rc = -1;

	rc = snapshot_boot(1, 0, 0, 0, 0);

	return rc;
}

U_BOOT_CMD(
		hib,	1,	0,	do_hib,
		"hib\t- resume from hibernation image\n",
		"hib\n"
		);

u32 _crc32_le(u32 crc, unsigned char const *p, size_t len)
{
	int i;
	while (len--) {
		crc ^= *p++;
		for (i = 0; i < 8; i++)
			crc = (crc >> 1) ^ ((crc & 1) ? CRCPOLY_LE : 0);
	}
	return crc;
}

static void hib_image_info(void)
{
	struct swsusp_info si;
	storage_partition_t partition;

	/* Get partition which has snapshot boot image(hibernation image) */
	if (storage_get_partition(SNAP_PART_NAME, &partition) < 0) {
		printf("Invalid snapshot image partition\n");
		return -1;
	}
	/* Get header */
	if (storage_read(partition.offset, sizeof(struct swsusp_info),&si) < 0) {
		printf("Can't read snapshot image header\n");
		return -1;
	}
	print_snapshot_header(&si);
}

int do_snapshot (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	unsigned char snapshot;
	unsigned char snapshot_old;

	if(argc > 4) {
		printf ("Usage:\n%s\n", cmdtp->usage);
		printf ("%s\n", cmdtp->help);
		snapshot = DDI_NVM_GetSnapShot_Support();
		printf ("current snapshot mode = %s \n", (snapshot == 0xff) ? "on" : "off");
		return 1;
	}

	if( !strcmp(argv[1],"on") || !strcmp(argv[1],"off"))
	{
		snapshot_old = DDI_NVM_GetSnapShot_Support();
		printf ("snapshot mode is changed to %s from %s \n", argv[1], (snapshot_old == 0xff) ? "on" : "off");

		if(!strcmp(argv[1],"on"))
			DDI_NVM_SetSnapShot_Support('S');
		else
			DDI_NVM_SetSnapShot_Support(0);

	} else if( !strcmp(argv[1],"info") ) {
		printf(" !! Display snapshot header info ----------------\n");
		hib_image_info();
	} else if( !strcmp(argv[1],"remake") ) {
		remake_hib();
		printf("snapshot image has been removed\n");
	} else if( !strcmp(argv[1],"load") ) {
		if( argc > 2 ) {
			if( !strcmp(argv[2],"nolog") )
				snapshot_boot(1, 1, 0, 1, 0);
			else if( !strcmp(argv[2],"loop") ) {
				if( argc > 3 ) {
					int cnt = simple_strtoul(argv[3], NULL, NULL);
					int loop = cnt;
					int try = 0;
					while( 1 ) {
						snapshot_boot(1, 1, 0, 1, 0);
						if( cnt )
							if( --loop <= 0 )
								break;
#if defined(CONFIG_MULTICORES_PLATFORM)
						mtk_msleep(500);
#endif
						printf("Decompress test try cnt=%d\n",try++);
					}
				}
			} else
				snapshot_boot(1, 1, 1, 1, 1);
		} else
			snapshot_boot(1, 1, 1, 1, 1);
	} else if( !strcmp(argv[1],"single") ) {

		if( argc > 2 ) {
			if( !strcmp(argv[2],"on") ) {
				printf("current used %s , changed to single-core\n",use_single_core?"single-core":"multi-core");
				use_single_core = 1;
			} else if( !strcmp(argv[2],"off") ) {
				printf("current used %s , changed to multi-core\n",use_single_core?"single-core":"multi-core");
				use_single_core = 0;
			} else {
				printf("current used %s , changed to multi-core\n",use_single_core?"single-core":"multi-core");
				use_single_core = 0;
			}
		}
	} else if( !strcmp(argv[1],"art") ) {
		unsigned char art;
		if( argc > 2 ) {
			if( !strcmp(argv[2],"on") ) {
				DDI_NVM_SetSnapShotART('A');
			} else if( !strcmp(argv[2],"resume") ) {
				DDI_NVM_SetSnapShotART('R');
			} else if( !strcmp(argv[2],"making") ) {
				DDI_NVM_SetSnapShotART('M');
			} else {
				DDI_NVM_SetSnapShotART(0);
			}
		}
		art = DDI_NVM_GetSnapShotART();
		printf("Snapshot ART(Auto Rebooting Test)=%s\n",(art=='A')?"auto":(art=='R')?"resume":(art=='M')?"making":"off");
	}else{
		printf ("Usage:\n%s\n", cmdtp->usage);
		printf ("%s\n", cmdtp->help);
		snapshot = DDI_NVM_GetSnapShot_Support();
		printf ("current snapshot mode = %s \n", (snapshot == 0xff) ? "on" : "off");
		return 1;
	}

	return 0;
}

U_BOOT_CMD(
	snapshot,	  4,	  0,	  do_snapshot,
	"snapshot\t- set snapshot flag on/off at NVM\n",
	"snapshot [on|off]\n"
	"snapshot info -Display snapshot header\n"
	"snapshot load [nolog|loop [cnt]] -load snapshot image without jump to kernel, if cnt is 0 ,loading image infinitely\n"
	"snapshot art [on|off] - auto rebooting test\n"
);
