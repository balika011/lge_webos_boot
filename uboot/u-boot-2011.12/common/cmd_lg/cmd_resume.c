#include <common.h>
#include <command.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/ctype.h>
#include <exports.h>
#include <net.h>
#include <xyzModem.h>
#include <environment.h>
#include <mmc.h>
#include <partinfo.h>
#include "cmd_polltimer.h"

#include <lg_modeldef.h>
#include <cmd_resume.h>
#ifdef SIGN_USE_PARTIAL
#include "x_ldr_env.h"
#endif

DECLARE_GLOBAL_DATA_PTR;

/* Cofigurable variable */

#define PAGE_SIZE	(4096)
#define PAGE_SHIFT	(12)

#define DMA_UNIT_SIZE	DMA_READ_BUFFER_SIZE

#define	USE_ASYNC_SHA1

struct swsusp_info *header;
struct swsusp_runtime_val swsusp_resume_val;
extern void DDI_NVM_SetMakeHib( unsigned char mode );
extern unsigned char DDI_NVM_GetSnapShot_Support( void );
extern void DDI_NVM_SetSnapShot_Support( unsigned char mode );
extern int storage_read(uint32_t offset, size_t len, void* buf);
extern int storage_erase(unsigned long long offset, unsigned long long len);
extern unsigned short __InvertOn;


struct dma_control {
	unsigned int full_len; /* total length */
	unsigned int send_pos; /* DMA request sent */
	unsigned int recv_pos; /* DMA completed */
	unsigned int used_pos; /* decompressed */
};





static void dump_dma_control(struct dma_control *dmac)
{
	printf("full_len: %d\nsend_pos: %d\nrecv_pos: %d\nused_pos: %d\n",
	       dmac->full_len, dmac->send_pos, dmac->recv_pos, dmac->used_pos);
}

//#define ALIGN(n, a)  (((n) + ((a)-1)) / (a) * (a))
#define ALIGN(n, a)  		__ALIGN_MASK(n, (a)-1)
#define __ALISH_MASK(n, mask)	(((n) + (mask)) & ~(mask))


void remake_hib(void)
{
    struct partition_info *partition  = NULL;
    int idx  = -1;
    DDI_NVM_SetMakeHib(1);
    idx = get_partition_idx(HIB_PART_NAME);
    if(0 < idx)
    {
        partition = GET_PART_INFO(idx);
    }
    if(NULL != partition)
    {
        storage_erase(partition->offset, 0x200);
    }
}



static int dma_read(struct dma_control *dmac, int force)
{
        int ret = 0;
	
	if (dmac->recv_pos >= dmac->full_len)
		return 1;

	/*
	 * we need at least DMA_UNIT_SIZE buffer available to decompress
	 * otherwise request DMA read.
	 */
	if (dmac->used_pos + DMA_UNIT_SIZE > dmac->recv_pos ||
	    force)
	{
		unsigned int len;

		len = dmac->send_pos - dmac->recv_pos;
		if (len > DMA_UNIT_SIZE) {
			printf("DMA control data is corrupted!\n");
			dump_dma_control(dmac);
			return -1;
		}

		/* Dma is requested already, wait to complete */
		if (len) {
			ret = emmc_async_dma_wait_finish();
			if(ret) {
                printf("[dma_read]emmc_async_dma_wait_finish done %d\n", ret);		
                return 3;			
            }

#ifdef DEBUG_DMA
			printf("read %d bytes\n", len);
#endif
			dmac->recv_pos += len;
		}

		/* request at most DMA_UNIT_SIZE bytes */
		len = DMA_UNIT_SIZE;
		if (dmac->send_pos > dmac->full_len - len) {
			len = dmac->full_len - dmac->send_pos;
		}

		if (len == 0) {
			printf("DMA finished!\n");
			return 1;
		}

		/* request more data to DMA */
		emmc_async_dma_start_trigger(len);
		dmac->send_pos += len;

#ifdef DEBUG_DMA
		printf("request %d bytes\n", len);
#endif
		/*
		 * NOTE: This reduces memory bus contention so that
		 * the overall performance might be improved.
		 * (around 100ms on my board)
		 */
		udelay(300);
	}

	return 0;
}

int verify_snapshot(const char*partname, ulong length)
{
    unsigned char hash_value[SB_HASH_SIZE];
    unsigned int hash_size;
    unsigned long sb_offset;
    struct {
        unsigned char aes_key[SB_AES_KEY_SIZE];
        unsigned char hash_value[SB_HASH_SIZE];
        unsigned char unused[512 - SB_AES_KEY_SIZE - SB_HASH_SIZE];
    } saved;
    int result;
	char *xip_argv[3] = {"xip", "lgapp", "0"};

    /* calculate hash value for entire image in memory (including header) */
    memset(hash_value, 0, sizeof(hash_value));

	#ifdef USE_ASYNC_SHA1
	printf("---[%d] async SHA1...\n", readMsTicks());
	SHA1_Onece_Async_Start(IMAGE_LOAD_BUFFER_ADDR, length, hash_value, &hash_size);
	printf("---[%d] async SHA1 started\n", readMsTicks());

	if(do_xiplz4(find_cmd("xip"), 0, 3, xip_argv))
	{
		printf("do_xip fail\n");
		return -1;
	}

	SHA1_Onece_Async_Stop(IMAGE_LOAD_BUFFER_ADDR, length, hash_value, &hash_size);
	printf("---[%d] async SHA1 finish\n", readMsTicks());

	if(do_verify_xip("lgapp"))
	{
		printf("do_verify_xip fail\n");
		return -1;
	}
	#else
    SHA1_Onece(IMAGE_LOAD_BUFFER_ADDR, length, hash_value, &hash_size);
	#endif

    if (hash_size != 20) { /* 160-bit SHA1 hash */
	    printf("SECURE BOOT: hash size is different (%d)\n", hash_size);
	    return 0;
    }

    /* cur_meta_floffset points to the first meta page after the header page */
    sb_offset = swsusp_resume_val.cur_meta_floffset - PAGE_SIZE + SB_AES_KEY_OFFSET;
    /* read AES key and hash value from flash */
    result = storage_read(sb_offset, sizeof(saved), &saved);
    if (result) {
        printf("storage_read for AES key failed (%d)\n", result);
		return 0;
    }

    /* compare hash value with saved (precalculated) value */
    result = DMX_Decrypt_Snapshot(saved.hash_value, SB_HASH_SIZE, hash_value,
				  saved.aes_key, SB_AES_KEY_SIZE);

    printf("SECURE BOOT: verification result: %s\n", result ? "SUCCESS" : "FAILURE");
    return result;
}

int page_restore(void)
{
	unsigned int *meta_buf, *meta_buf_ptr;
	char *data_buf, *data_buf_ptr_cur, *data_buf_ptr_next;
	int ret, size = PAGE_SIZE, meta_buf_index;
	ulong crc = 0, crc_size = PAGE_SIZE;
	void *dst;
	int comp_block_size, decompressed_size;

	unsigned int s_mtime, e_mtime;

	struct dma_control dmac = { ALIGN(header->compressed_image_size, 4096), };
	polling_timer();
	dump_dma_control(&dmac);
	
	// Set metadata pointer (after header page)
	meta_buf = (unsigned int *)(IMAGE_LOAD_BUFFER_ADDR + PAGE_SIZE);

	s_mtime = readMsTicks();

#ifdef USE_DMA
	ret = emmc_async_read_setup(0, swsusp_resume_val.cur_meta_floffset,
			      ALIGN(header->compressed_image_size, 4096), meta_buf);

	polling_timer();
  	if (ret)
  	{		
    	printf("[page_store]emmc_async_read_setup done %d\n", ret);		
		return 2;	
  	}
	
	/* request 2 * DMA_UNIT_SIZE data in advance */
	ret = dma_read(&dmac, 1);
	if(ret == 3)
  	{
    	printf("[page_store]dma read failed %d\n", ret);
    	return ret;
  	}
	
	ret = dma_read(&dmac, 1);
	if(ret == 3)
  	{
    	printf("[page_store]dma read failed %d\n", ret);
    	return ret;
  	}

	/* wait for all meta pages ready */
	while (dmac.recv_pos < swsusp_resume_val.total_meta_pages * PAGE_SIZE)
	{
		ret = dma_read(&dmac, 1);
		if(ret == 3)
		{
      		printf("[page_store]dma read failed %d\n", ret);
			return ret;
		}
	}

	dmac.used_pos = swsusp_resume_val.total_meta_pages * PAGE_SIZE;
	polling_timer();
#else
	// Whole snapshot image read
	polling_timer();
	ret = storage_read((uint32_t)swsusp_resume_val.cur_meta_floffset,
			   (size_t)ALIGN(header->compressed_image_size, 4096), (char *)meta_buf);
	if(ret)
		goto storage_read_fail;

	e_mtime = readMsTicks();

	printf("snapshot image read complete, size = %dkbytes\n", (header->compressed_image_size) >> 10);
	printf("Elapsed time = %ums\n", e_mtime - s_mtime);

	s_mtime = readMsTicks();
	polling_timer();
#endif
	// Check read meta data
#ifdef CRC_CHECK
	meta_buf_ptr = meta_buf;
	while(swsusp_resume_val.left_meta_pages)
	{
		crc = _crc32_le(crc, (const uchar *)meta_buf_ptr, crc_size);
		swsusp_resume_val.left_meta_pages--;
		meta_buf_ptr += 1024;
	}
#else
	swsusp_resume_val.left_meta_pages = 0;
#endif

	// Restore data pages
#ifdef USE_COMPRESS
	/* sequential read and decompress routine */
	data_buf = (char *)meta_buf + (PAGE_SIZE * swsusp_resume_val.total_meta_pages);

	printf("getting started to read data pages using DMA\n");

	meta_buf_index = 0;
	while(swsusp_resume_val.left_data_pages)
	{
#ifdef USE_DMA
		/* ensure that we have space for block size */
		ret = dma_read(&dmac, dmac.used_pos + 4 > dmac.recv_pos);
    	if(ret == 3)
    	{
      		printf("[page_store]dma read failed %d\n", ret);
      		return ret;
    	}
#endif
		// Get each compressed block size
		comp_block_size = *((int*)data_buf);
		data_buf += sizeof(int);
		dmac.used_pos += sizeof(int);
		//polling_timer();
#ifdef USE_DMA
		/* ensure that we have actual data block */
		ret = dma_read(&dmac, dmac.used_pos + comp_block_size > dmac.recv_pos);
    	if(ret == 3)
    	{
      		printf("[page_store]dma read failed %d\n", ret);
      		return ret;
    	}
		/* Paranoid check */
		while (dmac.used_pos + comp_block_size > dmac.recv_pos) {
#ifdef DEBUG_DMA
			printf("some delay might be needed here...\n");
#endif
			ret = dma_read(&dmac, 1);
      		if(ret == 3)
      		{
        			printf("[page_store]dma read failed %d\n", ret);
        			return ret;
      		}
		}
#endif
		// decompress page block to original page region
		dst = (void *)(meta_buf[meta_buf_index] << PAGE_SHIFT);

		//printf("decompressing %d bytes ... ", comp_block_size);
		//printf ("purplearrow, lzo1x_decompress is not implement\n");
		lzo1x_decompress(data_buf, comp_block_size,  dst, &decompressed_size, NULL);
		if(decompressed_size != PAGE_SIZE)
		{
			e_mtime = readMsTicks();
			printf("%s[%d] : Something goes wrong, decompressed block size is not match with PAGE_SIZE (%d)\n",
			       __func__, __LINE__, decompressed_size);
#ifdef USE_DMA
			dump_dma_control(&dmac);
			ret = emmc_async_read_finish(0);
			if (ret)
				printf("emmc_async_read_finish done %d\n", ret);
#endif
			printf("snapshot image decomp and restore failed, Elapsed time = %ums\n", e_mtime - s_mtime);

			return 0;
		}
		//printf("done\n");

		data_buf += comp_block_size;
		dmac.used_pos += comp_block_size;
		polling_timer();
#ifdef CRC_CHECK
		crc = _crc32_le(crc, (const uchar *)(meta_buf[meta_buf_index] << PAGE_SHIFT), crc_size);
#endif
		// update read states
		swsusp_resume_val.left_data_pages--;
		meta_buf_index++;
	}
#else // else of USE_COMPRESS
	data_buf = (char *)meta_buf +  (PAGE_SIZE * swsusp_resume_val.total_meta_pages);

	meta_buf_index = 0;
	while(swsusp_resume_val.left_data_pages)
	{
		dst = (void *)(meta_buf[meta_buf_index] << PAGE_SHIFT);
		memcpy(dst, (void *)data_buf, PAGE_SIZE);
		data_buf += PAGE_SIZE;

		#ifdef CRC_CHECK
		crc = _crc32_le(crc, (const uchar *)(meta_buf[meta_buf_index] << PAGE_SHIFT), crc_size);
		#endif
		swsusp_resume_val.left_data_pages--;
		meta_buf_index++;
	}
#endif // end of USE_COMPRESS

	e_mtime = readMsTicks();

#ifdef USE_DMA
	ret = emmc_async_read_finish(0);
  	if (ret)
    	printf("emmc_async_read_finish done %d\n", ret);
	dump_dma_control(&dmac);
#endif
	printf("snapshot image decomp and restore complete, Elapsed time = %ums\n", e_mtime - s_mtime);

#ifdef SECURE_BOOT
	/*
	 * XXX: Ensure that next storage_read() not to fail...
	 *      Maybe we can fix this by placing the signature right after the image
	 *      so that the DMA pass can read it all at once.
	 */
	polling_timer();

	udelay(1000);

	s_mtime = readMsTicks();
	comp_block_size = ALIGN(header->compressed_image_size, PAGE_SIZE) + PAGE_SIZE;
	if (!verify_snapshot(HIB_PART_NAME, comp_block_size)) {
		gd->flags &= ~GD_FLG_SILENT;
		printf("SECURE BOOT: verification failed: halt...\n");
//		memset(IMAGE_LOAD_BUFFER_ADDR, 0, comp_block_size);
//		writeFullVerifyOTP();

		if(DDI_NVM_GetDebugStatus() == RELEASE_LEVEL)	{
			char cmd[] = "reset";
			remake_hib();
			run_command(cmd, 0);
		}
		else
			while (1); /* halt */

	}
	e_mtime = readMsTicks();
	polling_timer();
	printf("SECURE BOOT: snapshot image verification done, Elapsed time = %ums\n", e_mtime - s_mtime);
#endif

#ifdef CRC_CHECK
	printf("%s[%d] : crc = %#x\n", __func__, __LINE__, crc);
#endif
	polling_timer();
	return 1;

storage_read_fail:
	printf("Storage read fail\n");
	return 0;
}

int check_snapshot_mode(void)
{
#if 1
	char *bootmode;
	char *snapshot;

	bootmode = getenv("bootmode");
	if(!strcmp(bootmode, "nfs") || (DDI_NVM_GetDebugStatus() == DEBUG_LEVEL) 
	   || DDI_NVM_GetSWUMode() || (DDI_NVM_GetSnapShot_Support() == 0) )
		return LGSNAP_COLDBOOT;
	else if( DDI_NVM_GetMakeHib() )
		return LGSNAP_MAKING_IMAGE;
	else
		return LGSNAP_SNAPSHOTBOOT;
#else
	return LGSNAP_COLDBOOT;
#endif
}

#ifdef CC_TRUSTZONE_SUPPORT
extern void init_tz(void);
#endif
#ifdef SIGN_USE_PARTIAL	
extern int getFullVerifyOTP(void);
extern ulong appxip_len;
#endif
int do_hib(void)
{
	struct partition_info	*mpi;
	int ret, *in_suspend, ret1;
	extern void __restore_processor_state(unsigned long *, unsigned long *);
	//char *xip_argv[2] = {"xip", "lgapp"};
	char* pargs[3] = {0,"tzfw","0x200000"};
	polling_timer();
	//BootSplash();
// verify tzfw partition  
//	do_cp2ram (NULL, 0, 3, pargs);
//	do_verification(NULL, 0, 4, pargs);
#ifdef SIGN_USE_PARTIAL
	LDR_ENV_T* prLdrEnv = (LDR_ENV_T*)0xfb005000;
#endif	
	//BootSplash();
	mpi	= get_used_partition(HIB_PART_NAME);
	if(!mpi) {
		printf("mpi = NULL, no hibernation partition\n");
		return -1;
	}

	ret = read_swsusp_header(mpi);
	if(ret == -1)
	{
		printf("No hibernation image\n");
		return -1;
	}

#ifndef	USE_ASYNC_SHA1
	// load xip Image
//	if(do_xipz(find_cmd("xip"), 0, 2, xip_argv))
	if(do_xiplz4(find_cmd("xip"), 0, 2, xip_argv))
	{
		printf("do_xip fail\n");
		return -1;
	}
#endif

	polling_timer();
	// kernel page restore
	//BOOTLOG("page_restore() --->\n");
	ret = page_restore();
	//BOOTLOG("page_restore() <---\n");
  	if ((ret == 2) || (ret == 3))
  	{
    	do
    	{
      		if(ret == 2)              
        		ret1 = emmc_async_read_stop(0, 0);
      		else if(ret == 3) 
        		ret1 = emmc_async_read_stop(0, 1);

      		if(ret1 != -1)
      		{
        		swsusp_resume_val.left_data_pages = swsusp_resume_val.total_data_pages;
        		swsusp_resume_val.left_meta_pages = swsusp_resume_val.total_meta_pages;
				//BOOTLOG("page_restore() --->\n");
        		ret = page_restore();
				//BOOTLOG("page_restore() <---\n");
      		}
			
    	}while((ret1 != -1) && (ret != 1));
		polling_timer();
		
  	}

	if(ret != 1)
	{
		printf("page restore fail\n");
		return -1;
	}	

	while(1) //InvertOn check loop
	{
		if(!__InvertOn) // __InvertOn 0 : off , 1 : on
		{
			printf("Check Invert On/Off %d \n", __InvertOn);
			// if __InvertOn is 0, call BootSplash() to trun Invert on
			// BootSplash() will set __InvertOn
			BootSplash();
		}
		else
		{
			break;
		}
	}
#ifdef CC_TRUSTZONE_SUPPORT
	init_tz();
#endif

#ifdef SIGN_USE_PARTIAL	
		//copy pub key to mem		
		memcpy((void *)0x4000000+appxip_len-sizeof(prLdrEnv->au4CustKey) /*tail of appxip*/, (void *)prLdrEnv->au4CustKey, sizeof(prLdrEnv->au4CustKey));
#endif
	// Reset in_suspend
	in_suspend = (int *)(header->in_suspend_paddr);
	*in_suspend = 0;

	// restore processor and coprocessor state
	//gd->flags &= ~GD_FLG_SILENT;

	printf("\n[%d] Restore processor state and jump to kernel\n", readMsTicks());
	//PrintLateBuffer();
//	BOOTLOG("Restore processor state and jump to kernel\n");
//	PrintBootBuffer();
//	udelay(10000000000000);
	printf ("purplearrow, __restore_processor_state is not implemented\n");
	//__restore_processor_state(header->resume_func_vaddr, header->saved_context_paddr);

	// never returns
	return ret;
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

void print_swsusp_resume_val(void)
{
	printf("========swsusp resume runtime val============\n");
	printf("total_image_pages = %d\n", swsusp_resume_val.total_image_pages);
	printf("total_data_pages = %d\n", swsusp_resume_val.total_data_pages);
	printf("total_meta_pages = %d\n", swsusp_resume_val.total_meta_pages);
	printf("left_data_pages = %d\n", swsusp_resume_val.left_data_pages);
	printf("left_meta_pages = %d\n", swsusp_resume_val.left_meta_pages);
	printf("cur_meta_floffset = %x\n", swsusp_resume_val.cur_meta_floffset);
	printf("========swsusp resume runtime val============\n");
}


int read_swsusp_header(struct partition_info *mpi)
{
	int ret;
	unsigned int floffset;
	unsigned int size;
	char *buf = (char *)IMAGE_LOAD_BUFFER_ADDR;

	polling_timer();
	if(buf == NULL)
	{
		printf("%s[%d] : fail alloc swsusp_info buffer\n", __func__, __LINE__);
		return -1;
	}

	floffset = (ulong)(mpi->offset + CFG_FLASH_BASE);
	size = PAGE_SIZE;

	// Read swsusp_info header
	ret = storage_read((uint32_t)floffset, (size_t)size, buf);
	if(ret) {
		printf("%s[%d] : storage read error.. \n", __func__, __LINE__);
		return -1;
	}

	header = (struct swsusp_info *)buf;

	printf("swsusp info read complete\n");
	printf("swsusp_info{\n");
	printf("version code = %#x\n", header->version_code);
	printf("num_physpages = %u\n", header->num_physpages);
	printf("cpus = %d\n", header->cpus);
	printf("image_pages = %u\n", header->image_pages);
	printf("pages = %u\n", header->pages);
	printf("size = %u\n", header->size);
	printf("}swsusp_info\n");

	if(header->version_code != 0xaceace)
	{
		return -1;
	}

	// Init runtime structure
	swsusp_resume_val.total_image_pages = header->pages - 1; // the first page is swsusp_header
	swsusp_resume_val.total_data_pages = header->image_pages;
	swsusp_resume_val.total_meta_pages = swsusp_resume_val.total_image_pages - swsusp_resume_val.total_data_pages;
	swsusp_resume_val.left_data_pages = swsusp_resume_val.total_data_pages;
	swsusp_resume_val.left_meta_pages = swsusp_resume_val.total_meta_pages;
	swsusp_resume_val.cur_meta_floffset = floffset + PAGE_SIZE;
	polling_timer();
	print_swsusp_resume_val();
	polling_timer();

	return ret;
}

int do_snapshot (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	unsigned char snapshot;
	unsigned char snapshot_old;

	if(argc != 2) {
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
	snapshot,	  2,	  0,	  do_snapshot,
	"snapshot\t- set snapshot flag on/off at NVM\n",
	"snapshot [on|off]\n"
);
