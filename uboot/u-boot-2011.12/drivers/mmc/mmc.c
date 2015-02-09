/*
 * Copyright 2008, Freescale Semiconductor, Inc
 * Andy Fleming
 *
 * Based vaguely on the Linux code
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <config.h>
#include <common.h>
#include <command.h>
#include <mmc.h>
#include <part.h>
#include <malloc.h>
#include <linux/list.h>
#include <div64.h>
#include <jffs2/load_kernel.h>

#if defined(CC_EMMC_BOOT)

typedef struct mmc_async_operation_s
{
	uint total_sz;
  uint cur_off;
  uint cur_pos;
  uint cur_bytes;
#ifdef CC_MTD_ENCRYPT_SUPPORT
#define ATTR_ENCRYPTION    (0x1<<0)
  uint part_attr; 
#endif 
  uint fgdone;
  
  struct mmc_cmd cur_cmd;
  struct mmc_data cur_data;	
}mmc_async_operation_t;

mmc_async_operation_t mmc_async;
struct mmc emmc_info[CFG_MAX_EMMC_DEVICE];
static char *_pu1BlkBuf = NULL;

#ifdef CC_PARTITION_WP_SUPPORT
extern uint wp_config;
#endif
static struct list_head mmc_devices;
static int cur_dev_num = -1;
static char EXT_CSD[512];
static int i4mmcInit = 0;
static uint fgDisWP = 0;

extern int dataMode;
#if defined(CC_MT5399) || defined(CONFIG_ARCH_MT5399) || \
    defined(CC_MT5890) || defined(CONFIG_ARCH_MT5890) || \
    defined(CC_MT5882) || defined(CONFIG_ARCH_MT5882) 
extern void MsdcSetSpeedMode(uint speedMode);
#endif
extern void MsdcDescriptorConfig(void *pBuff, uint u4BufLen);
extern int MsdcWaitHostIdle(uint timeout);
extern void MsdcFindDev(uint *pCID);
extern void MsdcSetSampleEdge(uint fgWhere);
extern int MsdcDMAStart(uint cur_pos, uint cur_off, uint bytes, uint total_sz);
extern int MsdcDMAWaitIntr(struct mmc_cmd *cmd, uint cur_off, uint bytes, uint total_sz);
extern int MsdcDMATernmial(struct mmc *mmc, struct mmc_cmd *cmd, struct mmc_data *data);
extern int MsdcReqCmdStart(struct mmc *mmc, struct mmc_cmd *cmd, struct mmc_data *data);
#ifdef CC_PARTITION_WP_SUPPORT
extern uint MsdcPartitionWp(uint blkindx);
extern uint MsdcPartitionWpConfig(uint wp);
#endif
#ifdef CC_MTD_ENCRYPT_SUPPORT
extern uint MsdcPartitionEncrypted(uint blkindx);
extern int MsdcAesEncryption(uint buff, uint len);
extern int MsdcAesDecryption(uint buff, uint len);
#endif

int __board_mmc_getcd(u8 *cd, struct mmc *mmc) {
	return -1;
}

int board_mmc_getcd(u8 *cd, struct mmc *mmc)__attribute__((weak,
	alias("__board_mmc_getcd")));

int mmc_send_cmd(struct mmc *mmc, struct mmc_cmd *cmd, struct mmc_data *data)
{
	return mmc->send_cmd(mmc, cmd, data);
}

int mmc_set_blocklen(struct mmc *mmc, int len)
{
	struct mmc_cmd cmd;

	cmd.cmdidx = MMC_CMD_SET_BLOCKLEN;
	cmd.resp_type = MMC_RSP_R1;
	cmd.cmdarg = len;
	cmd.flags = 0;

	return mmc_send_cmd(mmc, &cmd, NULL);
}

struct mmc *find_mmc_device(int dev_num)
{
	struct mmc *m;
	struct list_head *entry;

	list_for_each(entry, &mmc_devices) {
		m = list_entry(entry, struct mmc, link);

		if (m->block_dev.dev == dev_num)
			return m;
	}

	printf("MMC Device %d not found\n", dev_num);

	return NULL;
}

static ulong
mmc_write_blocks(struct mmc *mmc, ulong start, lbaint_t blkcnt, const void*src)
{
	struct mmc_cmd cmd;
	struct mmc_data data;
	uint ret;

	if ((start + blkcnt) > mmc->block_dev.lba) {
		printf("MMC: block number 0x%lx exceeds max(0x%lx)\n",
			(ulong)(start + blkcnt), (ulong)(mmc->block_dev.lba));
		return 0;
	}

	if (blkcnt > 1)
		cmd.cmdidx = MMC_CMD_WRITE_MULTIPLE_BLOCK;
	else
		cmd.cmdidx = MMC_CMD_WRITE_SINGLE_BLOCK;

	if (mmc->high_capacity)
		cmd.cmdarg = start;
	else
		cmd.cmdarg = start * mmc->write_bl_len;

	cmd.resp_type = MMC_RSP_R1;
	cmd.flags = 0;
	cmd.flags |= (dataMode & 0x0F);

	data.src = src;
	data.blocks = blkcnt;
	data.blocksize = mmc->write_bl_len;
	data.flags = MMC_DATA_WRITE;
#ifdef CC_PARTITION_WP_SUPPORT
if(MsdcPartitionWp(start)&&(wp_config))
{
   printf(" partition was  write protected!\n");      
   return 0;
}
#endif
#ifdef CC_MTD_ENCRYPT_SUPPORT
  if(MsdcPartitionEncrypted(start))
  {
    if(MsdcAesEncryption((uint)src, (uint)(blkcnt*mmc->write_bl_len)))
    {
      printf("aes encryption to buffer(addr:0x%08X len:0x%08X) before write process failed!\n", (uint)src, (uint)(blkcnt*mmc->write_bl_len));	
      //return 0;
    }   	
  }
#endif
  
	if(data.blocks > 1)
		cmd.flags |= (0x01<<4);

    /* dma mode cache flush */
    if((cmd.flags & 0x0F) > 1)
        HalFlushDCacheMultipleLine((uint)(src), (uint)(blkcnt*mmc->write_bl_len));
  
    /* descriptor dma mode configuration */
	if((cmd.flags & 0x0F) == 3)
        MsdcDescriptorConfig((void *)(src), (uint)(blkcnt*mmc->write_bl_len));

    ret = mmc_send_cmd(mmc, &cmd, &data);
    if(ret)
    {
        printf("mmc write failed\n");
		return 0;
    }

	if ((blkcnt > 1)&&(!(cmd.flags>>4))) {
		cmd.cmdidx = MMC_CMD_STOP_TRANSMISSION;
		cmd.cmdarg = 0;
		cmd.resp_type = MMC_RSP_R1b;
		cmd.flags = 0;
		if (mmc_send_cmd(mmc, &cmd, NULL)) {
			printf("mmc fail to send stop cmd\n");
			return 0;
		}
	}

	//HalFlushInvalidateDCacheMultipleLine((uint)(start*mmc->write_bl_len), (uint)(blkcnt*mmc->write_bl_len));

	return blkcnt;
}

static uint get_wp_flag(void)
{
	return fgDisWP;
}

void set_wp_flag(uint flag)
{
	fgDisWP = flag;
}

int disable_wp(int dev_num)
{
	struct mmc *mmc = find_mmc_device(dev_num);
	if (!mmc)
		return 0;
	
	if(get_wp_flag() == 0)
	{
        if(0 == mmc_wp(mmc, 0, 0, 0, 0))
        {
            printf("Disable WP Success!\n");
			set_wp_flag(1);
        }
		else
		{
            printf("Disable WP failed!\n");
			return 0;
		}
	}
	return 1;	
}

static ulong
mmc_bwrite(int dev_num, ulong start, lbaint_t blkcnt, const void *src)
{
	lbaint_t cur, blocks_todo = blkcnt;

	struct mmc *mmc = find_mmc_device(dev_num);
	if (!mmc)
		return 0;

	//if (mmc_set_blocklen(mmc, mmc->write_bl_len))
		//return 0;

	do {
		/*
		 * The 65535 constraint comes from some hardware has
		 * only 16 bit width block number counter
		 */
		cur = (blocks_todo > 65535) ? 65535 : blocks_todo;
		if(mmc_write_blocks(mmc, start, cur, src) != cur)
			return 0;
		blocks_todo -= cur;
		start += cur;
		src += cur * mmc->write_bl_len;
	} while (blocks_todo > 0);

	return blkcnt;
}

int mmc_read_blocks(struct mmc *mmc, void *dst, ulong start, lbaint_t blkcnt)
{
	struct mmc_cmd cmd;
	struct mmc_data data;
	uint ret;

	if (blkcnt > 1)
		cmd.cmdidx = MMC_CMD_READ_MULTIPLE_BLOCK;
	else
		cmd.cmdidx = MMC_CMD_READ_SINGLE_BLOCK;

	if (mmc->high_capacity)
		cmd.cmdarg = start;
	else
		cmd.cmdarg = (start * mmc->read_bl_len);

	cmd.resp_type = MMC_RSP_R1;
	cmd.flags = 0;
	cmd.flags |= (dataMode & 0x0F);

	data.dest = dst;
	data.blocks = blkcnt;
	data.blocksize = mmc->read_bl_len;
	data.flags = MMC_DATA_READ;

  if(data.blocks > 1)
    cmd.flags |= (0x01<<4);

  /* dma mode cache flush */
  if((cmd.flags & 0x0F) > 1)
    HalFlushInvalidateDCacheMultipleLine((uint)(dst), (uint)(blkcnt*mmc->read_bl_len));
  
  /* descriptor dma mode configuration */
  if((cmd.flags & 0x0F) == 3)
    MsdcDescriptorConfig((void *)(dst), (uint)(blkcnt*mmc->read_bl_len));

  ret = mmc_send_cmd(mmc, &cmd, &data);
  if(ret)
  {
		printf("mmc read failed\n");
		return 0;
	}

	if ((blkcnt > 1)&&(!(cmd.flags>>4))) {
		cmd.cmdidx = MMC_CMD_STOP_TRANSMISSION;
		cmd.cmdarg = 0;
		cmd.resp_type = MMC_RSP_R1b;
		cmd.flags = 0;
		if (mmc_send_cmd(mmc, &cmd, NULL)) {
			printf("mmc fail to send stop cmd\n");
			return 0;
		}
	}

  if((cmd.flags & 0x0F) > 1)
    HalInvalidateDCacheMultipleLine((uint)(dst), (uint)(blkcnt*mmc->read_bl_len));
    
#ifdef CC_MTD_ENCRYPT_SUPPORT
  if(MsdcPartitionEncrypted(start))
  {
    if(MsdcAesDecryption((uint)dst, (uint)(blkcnt*mmc->write_bl_len)))
    {
      printf("aes encryption to buffer(addr:0x%08X len:0x%08X) before write process failed!\n", (uint)dst, (uint)(blkcnt*mmc->write_bl_len));	
      //return 0;
    }   	
  }
#endif

	return blkcnt;
}

typedef  int (*pemmc_user_callback)(int,int);
pemmc_user_callback pemmc_user_callback_internal = NULL;;

 void emmc_register_callback(pemmc_user_callback pemmc_user_callback_external)
{
	pemmc_user_callback_internal = pemmc_user_callback_external;
}

static ulong mmc_bread(int dev_num, ulong start, lbaint_t blkcnt, void *dst)
{
	lbaint_t cur, blocks_todo = blkcnt;

	if (blkcnt == 0)
		return 0;

	struct mmc *mmc = find_mmc_device(dev_num);
	if (!mmc)
		return 0;

	if ((start + blkcnt) > mmc->block_dev.lba) {
		printf("MMC: block number 0x%lx exceeds max(0x%lx)\n",
			(ulong)(start + blkcnt), (ulong)(mmc->block_dev.lba));
		return 0;
	}

	//if (mmc_set_blocklen(mmc, mmc->read_bl_len))
		//return 0;

	do {
		/*
		 * The 65535 constraint comes from some hardware has
		 * only 16 bit width block number counter
		 */
		cur = (blocks_todo > 8191) ? 8191 : blocks_todo;
		if(mmc_read_blocks(mmc, dst, start, cur) != cur)
		{
		    printf("mmc_read_blocks read error!\n");
			return 0;
		}
		if(NULL != pemmc_user_callback_internal)
		{ 
			int err = 0;
			if(cur != 0)
			{
				err = 0;
			}
			else
			{
				err = -1;
			}
			pemmc_user_callback_internal(err,cur*512);
			//printf("+++++++++++++++++g_read_bytes(0x%x)+++++++++++++++\n",pemmc_user_callback_internal(err,blkcnt*512));
		}	
		
		blocks_todo -= cur;
		start += cur;
		dst += cur * mmc->read_bl_len;
	} while (blocks_todo > 0);

	return blkcnt;
}

static ulong mmc_bytewrite(int dev_num, u64 bytestart, ulong bytecnt, const void *dst)
{
	u64 blkoffset;
	ulong restsize = bytecnt, offsetinblk;
	ulong memptr = (ulong)dst;

	struct mmc *mmc = find_mmc_device(dev_num);
	if (!mmc)
	{
		return 0;
	}

	if (!_pu1BlkBuf)
	{
		_pu1BlkBuf = (char *)malloc(mmc->write_bl_len + 0x1F);
		_pu1BlkBuf = (char *)((((uint)(_pu1BlkBuf))&(~0x1F)) + 0x20);
	}


	offsetinblk = bytestart & (mmc->write_bl_len - 1);
	blkoffset = bytestart - offsetinblk;

	// Handle offset non-block-size-aligned case
	if (offsetinblk)
	{
		ulong opsize;

		if (mmc_bread(dev_num, blkoffset/mmc->write_bl_len, 1, _pu1BlkBuf) != 1)
		{
			return 0;
		}

		if (restsize >= (mmc->write_bl_len - offsetinblk))
		{
			opsize = mmc->write_bl_len - offsetinblk;
		}
		else
		{
			opsize = restsize;
		}

		memcpy((void *)((char *)_pu1BlkBuf + offsetinblk), (void *)memptr, opsize);

		if (mmc_bwrite(dev_num, blkoffset/mmc->write_bl_len, 1, _pu1BlkBuf) != 1)
		{
			return 0;
		}

		restsize  -= opsize;
		memptr    += opsize;
		blkoffset += mmc->write_bl_len;
	}

	// Handle offset & size block-size-aligned case
	if (restsize >= mmc->read_bl_len)
	{
		ulong blkcnt = restsize/mmc->write_bl_len;

		if (mmc_bwrite(dev_num, blkoffset/mmc->write_bl_len, blkcnt, (void *)memptr) != blkcnt)
		{
			return 0;
		}

		restsize  -= (blkcnt * mmc->write_bl_len);
		memptr    += (blkcnt * mmc->write_bl_len);
		blkoffset += (blkcnt * mmc->write_bl_len);
	}

	// Handle size none-block-size-aligned case
	if (restsize)
	{
		if (mmc_bread(dev_num, blkoffset/mmc->write_bl_len, 1, _pu1BlkBuf) != 1)
		{
			return 0;
		}

		memcpy((void *)_pu1BlkBuf, (void *)memptr, restsize);

		if (mmc_bwrite(dev_num, blkoffset/mmc->write_bl_len, 1, _pu1BlkBuf) != 1)
		{
			return 0;
		}
	}

	return bytecnt;
}

static ulong mmc_byteread(int dev_num, u64 bytestart, ulong bytecnt, void *dst)
{
	u64 blkoffset;
	ulong restsize = bytecnt, offsetinblk;
	ulong memptr = (ulong)dst;

	struct mmc *mmc = find_mmc_device(dev_num);
	if (!mmc)
	{
		return 0;
	}

	if (!_pu1BlkBuf)
	{
		_pu1BlkBuf = (char *)malloc(mmc->read_bl_len + 0x1F);
		_pu1BlkBuf = (char *)((((uint)(_pu1BlkBuf))&(~0x1F)) + 0x20);
	}

	offsetinblk = bytestart & (mmc->read_bl_len - 1);
	blkoffset = bytestart - offsetinblk;

	// Handle offset non-block-size-aligned case
	if (offsetinblk)
	{
		ulong opsize;

		if (mmc_bread(dev_num, blkoffset/mmc->read_bl_len, 1, _pu1BlkBuf) != 1)
		{
			return 0;
		}

		if (restsize >= (mmc->read_bl_len - offsetinblk))
		{
			opsize = mmc->read_bl_len - offsetinblk;
		}
		else
		{
			opsize = restsize;
		}

		memcpy((void *)memptr, (void *)((char *)_pu1BlkBuf + offsetinblk), opsize);

		restsize  -= opsize;
		memptr    += opsize;
		blkoffset += mmc->read_bl_len;
	}

	// Handle offset & size block-size-aligned case
	if (restsize >= mmc->read_bl_len)
	{
		ulong blkcnt = restsize/mmc->read_bl_len;

		if (mmc_bread(dev_num, blkoffset/mmc->read_bl_len, blkcnt, (void *)memptr) != blkcnt)
		{
			return 0;
		}

		restsize  -= (blkcnt * mmc->read_bl_len);
		memptr    += (blkcnt * mmc->read_bl_len);
		blkoffset += (blkcnt * mmc->read_bl_len);
	}

	// Handle size none-block-size-aligned case
	if (restsize)
	{
		if (mmc_bread(dev_num, blkoffset/mmc->read_bl_len, 1, _pu1BlkBuf) != 1)
		{
			return 0;
		}

		memcpy((void *)memptr, (void *)_pu1BlkBuf, restsize);
	}

	return bytecnt;
}

/* emmc_async_read_setup
   - only support basic dma mode
   - It is strongly recommanded to adopt basic dma async for little data(<64KB every time)
 */
int emmc_async_read_setup(int dev_num, u64 bytestart, ulong bytecnt, void *dst)
{
	ulong blkstart, blkcnt;
	struct mmc_cmd cmd;
	struct mmc_data data;
	
	if (bytecnt == 0)
		return 0;

	struct mmc *mmc = find_mmc_device(dev_num);
	if (!mmc)
		return 0;

	blkstart = bytestart / mmc->read_bl_len;
	blkcnt = bytecnt / mmc->read_bl_len;

	if (blkcnt > 1)
		cmd.cmdidx = MMC_CMD_READ_MULTIPLE_BLOCK;
	else
		cmd.cmdidx = MMC_CMD_READ_SINGLE_BLOCK;

	if (mmc->high_capacity)
		cmd.cmdarg = blkstart;
	else
		cmd.cmdarg = blkstart * mmc->read_bl_len;

	cmd.resp_type = MMC_RSP_R1;
	cmd.flags = 0;
	cmd.flags |= (dataMode & 0x0F);

	data.dest = dst;
	data.blocks = blkcnt;
	data.blocksize = mmc->read_bl_len;
	data.flags = MMC_DATA_READ;

	if(data.blocks > 1)
		cmd.flags |= (0x01<<4);

	/* dma mode cache flush */
	if((cmd.flags & 0x0F) > 1)
		HalFlushInvalidateDCacheMultipleLine((uint)(dst), (uint)(blkcnt*mmc->read_bl_len));
  
    /* descriptor dma mode configuration */
    if((cmd.flags & 0x0F) == 3)
        MsdcDescriptorConfig((void *)(dst), (uint)(blkcnt*mmc->read_bl_len));

	mmc_async.total_sz = bytecnt;
	mmc_async.cur_off = 0;
	mmc_async.cur_pos = (uint)(dst);
	mmc_async.cur_bytes = 0;
#ifdef CC_MTD_ENCRYPT_SUPPORT
    if(MsdcPartitionEncrypted(blkstart))
    {
	mmc_async.part_attr |= ATTR_ENCRYPTION;
	}
#endif
	mmc_async.fgdone = 0;
	memcpy((void *)(&mmc_async.cur_cmd), (void *)(&cmd), sizeof(struct mmc_cmd));
	memcpy((void *)(&mmc_async.cur_data), (void *)(&data), sizeof(struct mmc_data));

	if((cmd.flags & 0x0F) > 1)
		HalFlushInvalidateDCacheMultipleLine((uint)(dst), (uint)(bytecnt));

	if(MsdcReqCmdStart(mmc, &cmd, &data))
	{
		return -1;
	}
	
	return 0;
}

int emmc_async_dma_start_trigger(uint length)
{
	mmc_async.cur_bytes = length;
	mmc_async.fgdone = 0;
	
	if(0 != MsdcDMAStart(mmc_async.cur_pos, mmc_async.cur_off, 
		                   mmc_async.cur_bytes, mmc_async.total_sz))
	{
		printf("DMA Start failed!\n"); 
		return -1;
	}

	return 0;
}

int emmc_async_dma_wait_finish(void)
{	  
	struct mmc_cmd cmd;	
#ifdef CC_MTD_ENCRYPT_SUPPORT
  uint tmp = 0;
#endif

	cmd = mmc_async.cur_cmd;

	if(0 != MsdcDMAWaitIntr(&cmd, mmc_async.cur_off, mmc_async.cur_bytes, mmc_async.total_sz))
	{
		printf("DMA Transfer failed!\n");  
		return -1;
	}

	if((cmd.flags & 0x0F) > 1)
        HalInvalidateDCacheMultipleLine(mmc_async.cur_pos, mmc_async.cur_bytes);
	                                    
#ifdef CC_MTD_ENCRYPT_SUPPORT
  if(mmc_async.part_attr & ATTR_ENCRYPTION)
  {
    if(MsdcAesDecryption((uint)(mmc_async.cur_pos), (uint)(mmc_async.cur_bytes)))
    {
      //printf("aes encryption to buffer(addr:0x%08X len:0x%08X) before write process failed!\n", (uint)(mmc_async.cur_pos), 
                                                                                                //(uint)(mmc_async.cur_bytes));	
    }   	
  }
#endif 

	mmc_async.cur_pos += mmc_async.cur_bytes;
	mmc_async.cur_off += mmc_async.cur_bytes;
	mmc_async.fgdone = 1;

	return 0;
}

int emmc_async_read_finish(int dev_num)
{
	struct mmc_cmd cmd, cmd_stop;
	struct mmc_data data;	

	struct mmc *mmc = find_mmc_device(dev_num);
	if (!mmc)
	return 0;

	cmd = mmc_async.cur_cmd;
	data = mmc_async.cur_data;
	
	if(mmc_async.cur_off < mmc_async.total_sz)
	{
    if(MsdcDMATernmial(mmc, &cmd, &data))
    {
      printf("force stop dma egineer/emmc failed!\n");
      return -1;  	
    }	
	}
	else
	{
	  if ((data.blocks > 1)&&(!(cmd.flags>>4))) {
		  cmd_stop.cmdidx = MMC_CMD_STOP_TRANSMISSION;
		  cmd_stop.cmdarg = 0;
		  cmd_stop.resp_type = MMC_RSP_R1b;
		  cmd_stop.flags = 0;
		  if (mmc_send_cmd(mmc, &cmd_stop, NULL)) {
			  printf("mmc fail to send stop cmd\n");
			  return -1;
		  }
	  }
  }

	return 0;
}

int emmc_async_read_stop(int dev_num, int i4fg)
{
	struct mmc_cmd cmd;
	struct mmc_data data;	

	struct mmc *mmc = find_mmc_device(dev_num);
	if (!mmc)
		return 0;

	cmd = mmc_async.cur_cmd;
	data = mmc_async.cur_data;

	if(MsdcDMATernmial(mmc, &cmd, &data))
	{
		return -1;  	
	}
	
	return 0;
}

/* Support ordinary erase and trim operation*/

unsigned int emmc_get_erase_size(void)
{
	unsigned int erase_size;

	if (ext_csd.erase_grp_defn & 0x1)
	{
		erase_size = ext_csd.hc_erase_grp_size * 512 * 1024;
	}
	else
	{
		erase_size = (mmc_csd_v4.erase_grp_size + 1) * (mmc_csd_v4.erase_grp_mult + 1);
		erase_size = (erase_size << mmc_csd_v4.write_bl_len);
	}
	printf("EMMC erase align & size : %u/0x%X\n", erase_size, erase_size);

	return erase_size;
}

int mmc_go_idle(struct mmc* mmc)
{
	struct mmc_cmd cmd;
	int err;

	udelay(1000);

	cmd.cmdidx = MMC_CMD_GO_IDLE_STATE;
	cmd.cmdarg = 0;
	cmd.resp_type = MMC_RSP_NONE;
	cmd.flags = 0;

	err = mmc_send_cmd(mmc, &cmd, NULL);

	if (err)
		return err;

	udelay(2000);

	return 0;
}

int sd_send_op_cond(struct mmc *mmc)
{
	int timeout = 1000;
	int err;
	struct mmc_cmd cmd;

	do {
		cmd.cmdidx = MMC_CMD_APP_CMD;
		cmd.resp_type = MMC_RSP_R1;
		cmd.cmdarg = 0;
		cmd.flags = 0;

		err = mmc_send_cmd(mmc, &cmd, NULL);

		if (err)
			return err;

		cmd.cmdidx = SD_CMD_APP_SEND_OP_COND;
		cmd.resp_type = MMC_RSP_R3;

		/*
		 * Most cards do not answer if some reserved bits
		 * in the ocr are set. However, Some controller
		 * can set bit 7 (reserved for low voltages), but
		 * how to manage low voltages SD card is not yet
		 * specified.
		 */
		cmd.cmdarg = mmc->voltages & 0xff8000;

		if (mmc->version == SD_VERSION_2)
			cmd.cmdarg |= OCR_HCS;

		err = mmc_send_cmd(mmc, &cmd, NULL);

		if (err)
			return err;

		udelay(1000);
	} while ((!(cmd.response[0] & OCR_BUSY)) && timeout--);

	if (timeout <= 0)
		return UNUSABLE_ERR;

	if (mmc->version != SD_VERSION_2)
		mmc->version = SD_VERSION_1_0;

	mmc->ocr = cmd.response[0];

	mmc->high_capacity = ((mmc->ocr & OCR_HCS) == OCR_HCS);
	mmc->rca = 0;

	return 0;
}

int mmc_send_op_cond(struct mmc *mmc)
{
	int timeout = 1000;
	struct mmc_cmd cmd;
	int err;

	/* Some cards seem to need this */
	mmc_go_idle(mmc);

	do {
		cmd.cmdidx = MMC_CMD_SEND_OP_COND;
		cmd.resp_type = MMC_RSP_R3;
		cmd.cmdarg = OCR_HCS | mmc->voltages;
		cmd.flags = 0;

		err = mmc_send_cmd(mmc, &cmd, NULL);

		if (err)
			return err;

		udelay(1000);
	} while (!(cmd.response[0] & OCR_BUSY) && timeout--);

	if (timeout <= 0)
		return UNUSABLE_ERR;

	mmc->version = MMC_VERSION_UNKNOWN;
	mmc->ocr = cmd.response[0];

	mmc->high_capacity = ((mmc->ocr & OCR_HCS) == OCR_HCS);
	mmc->rca = 0;

	return 0;
}

int mmc_send_ext_csd(struct mmc *mmc, char *ext_csd)
{
	struct mmc_cmd cmd;
	struct mmc_data data;
	int err;

	/* Get the Card Status Register */
	cmd.cmdidx = MMC_CMD_SEND_EXT_CSD;
	cmd.resp_type = MMC_RSP_R1;
	cmd.cmdarg = 0;
	cmd.flags = 0;
	cmd.flags |= (dataMode & 0x0F);
	
	/* dma mode cache flush */
	if((cmd.flags & 0x0F) > 1)
		HalFlushInvalidateDCacheMultipleLine((uint)(ext_csd), 512);
  
	/* descriptor dma mode configuration */
	if((cmd.flags & 0x0F) == 3)
		MsdcDescriptorConfig((void *)(ext_csd), 512);

	data.dest = ext_csd;
	data.blocks = 1;
	data.blocksize = 512;
	data.flags = MMC_DATA_READ;

	err = mmc_send_cmd(mmc, &cmd, &data);

	return err;
}

int mmc_switch(struct mmc *mmc, u8 set, u8 index, u8 value)
{
	struct mmc_cmd cmd;

	cmd.cmdidx = MMC_CMD_SWITCH;
	cmd.resp_type = MMC_RSP_R1b;
	cmd.cmdarg = (MMC_SWITCH_MODE_WRITE_BYTE << 24) | (index << 16) | (value << 8);
	cmd.flags = 0;

	return mmc_send_cmd(mmc, &cmd, NULL);
}

int mmc_change_freq(struct mmc *mmc)
{
	char ext_csd[512];
	char cardtype;
	int err;

	mmc->card_caps = 0;

	/* Only version 4 supports high-speed */
	if (mmc->version < MMC_VERSION_4)
		return 0;

	mmc->card_caps |= MMC_MODE_4BIT;

	err = mmc_send_ext_csd(mmc, ext_csd);

	if (err)
		return err;
 
	/* It's not absolutely right for some emmc brand, for example THGBM3G4D1FBAIG;
	 * I think it is because of normal capacity with higher spec.
	 if (ext_csd[212] || ext_csd[213] || ext_csd[214] || ext_csd[215])
	 	mmc->high_capacity = 1;
	*/

	cardtype = ext_csd[196] & 0xf;

#if defined(CC_MT5399) || defined(CONFIG_ARCH_MT5399)|| \
    defined(CC_MT5890) || defined(CONFIG_ARCH_MT5890)|| \
    defined(CC_MT5882) || defined(CONFIG_ARCH_MT5882)
#else
	/* Position 1 Sample Edge Setting
	   I think it is necessary to handle high speed mode at low frequency(400KHz);
	   It is some special case for timing;
	   And switch command - cmd6 should be applied by new timing becuase of its response.
	 */
	MsdcSetSampleEdge(1);
#endif

	err = mmc_switch(mmc, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_HS_TIMING, 1);

	if (err)
		return err;

	/* Now check to see that it worked */
	err = mmc_send_ext_csd(mmc, ext_csd);

	if (err)
		return err;

	/* No high-speed support */
	if (!ext_csd[185])
		return 0;

	/* High Speed is set, there are two types: 52MHz and 26MHz */
	if (cardtype & MMC_HS_52MHZ)
		mmc->card_caps |= MMC_MODE_HS_52MHz | MMC_MODE_HS;
	else
		mmc->card_caps |= MMC_MODE_HS;

	return 0;
}

int sd_switch(struct mmc *mmc, int mode, int group, u8 value, u8 *resp)
{
	struct mmc_cmd cmd;
	struct mmc_data data;

	/* Switch the frequency */
	cmd.cmdidx = SD_CMD_SWITCH_FUNC;
	cmd.resp_type = MMC_RSP_R1;
	cmd.cmdarg = (mode << 31) | 0xffffff;
	cmd.cmdarg &= ~(0xf << (group * 4));
	cmd.cmdarg |= value << (group * 4);
	cmd.flags = 0;

	data.dest = (char *)resp;
	data.blocksize = 64;
	data.blocks = 1;
	data.flags = MMC_DATA_READ;

	return mmc_send_cmd(mmc, &cmd, &data);
}

int sd_change_freq(struct mmc *mmc)
{
	int err;
	struct mmc_cmd cmd;
	uint scr[2];
	uint switch_status[16];
	struct mmc_data data;
	int timeout;

	mmc->card_caps = 0;

	/* Read the SCR to find out if this card supports higher speeds */
	cmd.cmdidx = MMC_CMD_APP_CMD;
	cmd.resp_type = MMC_RSP_R1;
	cmd.cmdarg = mmc->rca << 16;
	cmd.flags = 0;

	err = mmc_send_cmd(mmc, &cmd, NULL);

	if (err)
		return err;

	cmd.cmdidx = SD_CMD_APP_SEND_SCR;
	cmd.resp_type = MMC_RSP_R1;
	cmd.cmdarg = 0;
	cmd.flags = 0;

	timeout = 3;

retry_scr:
	data.dest = (char *)&scr;
	data.blocksize = 8;
	data.blocks = 1;
	data.flags = MMC_DATA_READ;

	err = mmc_send_cmd(mmc, &cmd, &data);

	if (err) {
		if (timeout--)
			goto retry_scr;

		return err;
	}

	mmc->scr[0] = __be32_to_cpu(scr[0]);
	mmc->scr[1] = __be32_to_cpu(scr[1]);

	switch ((mmc->scr[0] >> 24) & 0xf) {
		case 0:
			mmc->version = SD_VERSION_1_0;
			break;
		case 1:
			mmc->version = SD_VERSION_1_10;
			break;
		case 2:
			mmc->version = SD_VERSION_2;
			break;
		default:
			mmc->version = SD_VERSION_1_0;
			break;
	}

	/* Version 1.0 doesn't support switching */
	if (mmc->version == SD_VERSION_1_0)
		return 0;

	timeout = 4;
	while (timeout--) {
		err = sd_switch(mmc, SD_SWITCH_CHECK, 0, 1,
				(u8 *)&switch_status);

		if (err)
			return err;

		/* The high-speed function is busy.  Try again */
		if (!(__be32_to_cpu(switch_status[7]) & SD_HIGHSPEED_BUSY))
			break;
	}

	if (mmc->scr[0] & SD_DATA_4BIT)
		mmc->card_caps |= MMC_MODE_4BIT;

	/* If high-speed isn't supported, we return */
	if (!(__be32_to_cpu(switch_status[3]) & SD_HIGHSPEED_SUPPORTED))
		return 0;

	err = sd_switch(mmc, SD_SWITCH_SWITCH, 0, 1, (u8 *)&switch_status);

	if (err)
		return err;

	if ((__be32_to_cpu(switch_status[4]) & 0x0f000000) == 0x01000000)
		mmc->card_caps |= MMC_MODE_HS;

	return 0;
}

/* frequency bases */
/* divided by 10 to be nice to platforms without floating point */
int fbase[] = {
	10000,
	100000,
	1000000,
	10000000,
};

/* Multiplier values for TRAN_SPEED.  Multiplied by 10 to be nice
 * to platforms without floating point.
 */
int multipliers[] = {
	0,	/* reserved */
	10,
	12,
	13,
	15,
	20,
	25,
	30,
	35,
	40,
	45,
	50,
	55,
	60,
	70,
	80,
};

void mmc_set_ios(struct mmc *mmc)
{
	mmc->set_ios(mmc);
}

void mmc_set_clock(struct mmc *mmc, uint clock)
{
	if (clock > mmc->f_max)
		clock = mmc->f_max;

	if (clock < mmc->f_min)
		clock = mmc->f_min;

	mmc->clock = clock;

	mmc_set_ios(mmc);
}

void mmc_set_bus_width(struct mmc *mmc, uint width)
{
	mmc->bus_width = width;

	mmc_set_ios(mmc);
}

int mmc_startup(struct mmc *mmc)
{
	int err;
	uint mult, freq;
	u64 cmult, csize;
	struct mmc_cmd cmd;
	char ext_csd[512];

	/* Put the Card in Identify Mode */
	cmd.cmdidx = MMC_CMD_ALL_SEND_CID;
	cmd.resp_type = MMC_RSP_R2;
	cmd.cmdarg = 0;
	cmd.flags = 0;

	err = mmc_send_cmd(mmc, &cmd, NULL);

	if (err)
		return err;

	memcpy(mmc->cid, cmd.response, 16);

  /* Find device from supported list*/
	MsdcFindDev(mmc->cid);

	/*
	 * For MMC cards, set the Relative Address.
	 * For SD cards, get the Relatvie Address.
	 * This also puts the cards into Standby State
	 */
	cmd.cmdidx = SD_CMD_SEND_RELATIVE_ADDR;
	cmd.cmdarg = mmc->rca << 16;
	cmd.resp_type = MMC_RSP_R6;
	cmd.flags = 0;

	err = mmc_send_cmd(mmc, &cmd, NULL);

	if (err)
		return err;

	if (IS_SD(mmc))
		mmc->rca = (cmd.response[0] >> 16) & 0xffff;

	/* Get the Card-Specific Data */
	cmd.cmdidx = MMC_CMD_SEND_CSD;
	cmd.resp_type = MMC_RSP_R2;
	cmd.cmdarg = mmc->rca << 16;
	cmd.flags = 0;

	err = mmc_send_cmd(mmc, &cmd, NULL);

	if (err)
		return err;

	mmc->csd[0] = cmd.response[0];
	mmc->csd[1] = cmd.response[1];
	mmc->csd[2] = cmd.response[2];
	mmc->csd[3] = cmd.response[3];
	emmc_decode_csd(mmc->csd);
	emmc_dump_csd();
	if (mmc->version == MMC_VERSION_UNKNOWN) {
		int version = (cmd.response[0] >> 26) & 0xf;

		switch (version) {
			case 0:
				mmc->version = MMC_VERSION_1_2;
				break;
			case 1:
				mmc->version = MMC_VERSION_1_4;
				break;
			case 2:
				mmc->version = MMC_VERSION_2_2;
				break;
			case 3:
				mmc->version = MMC_VERSION_3;
				break;
			case 4:
				mmc->version = MMC_VERSION_4;
				break;
			default:
				mmc->version = MMC_VERSION_1_2;
				break;
		}
	}

	/* divide frequency by 10, since the mults are 10x bigger */
	freq = fbase[(cmd.response[0] & 0x7)];
	mult = multipliers[((cmd.response[0] >> 3) & 0xf)];

	mmc->tran_speed = freq * mult;

	mmc->read_bl_len = 1 << ((cmd.response[1] >> 16) & 0xf);

	if (IS_SD(mmc))
		mmc->write_bl_len = mmc->read_bl_len;
	else
		mmc->write_bl_len = 1 << ((cmd.response[3] >> 22) & 0xf);

	if (mmc->high_capacity) {
		csize = (mmc->csd[1] & 0x3f) << 16
			| (mmc->csd[2] & 0xffff0000) >> 16;
		cmult = 8;
		//cmult = 0x7;     //modify by shunli wang at 2011.05.12
	} else {
		csize = (mmc->csd[1] & 0x3ff) << 2
			| (mmc->csd[2] & 0xc0000000) >> 30;
		cmult = (mmc->csd[2] & 0x00038000) >> 15;
	}

	mmc->capacity = (csize + 1) << (cmult + 2);
	mmc->capacity *= mmc->read_bl_len;

	if (mmc->read_bl_len > 512)
		mmc->read_bl_len = 512;

	if (mmc->write_bl_len > 512)
		mmc->write_bl_len = 512;

	/* Select the card, and put it into Transfer Mode */
	cmd.cmdidx = MMC_CMD_SELECT_CARD;
	cmd.resp_type = MMC_RSP_R1b;
	cmd.cmdarg = mmc->rca << 16;
	cmd.flags = 0;
	err = mmc_send_cmd(mmc, &cmd, NULL);

	if (err)
		return err;

	if (!IS_SD(mmc) && (mmc->version >= MMC_VERSION_4)) {
		/* check  ext_csd version and capacity */
		err = mmc_send_ext_csd(mmc, ext_csd);
		if (!err && (ext_csd[192] >= 2) && (mmc->high_capacity)) {
			mmc->capacity = ext_csd[212] << 0 | ext_csd[213] << 8 |
					ext_csd[214] << 16 | ext_csd[215] << 24;
			mmc->capacity *= 512;
		}
		memcpy(EXT_CSD, ext_csd, 512);
	}

	if (IS_SD(mmc))
		err = sd_change_freq(mmc);
	else
		err = mmc_change_freq(mmc);

	if (err)
		return err;

	/* Restrict card's capabilities by what the host can do */
	// It is a bug modified by shunli.wang
	mmc->card_caps |= MMC_MODE_8BIT;
	mmc->card_caps &= mmc->host_caps;

	if (IS_SD(mmc)) {
		if (mmc->card_caps & MMC_MODE_4BIT) {
			cmd.cmdidx = MMC_CMD_APP_CMD;
			cmd.resp_type = MMC_RSP_R1;
			cmd.cmdarg = mmc->rca << 16;
			cmd.flags = 0;

			err = mmc_send_cmd(mmc, &cmd, NULL);
			if (err)
				return err;

			cmd.cmdidx = SD_CMD_APP_SET_BUS_WIDTH;
			cmd.resp_type = MMC_RSP_R1;
			cmd.cmdarg = 2;
			cmd.flags = 0;
			err = mmc_send_cmd(mmc, &cmd, NULL);
			if (err)
				return err;

			mmc_set_bus_width(mmc, 4);
		}

		if (mmc->card_caps & MMC_MODE_HS)
			mmc_set_clock(mmc, 50000000);
		else
			mmc_set_clock(mmc, 25000000);
	} else {
		if (mmc->card_caps & MMC_MODE_4BIT) {
			/* Set the card to use 4 bit*/
			err = mmc_switch(mmc, EXT_CSD_CMD_SET_NORMAL,
					EXT_CSD_BUS_WIDTH,
					EXT_CSD_BUS_WIDTH_4);

			if (err)
				return err;

			mmc_set_bus_width(mmc, 4);
		} else if (mmc->card_caps & MMC_MODE_8BIT) {
			/* Set the card to use 8 bit*/
			err = mmc_switch(mmc, EXT_CSD_CMD_SET_NORMAL,
					EXT_CSD_BUS_WIDTH,
					EXT_CSD_BUS_WIDTH_8);

			if (err)
				return err;

			mmc_set_bus_width(mmc, 8);
		}

		if (mmc->card_caps & MMC_MODE_HS) {
			if (mmc->card_caps & MMC_MODE_HS_52MHz)
				mmc_set_clock(mmc, 52000000);
			else
				mmc_set_clock(mmc, 26000000);
		} else
			mmc_set_clock(mmc, 20000000);
	}

#if defined(CC_MT5399) || defined(CONFIG_ARCH_MT5399)|| \
    defined(CC_MT5890) || defined(CONFIG_ARCH_MT5890)|| \
    defined(CC_MT5882) || defined(CONFIG_ARCH_MT5882) 
#else
	/* Position 2 Sample Edge Setting*/
	MsdcSetSampleEdge(2);
#endif

	/* fill in device description */
	mmc->block_dev.lun = 0;
	mmc->block_dev.type = MTD_DEV_TYPE_EMMC;
	mmc->block_dev.blksz = mmc->read_bl_len;
	mmc->block_dev.lba = lldiv(mmc->capacity, mmc->read_bl_len);
	sprintf(mmc->block_dev.vendor, "Man %06x Snr %08x", mmc->cid[0] >> 8,
			(mmc->cid[2] << 8) | (mmc->cid[3] >> 24));
	sprintf(mmc->block_dev.product, "%c%c%c%c%c", mmc->cid[0] & 0xff,
			(mmc->cid[1] >> 24), (mmc->cid[1] >> 16) & 0xff,
			(mmc->cid[1] >> 8) & 0xff, mmc->cid[1] & 0xff);
	sprintf(mmc->block_dev.revision, "%d.%d", mmc->cid[2] >> 28,
			(mmc->cid[2] >> 24) & 0xf);
	//init_part(&mmc->block_dev);

	return 0;
}

int mmc_send_if_cond(struct mmc *mmc)
{
	struct mmc_cmd cmd;
	int err;

	cmd.cmdidx = SD_CMD_SEND_IF_COND;
	/* We set the bit if the host supports voltages between 2.7 and 3.6 V */
	cmd.cmdarg = ((mmc->voltages & 0xff8000) != 0) << 8 | 0xaa;
	cmd.resp_type = MMC_RSP_R7;
	cmd.flags = 0;

	err = mmc_send_cmd(mmc, &cmd, NULL);

	if (err)
		return err;

	if ((cmd.response[0] & 0xff) != 0xaa)
		return UNUSABLE_ERR;
	else
		mmc->version = SD_VERSION_2;

	return 0;
}

/* Support ordinary erase and trim operation
   mode:
     0 - Erase
     1 - Trim
     2 - Secure Erase
     3 - Secure Trim Step 1 (Spec 4.41 or elder)
     4 - Secure Trim Step 2 (Spec 4.41 or elder)
 */

int mmc_erase(struct mmc *mmc, u64 bytestart, u64 length, uint mode)
{
	struct mmc_cmd cmd;
	uint cmdarg = 0;
#ifdef CFG_LG_CHG
	length = (length -1);
#endif
	
  switch(mode)
  {
  case 0:
      cmdarg = 0x00000000;
      break;      
  case 1:
      cmdarg = 0x00000001;
      break;
  case 2:
      cmdarg = 0x80000000;
      break;
  case 3:
      cmdarg = 0x80000001;
    	break;
  case 4:
      cmdarg = 0x80008000;
    	break;
  default:
    	cmdarg = 0x00000000;
      break; 			
  }
	
	if((mode == 1) && ((EXT_CSD[231] & (0x1<<4)) == 0x0))
	{
    printf("Not Support Trim!\n");
		return 0;
	}

  // Send CMD35
	cmd.cmdidx = MMC_CMD_ERASE_START;
	cmd.resp_type = MMC_RSP_R1;
	cmd.flags = 0;
	if(mmc->high_capacity)
	{
    cmd.cmdarg = (uint)(bytestart >> 9);
	}
	else
	{
    cmd.cmdarg = (uint)bytestart;
	}
	
  if (mmc_send_cmd(mmc, &cmd, NULL)) {
    printf("mmc fail to send erase start cmd\n");
    return -1;
  }

	// Send CMD36
	cmd.cmdidx = MMC_CMD_ERASE_END;
	cmd.resp_type = MMC_RSP_R1;
	cmd.flags = 0;
	if(mmc->high_capacity)
	{
    cmd.cmdarg = (uint)((bytestart + length - 1) >> 9);
	}
	else
	{
    cmd.cmdarg = (uint)(bytestart + length - 1);
	}
	
  if (mmc_send_cmd(mmc, &cmd, NULL)) {
    printf("mmc fail to send erase end cmd\n");
    return -1;
  }

	// Send CMD38
	cmd.cmdidx = MMC_CMD_ERASE;
	cmd.cmdarg = cmdarg;
	cmd.resp_type = MMC_RSP_R1b;
	cmd.flags = 0;
  if (mmc_send_cmd(mmc, &cmd, NULL)) {
    printf("mmc fail to send erase cmd\n");
    return -1;
  }

  // wait host idle
  MsdcWaitHostIdle(10000);
	
	// Get Card Status
	cmd.cmdidx = MMC_CMD_SEND_STATUS;
	cmd.cmdarg = mmc->rca << 16;
	cmd.resp_type = MMC_RSP_R1;
	cmd.flags = 0;
	
  if (mmc_send_cmd(mmc, &cmd, NULL)) {
    printf("mmc fail to send card status cmd\n");
    return -1;
  }
	if(cmd.response[0] & (0x1<<28))
	{
    printf("an error in the sequence of erase commands occurred!\n");
    return -1;
	}
	else if(cmd.response[0] & (0x1<<31))
	{
    printf("out of the allowed range for mmc!\n");
		return -1;
	}
	else if(cmd.response[0] & (0x1<<27))
	{
    printf("an invalid selection of erase groups for erase occurred!\n");
		return -1;
	}

  return 0;
}

int mmc_erase_all(struct mmc *mmc)
{
  uint64_t capacity = mmc->capacity;
  uint err = 0;
    
  err = mmc_erase(mmc, 0, capacity, 0);

  return err;    	
}

int mmc_reverse_uchar(uchar *pBuff, uint len)
{
  uint i, j;
	uchar u1Tmp;

	for(i = 0, j = len - 1; i<j; i++, j--)
	{
    u1Tmp = pBuff[i];
		pBuff[i] = pBuff[j];
		pBuff[j] = u1Tmp;
	}

	return 0;
}

int mmc_reverse_uint(uint *pBuff, uint len)
{
  uint i, j;
  uint u4Tmp; 

	for(i = 0, j = len - 1; i<j; i++, j--)
	{
		u4Tmp = pBuff[i];
		pBuff[i] = pBuff[j];
		pBuff[j] = u4Tmp;
	}

	return 0;
}

int mmc_wp_segmant(struct mmc *mmc, uint start, uint fgEn)
{
	struct mmc_cmd cmd;
	int err;

	cmd.cmdidx = fgEn?(MMC_CMD_SET_WRITE_PROT):(MMC_CMD_CLR_WRITE_PROT);
	cmd.resp_type = MMC_RSP_R1b;
	cmd.flags = 0;
	
	if (mmc->high_capacity)
		cmd.cmdarg = start;
	else
		cmd.cmdarg = start * mmc->write_bl_len;

	err = mmc_send_cmd(mmc, &cmd, NULL);

	return err;	
}

/* Support kinds of write protect operations
   level:
     0 - All Card
     1 - USER AREA
     2 - BOOT PARTITION
   type:
     0 - Temporary
     1 - Power on
     2 - Permenent 
   fgEn:
     0 - Disable WP
     1 - Enable WP  
 */
int mmc_wp(struct mmc *mmc, ulong bytestart, uint level, uint type, uint fgEn)
{
	struct mmc_cmd cmd;
	struct mmc_data data;
	uint tmpcsd[4], ret = 0;
	uint writeblklen, erasegroupsz, wpgroupsz, erasegrpdef;
	
	/* update the ext_csd datas */
	mmc_send_ext_csd(mmc, EXT_CSD);

    /* reverse csd for the normal order*/
	memcpy((void *)tmpcsd, (void *)(mmc->csd), sizeof(mmc->csd)/sizeof(uchar));
    mmc_reverse_uint(tmpcsd, sizeof(tmpcsd)/sizeof(uint));

    /* calculate necessary variable
        - erase group def 
        - write block length
        - erase group size
        - write protect group size
      */
    erasegrpdef = EXT_CSD[175];
    writeblklen = (0x1<<((tmpcsd[0]&(0x0F<<22))>>22));
  
    if(erasegrpdef == 0)
    {
        erasegroupsz = writeblklen; 
        erasegroupsz *= (((tmpcsd[1]&(0x1F<<10))>>10) + 1)*(((tmpcsd[1]&(0x1F<<5))>>5) + 1);
        
        wpgroupsz = erasegroupsz;
        wpgroupsz *= ((tmpcsd[1]&(0x1F<<0))>>0) + 1;  	
    }
    else
    {
        erasegroupsz = (512*1024); 
        erasegroupsz *= (EXT_CSD[224]);
        
        wpgroupsz = erasegroupsz;
        wpgroupsz *= (EXT_CSD[221]);       	
    }

	printf("Write Block Len:%08X, Erapse Group:%08X, WP Group:%08X\n", writeblklen, erasegroupsz, wpgroupsz);

    if(level == 0)
    {
        if(type == 1)
        {
            printf("Not support Power-on write protect for All card!\n");
            ret = -1;
            goto end;	
        }
    	  
        if(((type == 0) && (((tmpcsd[0]&(0x1<<12))>>12) == fgEn)) ||
            ((type == 2) && (((tmpcsd[0]&(0x1<<13))>>13) == fgEn)))
        {
            printf("write protect has been %s!\n", (fgEn == 0)?"disable":"enable");
            ret = 0;
            goto end;	 	
        }	
        
        if(type == 0)
        {
            if(fgEn == 0x1)
            {
                tmpcsd[0] |= (0x1<<12);
            }
            else if(fgEn == 0x0)
            {
                tmpcsd[0] &= (~(0x1<<12));
            }
        }

#if 0        
        if(type == 2)
        {
            if(fgEn == 0x1)
            {
                tmpcsd[0] |= (0x1<<13);
            }
            else if(fgEn == 0x0)
            {
                tmpcsd[0] &= (~(0x1<<13));
            }
        } 
#endif

    mmc_reverse_uchar((uchar *)tmpcsd, sizeof(tmpcsd)/sizeof(uchar));
    
    // Send CMD27
    cmd.cmdidx = MMC_CMD_PROGRAM_CSD;
    cmd.cmdarg = 0;
    cmd.resp_type = MMC_RSP_R1;
    cmd.flags |= (dataMode & 0x0F);
    
    data.src = (char *)(tmpcsd);
    data.blocks = 1;
    data.blocksize = sizeof(tmpcsd)/sizeof(uchar);
    data.flags = MMC_DATA_WRITE;
    if (mmc_send_cmd(mmc, &cmd, &data)) {
        printf("mmc fail to send program CSD cmd\n");
        ret = -1;
        goto end;	
    }   
    
    // Send CMD13
    cmd.cmdidx = MMC_CMD_SEND_STATUS;
    cmd.cmdarg = mmc->rca << 16;
    cmd.resp_type = MMC_RSP_R1;
    cmd.flags = 0;
    if (mmc_send_cmd(mmc, &cmd, NULL)) {
        printf("mmc fail to send card status cmd!\n");
        ret = -1;
        goto end;	
    }
    if(cmd.response[0] & (0x1<<16))
    {
        printf("Read only section not match the card content!\n");
        ret = -1;
        goto end;	
    }  
    
    mmc_reverse_uchar((uchar *)tmpcsd, sizeof(tmpcsd)/sizeof(uchar));  
    mmc_reverse_uint(tmpcsd, sizeof(tmpcsd)/sizeof(uint));
    memcpy((void *)(mmc->csd), (void *)tmpcsd, sizeof(mmc->csd)/sizeof(uchar));
                
  }
  else if(level == 1)
  {
    if(type == 0)
    {
      if(fgEn == 1)
      {
        EXT_CSD[171]&=(~(0x1<<0));
        EXT_CSD[171]&=(~(0x1<<2));
        if(mmc_switch(mmc, EXT_CSD_CMD_SET_NORMAL, 171, EXT_CSD[171])){
          printf("mmc fail to set ext csd!\n");
				  ret = -1;
          goto end;  
        }
      }   
      if(mmc_wp_segmant(mmc, bytestart/(mmc->read_bl_len), fgEn)){
          printf("mmc fail to write protect segmant!\n");
				  ret = -1;
          goto end;	
      }
    }
    else if (type == 1)
    {
      if(fgEn == 0)
      {
        printf("Not support disable Power-on/Permenant write protect\n");	
        ret = -1;
        goto end;	
      }
      
      EXT_CSD[171]|=(0x1<<0);
      EXT_CSD[171]|=(0x1<<0);
      EXT_CSD[171]&=(~(0x1<<2));
      if(mmc_switch(mmc, EXT_CSD_CMD_SET_NORMAL, 171, EXT_CSD[171])){
        printf("mmc fail to set ext csd!\n");
				ret = -1;
        goto end;  
      }
      if(mmc_wp_segmant(mmc, bytestart/(mmc->read_bl_len), fgEn)){
        printf("mmc fail to write protect segmant!\n");
				ret = -1;
        goto end;	
      } 
    }
#if 0
    else if(type == 2)
    {
      if(fgEn == 0)
      {
        printf("Not support disable Power-on/Permenant write protect\n");	
        ret = -1;
        goto end;	
      }
      EXT_CSD[171]|=(0x1<<2);
      if(mmc_switch(mmc, EXT_CSD_CMD_SET_NORMAL, 171, EXT_CSD[171])){
        printf("mmc fail to set ext csd!\n");
				ret = -1;
        goto end;  
      }
      if(mmc_wp_segmant(mmc, bytestart/(mmc->read_bl_len), fgEn)){
        printf("mmc fail to write protect segmant!\n");
				ret = -1;
        goto end;	
      }  	
    }	
#endif
  }
  else
  { 
    if(type == 0)
    {
      printf("Boot partition not support temporary protect!\n"); 
      ret = -1;
      goto end;	   	
    }
    else if (type == 1)
    {
    	EXT_CSD[173]&=(~(0x1<<2));
    	EXT_CSD[173]&=(~(0x1<<0));
    	if(mmc_switch(mmc, EXT_CSD_CMD_SET_NORMAL, 173, EXT_CSD[173])){
        printf("mmc fail to set ext csd!\n");
				ret = -1;
        goto end;  
      } 
    }
#if 0
    else if(type == 2)
    {
    	ext_csd[173]|=(0x1<<2);
    	if(mmc_switch(mmc, EXT_CSD_CMD_SET_NORMAL, 173, EXT_CSD[173])){
        printf("mmc fail to set ext csd!\n");
				ret = -1;
        goto end;  
      } 
    }	
#endif    	
  }

end:    
	return ret;
}

int mmc_register(struct mmc *mmc)
{
	/* Setup the universal parts of the block interface just once */
	mmc->block_dev.if_type = IF_TYPE_MMC;
	mmc->block_dev.dev = cur_dev_num++;
	mmc->block_dev.removable = 0;
	mmc->block_dev.block_read = mmc_bread;
	mmc->block_dev.block_write = mmc_bwrite;

	mmc->block_dev.byte_read = mmc_byteread;
	mmc->block_dev.byte_write = mmc_bytewrite;

	INIT_LIST_HEAD (&mmc->link);

	list_add_tail (&mmc->link, &mmc_devices);

	return 0;
}

block_dev_desc_t *mmc_get_dev(int dev)
{
	struct mmc *mmc = find_mmc_device(dev);

	return mmc ? &mmc->block_dev : NULL;
}

int mmc_init(struct mmc *mmc)
{
	int err;

  if (i4mmcInit)
  {
    return 0;
  }

	memset(EXT_CSD, 0x00, 512);

	err = mmc->init(mmc);

	if (err)
		return err;

	mmc_set_bus_width(mmc, 1);
	mmc_set_clock(mmc, 1);

#if defined(CC_MT5399) || defined(CONFIG_ARCH_MT5399)|| \
    defined(CC_MT5890) || defined(CONFIG_ARCH_MT5890)|| \
    defined(CC_MT5882) || defined(CONFIG_ARCH_MT5882) 
#else
	/* Position 0 Sample Edge Setting*/
	MsdcSetSampleEdge(0);
#endif

	/* Reset the Card */
	err = mmc_go_idle(mmc);

	if (err)
		return err;

	/* Test for SD version 2 */
	//err = mmc_send_if_cond(mmc);

	/* Now try to get the SD card's operating condition */
	//err = sd_send_op_cond(mmc);

	/* If the command timed out, we check for an MMC card */
	err = TIMEOUT;
	if (err == TIMEOUT) {
		err = mmc_send_op_cond(mmc);

		if (err) {
			printf("Card did not respond to voltage select!\n");
			return UNUSABLE_ERR;
		}
  }

	err = mmc_startup(mmc);
	memcpy(ext_csd.buffer, EXT_CSD, 512);
	emmc_decode_ext_csd(&ext_csd);
	emmc_dump_ext_csd();
  i4mmcInit = 1;
  return err;
}

#if defined(CC_MT5399) || defined(CONFIG_ARCH_MT5399) || \
    defined(CC_MT5890) || defined(CONFIG_ARCH_MT5890) || \
    defined(CC_MT5882) || defined(CONFIG_ARCH_MT5882) 
/*
 * fast initialization of mmc
 * version: new
 * 
 */
int mmc_init_fast(struct mmc *mmc)
{
  uint mult, freq, tmpval, sec_count = 0;
  u64 cmult = 0, csize = 0;
  char cardtype;
  int err;
	
  if (i4mmcInit)
    return 0;

  /* ext_csd init */
  memset(EXT_CSD, 0x00, 512);

  /* reverse the value of cid */
  tmpval = mmc->cid[0];mmc->cid[0] = mmc->cid[3];mmc->cid[3] = tmpval;
  tmpval = mmc->cid[1];mmc->cid[1] = mmc->cid[2];mmc->cid[2] = tmpval;		
  
  /* Find device from supported list */
  MsdcFindDev(mmc->cid);

  /* reverse the value of csd */
  tmpval = mmc->csd[0];mmc->csd[0] = mmc->csd[3];mmc->csd[3] = tmpval;
  tmpval = mmc->csd[1];mmc->csd[1] = mmc->csd[2];mmc->csd[2] = tmpval;

  /* capacity > 2GB or not */
  mmc->high_capacity = ((mmc->ocr & OCR_HCS) == OCR_HCS);

  /* mmc csd decode */
  emmc_decode_csd(mmc->csd);
  emmc_dump_csd();
  /* mmc version assign */
  mmc->version = MMC_VERSION_UNKNOWN;
  if (mmc->version == MMC_VERSION_UNKNOWN) {
    int version = ((mmc->csd[0] >> 26) & 0xf);

    switch (version) {
      case 0:
        mmc->version = MMC_VERSION_1_2;
        break;
      case 1:
        mmc->version = MMC_VERSION_1_4;
        break;
      case 2:
        mmc->version = MMC_VERSION_2_2;
        break;
      case 3:
        mmc->version = MMC_VERSION_3;
        break;
      case 4:
        mmc->version = MMC_VERSION_4;
        break;
      default:
        mmc->version = MMC_VERSION_1_2;
        break;
    }
  }	

  /* old version will not be supported */
  if (mmc->version < MMC_VERSION_4) {
  	printf("old version of mmc device: %d\n", mmc->version);
  	return -1;
  }

  /* mmc ext_csd read */
  err = mmc_send_ext_csd(mmc, EXT_CSD);
  if (err) {
  	printf("<mmc_init_fast> get ext_csd failed(%d)\n", err);
    return err;
  }

  /* mmc tran speed assign */
  freq = fbase[(mmc->csd[0] & 0x7)];
  mult = multipliers[((mmc->csd[0] >> 3) & 0xf)];
  mmc->tran_speed = freq * mult;

  /* mmc read/write block length assign */
  mmc->read_bl_len = 1 << ((mmc->csd[1] >> 16) & 0xf);
  mmc->write_bl_len = 1 << ((mmc->csd[3] >> 22) & 0xf);

  /* card type & caps assign */
  cardtype = EXT_CSD[196] & 0x3f;
  mmc->card_caps |= cardtype;
  mmc->card_caps |= MMC_MODE_4BIT | MMC_MODE_8BIT;
  mmc->card_caps &= mmc->host_caps;

  /* HS timing setting */
  tmpval = 0;
  if (mmc->card_caps & MMC_MODE_HS200_MASK)
  	tmpval = 2;
  else if (mmc->card_caps & MMC_MODE_HS)
    tmpval = 1;
  printf("[emmc]bus timing switch to %s\n", (tmpval>0)?((tmpval>1)?"HS200":"HS"):"DS");
  err = mmc_switch(mmc, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_HS_TIMING, tmpval);
  if (err) {
  	printf("<mmc_init_fast> switch high speed timing(%d) failed(%d)\n", tmpval, err);
    return err;
  }
  
  if(tmpval == 1)
    MsdcSetSpeedMode(SPEED_MODE_HS);
  else if(tmpval == 2)
  	MsdcSetSpeedMode(SPEED_MODE_HS200);
  else
  	MsdcSetSpeedMode(SPEED_MODE_DS);

  /* bus width setting */
  tmpval = 0;
  if (!(mmc->card_caps & MMC_MODE_HS200_MASK))    
  	if (mmc->card_caps & MMC_MODE_DDR_MASK)  	  
	  tmpval += 4;
  
  if (mmc->card_caps & MMC_MODE_8BIT)
  	tmpval += 2;
  else if (mmc->card_caps & MMC_MODE_4BIT)
  	tmpval += 1;
  printf("[emmc]bus width switch to %d(%s)\n", 0x1<<((tmpval>4)?(tmpval-3):((tmpval>0)?(tmpval+1):tmpval)),   	
  	                                           (tmpval>4)?"DDR":"SDR");  
  err = mmc_switch(mmc, EXT_CSD_CMD_SET_NORMAL,
    EXT_CSD_BUS_WIDTH,
    tmpval);
  if (err) {
  	printf("<mmc_init_fast> switch bus width(%) failed(%d)\n", tmpval, err);
    return err;
  }
  

  if(tmpval > 4)
    MsdcSetSpeedMode(SPEED_MODE_DDR50);
      
  mmc_set_bus_width(mmc, tmpval);

  /* clock frequency setting */
  tmpval = 0;
  if (mmc->card_caps & MMC_MODE_HS200_MASK)
  	tmpval = 200000000;
  else
    tmpval = 50000000;

  printf("[emmc]bus clock switch to %d\n", tmpval);
  mmc_set_clock(mmc, tmpval);
	  
  /* mmc capacity calculate */
  if (mmc->high_capacity) {
    sec_count = *((uint *)(EXT_CSD + 212));
	mmc->capacity = (u64)sec_count;
	mmc->capacity *= 0x200;
  } else {
    csize = (mmc->csd[1] & 0x3ff) << 2
			| (mmc->csd[2] & 0xc0000000) >> 30;
    cmult = (mmc->csd[2] & 0x00038000) >> 15;	
	mmc->capacity = (csize + 1) << (cmult + 2);
	mmc->capacity *= mmc->read_bl_len;
  }

  /* fill in device description */
  mmc->block_dev.lun = 0;
  mmc->block_dev.type = MTD_DEV_TYPE_EMMC;
  mmc->block_dev.blksz = mmc->read_bl_len;
  mmc->block_dev.lba = lldiv(mmc->capacity, mmc->read_bl_len);
  sprintf(mmc->block_dev.vendor, "Man %06x Snr %08x", mmc->cid[0] >> 8,
			(mmc->cid[2] << 8) | (mmc->cid[3] >> 24));
  sprintf(mmc->block_dev.product, "%c%c%c%c%c", mmc->cid[0] & 0xff,
			(mmc->cid[1] >> 24), (mmc->cid[1] >> 16) & 0xff,
			(mmc->cid[1] >> 8) & 0xff, mmc->cid[1] & 0xff);
  sprintf(mmc->block_dev.revision, "%d.%d", mmc->cid[2] >> 28,
			(mmc->cid[2] >> 24) & 0xf);
  init_part(&mmc->block_dev);
  memcpy(ext_csd.buffer, EXT_CSD, 512);
  emmc_decode_ext_csd(&ext_csd);
  emmc_dump_ext_csd();

  i4mmcInit = 1;
  return 0;
}

#else
/*
 * fast initialization of mmc
 * version: old
 * 
 */
int mmc_init_fast(struct mmc *mmc)
{
  uint mult, freq, tmp;
  u64 cmult, csize;
  char cardtype;
  int err;
	
  if (i4mmcInit)
  {
    return 0;
  }

  /* Position 0 Sample Edge Setting*/
  MsdcSetSampleEdge(0);
  
  memset(EXT_CSD, 0x00, 512);
	
  tmp = mmc->cid[0];
  mmc->cid[0] = mmc->cid[3];
  mmc->cid[3] = tmp;
  tmp = mmc->cid[1];
  mmc->cid[1] = mmc->cid[2];
  mmc->cid[2] = tmp;
		
  /* Find device from supported list*/
  MsdcFindDev(mmc->cid);

  tmp = mmc->csd[0];
  mmc->csd[0] = mmc->csd[3];
  mmc->csd[3] = tmp;
  tmp = mmc->csd[1];
  mmc->csd[1] = mmc->csd[2];
  mmc->csd[2] = tmp;
  /* mmc csd decode */
  emmc_decode_csd(mmc->csd);
  emmc_dump_csd();

  mmc->version = MMC_VERSION_UNKNOWN;
  mmc->high_capacity = ((mmc->ocr & OCR_HCS) == OCR_HCS);
  if (mmc->version == MMC_VERSION_UNKNOWN) {
    int version = ((mmc->csd[0] >> 26) & 0xf);

    switch (version) {
      case 0:
        mmc->version = MMC_VERSION_1_2;
        break;
      case 1:
        mmc->version = MMC_VERSION_1_4;
        break;
      case 2:
        mmc->version = MMC_VERSION_2_2;
        break;
      case 3:
        mmc->version = MMC_VERSION_3;
        break;
      case 4:
        mmc->version = MMC_VERSION_4;
        break;
      default:
        mmc->version = MMC_VERSION_1_2;
        break;
    }
  }	

  /* divide frequency by 10, since the mults are 10x bigger */
  freq = fbase[(mmc->csd[0] & 0x7)];
  mult = multipliers[((mmc->csd[0] >> 3) & 0xf)];

  mmc->tran_speed = freq * mult;

  mmc->read_bl_len = 1 << ((mmc->csd[1] >> 16) & 0xf);
  mmc->write_bl_len = 1 << ((mmc->csd[3] >> 22) & 0xf);

  if (mmc->high_capacity) {
    csize = (mmc->csd[1] & 0x3f) << 16
			| (mmc->csd[2] & 0xffff0000) >> 16;
    cmult = 8;
    //cmult = 0x7;     //modify by shunli wang at 2011.05.12
  } else {
    csize = (mmc->csd[1] & 0x3ff) << 2
			| (mmc->csd[2] & 0xc0000000) >> 30;
    cmult = (mmc->csd[2] & 0x00038000) >> 15;
  }

  mmc->capacity = (csize + 1) << (cmult + 2);
  mmc->capacity *= mmc->read_bl_len;

  if (mmc->read_bl_len > 512)
      mmc->read_bl_len = 512;

  if (mmc->write_bl_len > 512)
      mmc->write_bl_len = 512;
	
  mmc->card_caps = 0;	
  /* Only version 4 supports high-speed */
  if (mmc->version >= MMC_VERSION_4)
  {  
    mmc->card_caps |= MMC_MODE_4BIT;
	  
    /* Position 1 Sample Edge Setting
	   I think it is necessary to handle high speed mode at low frequency(400KHz);
	   It is some special case for timing;
	   And switch command - cmd6 should be applied by new timing becuase of its response.
      */
    MsdcSetSampleEdge(1);
    
    err = mmc_switch(mmc, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_HS_TIMING, 1);
    if (err)
      return err;

    /* Now check to see that it worked */
    err = mmc_send_ext_csd(mmc, EXT_CSD);

    if (err)
      return err;

    cardtype = EXT_CSD[196] & 0xf;
    /* No high-speed support */
    if (!EXT_CSD[185])
      return 0;

    /* High Speed is set, there are two types: 52MHz and 26MHz */
    if (cardtype & MMC_HS_52MHZ)
      mmc->card_caps |= MMC_MODE_HS_52MHz | MMC_MODE_HS;
    else
      mmc->card_caps |= MMC_MODE_HS;
  }

  /* Restrict card's capabilities by what the host can do */
  // It is a bug modified by shunli.wang
  mmc->card_caps |= MMC_MODE_8BIT;
  mmc->card_caps &= mmc->host_caps;
	
  if (mmc->card_caps & MMC_MODE_4BIT) {
    /* Set the card to use 4 bit*/
    err = mmc_switch(mmc, EXT_CSD_CMD_SET_NORMAL,
    EXT_CSD_BUS_WIDTH,
    EXT_CSD_BUS_WIDTH_4);

    if (err)
      return err;
      
    mmc_set_bus_width(mmc, 4);
  } else if (mmc->card_caps & MMC_MODE_8BIT) {
    /* Set the card to use 8 bit*/
    err = mmc_switch(mmc, EXT_CSD_CMD_SET_NORMAL,
					EXT_CSD_BUS_WIDTH,
					EXT_CSD_BUS_WIDTH_8);

    if (err)
      return err;

    mmc_set_bus_width(mmc, 8);
  }

  if (mmc->card_caps & MMC_MODE_HS) {
    if (mmc->card_caps & MMC_MODE_HS_52MHz)
    {
      mmc_set_clock(mmc, 52000000);}
    else
				mmc_set_clock(mmc, 26000000);
  } else
    mmc_set_clock(mmc, 20000000);
    
  /* Position 2 Sample Edge Setting*/
  MsdcSetSampleEdge(2);
        
  if (mmc->version >= MMC_VERSION_4) {
    /* check  ext_csd version and capacity */
    if ((EXT_CSD[192] >= 2) && (mmc->high_capacity)) {
      mmc->capacity = EXT_CSD[212] << 0 | EXT_CSD[213] << 8 |
					EXT_CSD[214] << 16 | EXT_CSD[215] << 24;
      mmc->capacity *= 512;
    }
  }
        
  /* fill in device description */
  mmc->block_dev.lun = 0;
  mmc->block_dev.type = MTD_DEV_TYPE_EMMC;
  mmc->block_dev.blksz = mmc->read_bl_len;
  mmc->block_dev.lba = lldiv(mmc->capacity, mmc->read_bl_len);
  sprintf(mmc->block_dev.vendor, "Man %06x Snr %08x", mmc->cid[0] >> 8,
			(mmc->cid[2] << 8) | (mmc->cid[3] >> 24));
  sprintf(mmc->block_dev.product, "%c%c%c%c%c", mmc->cid[0] & 0xff,
			(mmc->cid[1] >> 24), (mmc->cid[1] >> 16) & 0xff,
			(mmc->cid[1] >> 8) & 0xff, mmc->cid[1] & 0xff);
  sprintf(mmc->block_dev.revision, "%d.%d", mmc->cid[2] >> 28,
			(mmc->cid[2] >> 24) & 0xf);
  init_part(&mmc->block_dev);

  memcpy(ext_csd.buffer, EXT_CSD, 512);
  emmc_decode_ext_csd(&ext_csd);
  emmc_dump_ext_csd();
  i4mmcInit = 1;
  return 0;
}
#endif

/*
 * CPU and board-specific MMC initializations.  Aliased function
 * signals caller to move on
 */

/* not used, we marked it.
static int __def_mmc_init(bd_t *bis)
{
	return -1;
}*/

//int cpu_mmc_init(bd_t *bis) __attribute__((weak, alias("__def_mmc_init")));
//int board_mmc_init(bd_t *bis) __attribute__((weak, alias("__def_mmc_init")));

void print_mmc_devices(char separator)
{
  int i = 0;
	struct mmc *m;
	struct list_head *entry;

	list_for_each(entry, &mmc_devices)
  {
    m = list_entry(entry, struct mmc, link);
    memcpy(&emmc_info[i], m, sizeof(struct mmc));
    i++;

		printf("%s: %d", m->name, m->block_dev.dev);

		if (entry->next != &mmc_devices)
			printf("%c ", separator);
	}

	printf("\n");
}

int mmc_initialize(bd_t *bis)
{
	INIT_LIST_HEAD (&mmc_devices);
	cur_dev_num = 0;

extern int board_mmc_init(bd_t *bis);
extern int cpu_mmc_init(bd_t *bis);

	if (board_mmc_init(bis) < 0)
		cpu_mmc_init(bis);

	print_mmc_devices(',');

#ifdef CFG_LG_CHG
	init_partinfo();
#endif
	return 0;
}

unsigned int emmc_get_trim_enable(void)
{
	if (ext_csd.ext_csd_rev >= 6) // 6 is emmc ver 4.5
	{
		return 1;
	}
	else if (ext_csd.ext_csd_rev == 5) // 6 is emmc ver 4.5
	{
		if (ext_csd.sec_support & 0x10)
		{
			return 1;
		}
	}

	return 0;
}

void emmc_decode_ext_csd(struct sd_mmc_ext_csd *pextcsd)
{
	unsigned char buffer[512];
	memcpy(buffer, pextcsd->buffer, 512);

	pextcsd->s_cmd_set =  buffer[504]; //1bits 504
	pextcsd->power_off_long_time =  buffer[247];
	pextcsd->ini_timeout_ap =  buffer[241]; //1bits 241
	pextcsd->pwr_cl_ddr_52_360 =  buffer[239]; //1bits 239
	pextcsd->pwr_cl_ddr_52_195 =  buffer[238]; //1bits 238
	pextcsd->min_perf_ddr_w8_52 =  buffer[235]; //1bits 235
	pextcsd->min_perf_ddr_r8_52 =  buffer[234]; //1bits 234
	pextcsd->trim_mult =  buffer[232]; //1bits 232
	pextcsd->sec_support                 =  buffer[231]; //1bits 231
	pextcsd->sec_erase_mult              =  buffer[230]; //1bits 230
	pextcsd->sec_trim_mult               =  buffer[229]; //1bits 229
	pextcsd->boot_info                   =  buffer[228]; //1bits 228
	pextcsd->boot_size_mult              =  buffer[226]; //1bits 226
	pextcsd->access_size                 =  buffer[225]; //1bits 225
	pextcsd->hc_erase_grp_size   =  buffer[224]; //1bits 224
	pextcsd->erase_timeout_multi =  buffer[223]; //1bits 223
	pextcsd->rel_wr_sec_cnt              =  buffer[222]; //1bits 222
	pextcsd->hc_wp_grp_size              =  buffer[221]; //1bits 221
	pextcsd->sleep_cur_vcc               =  buffer[220]; //1bits 220
	pextcsd->sleep_cur_vccq              =  buffer[219]; //1bits 219
	pextcsd->slp_awk_timeout     =  buffer[217]; //1bits 217

	pextcsd->sec_count = ( (buffer[215] << 24) | (buffer[214] << 16) | ( buffer[213] << 8) | (0 << buffer[212]));//4bits 215-212
	pextcsd->min_pref_w_8_52      = buffer[210];    //1bit 210
	pextcsd->min_pref_r_8_52      = buffer[209];    //1bit 209
	pextcsd->min_pref_w_8_26_4_52 = buffer[208];    //1bit 208
	pextcsd->min_pref_r_8_26_4_52 = buffer[207];    //1bit 207
	pextcsd->min_pref_w_4_26      = buffer[206];    //1bit 206
	pextcsd->min_pref_r_4_26      = buffer[205];    //1bit 205
	pextcsd->pwr_cl_26_360        = buffer[203];    //1bit 203
	pextcsd->pwr_cl_52_360        = buffer[202];    //1bit 202
	pextcsd->pwr_cl_26_195        = buffer[201];    //1bit 201
	pextcsd->pwr_cl_52_195        = buffer[200];    //1bit 200
	pextcsd->card_type            = buffer[196];    //1bit 196
	pextcsd->csd_structure        = buffer[194];    //1bit 194
	pextcsd->ext_csd_rev          = buffer[192];    //1bit 192

	// Modes segment
	pextcsd->cmd_Set              = buffer[191];    //1bit 191 r/w
	pextcsd->cmd_set_rev          = buffer[189];    //bit1 189
	pextcsd->power_class          = buffer[187];    //bit1 187
	pextcsd->hs_timing            = buffer[185];    //bit1 185
	pextcsd->bus_width            = buffer[183];    //bit1 183
	pextcsd->erased_mem_cont      = buffer[181];    //bit1 181
	pextcsd->boot_config                  = buffer[179];
	pextcsd->boot_config_prot     = buffer[178];
	pextcsd->boot_bus_width               = buffer[177];
	pextcsd->erase_grp_defn               = buffer[175];
	pextcsd->boot_wp                              = buffer[173];
	pextcsd->user_wp                              = buffer[171];
	pextcsd->fw_config                    = buffer[169];
	pextcsd->rpmb_size_mult               = buffer[168];
	pextcsd->rst_n_func                   = buffer[162];
	pextcsd->part_support                 = buffer[160];
	pextcsd->max_enh_size_mult    = (buffer[159] << 16)| (buffer[158] << 8) | buffer[157];
	pextcsd->part_attrb                   = buffer[156];
	pextcsd->part_set_complete    = buffer[155];
	pextcsd->GP_size_mult0                = (buffer[145] << 16) | (buffer[144] << 8) | buffer[143];
	pextcsd->GP_size_mult1                = (buffer[148] << 16) | (buffer[147] << 8) | buffer[146];
	pextcsd->GP_size_mult2                = (buffer[151] << 16) | (buffer[150] << 8) | buffer[149];
	pextcsd->GP_size_mult3                = (buffer[154] << 16) | (buffer[153] << 8) | buffer[152];
	pextcsd->enh_size_mult                = (buffer[142] << 16) | (buffer[141] << 8) | buffer[140];
	pextcsd->enh_start_addr       = (buffer[139] << 24) | (buffer[138] << 16) | (buffer[137] << 8) | buffer[136];
	pextcsd->sec_bad_blk_mngt     =  buffer[134];
	pextcsd->power_off_noti =  buffer[34];
}

void emmc_dump_ext_csd(void)
{
#if 0
	int i=0;

	printf("\n============= DUMPING EXT_CSD REGISTER =============\n");
	for(i=511; i>=0; i-=16)
	{
		printf("%02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %3d \n",
		ext_csd.buffer[i],   ext_csd.buffer[i-1],
		ext_csd.buffer[i-2], ext_csd.buffer[i-3],
		ext_csd.buffer[i-4], ext_csd.buffer[i-5],
		ext_csd.buffer[i-6], ext_csd.buffer[i-7],
		ext_csd.buffer[i-8], ext_csd.buffer[i-9],
		ext_csd.buffer[i-10],ext_csd.buffer[i-11],
		ext_csd.buffer[i-12],ext_csd.buffer[i-13],
		ext_csd.buffer[i-14],ext_csd.buffer[i-15], i-15);
	}
	printf("===================================================\n");
#endif
	printf("\n==========DUMPING EXT_CSD REGISTER==================\n");

	printf("byte [34] is power_off_noti      : [0x%x]\n", ext_csd.power_off_noti);
	printf("byte [134] is sec_bad_blk_mngt      : [0x%x]\n", ext_csd.sec_bad_blk_mngt);
	printf("byte [136] is enh_start_addr        : [0x%x]\n", ext_csd.enh_start_addr);
	printf("byte [140] is enh_size_mult         : [0x%x]\n", ext_csd.enh_size_mult);
	printf("byte [152] is GP_size_mult3         : [0x%x]\n", ext_csd.GP_size_mult3);
	printf("byte [149] is GP_size_mult2         : [0x%x]\n", ext_csd.GP_size_mult2);
	printf("byte [146] is GP_size_mult1         : [0x%x]\n", ext_csd.GP_size_mult1);
	printf("byte [143] is GP_size_mult0         : [0x%x]\n", ext_csd.GP_size_mult0);
	printf("byte [155] is part_set_complete     : [0x%x]\n", ext_csd.part_set_complete);
	printf("byte [156] is part_attrb            : [0x%x]\n", ext_csd.part_attrb);
	printf("byte [157] is max_enh_size_mult     : [0x%x]\n", ext_csd.max_enh_size_mult);
	printf("byte [160] is part_support          : [0x%x]\n", ext_csd.part_support);
	printf("byte [162] is rst_n_func            : [0x%x]\n", ext_csd.rst_n_func);
	printf("byte [168] is rpmb_size_mult        : [0x%x]\n", ext_csd.rpmb_size_mult);
	printf("byte [169] is fw_config             : [0x%x]\n", ext_csd.fw_config);
	printf("byte [171] is user_wp               : [0x%x]\n", ext_csd.user_wp);
	printf("byte [173] is boot_wp               : [0x%x]\n", ext_csd.boot_wp);
	printf("byte [175] is erase grp defn        : [0x%x]\n", ext_csd.erase_grp_defn);
	printf("byte [177] is boot bus width        : [0x%x]\n", ext_csd.boot_bus_width);
	printf("byte [179] is boot config           : [0x%x]\n", ext_csd.boot_config);
	printf("byte [181] is erased mem count      : [0x%x]\n", ext_csd.erased_mem_cont);
	printf("byte [183] is bus width mode        : [0x%x]\n", ext_csd.bus_width);
	printf("byte [185] is High Speed            : [0x%x]\n", ext_csd.hs_timing);
	printf("byte [187] is power class           : [0x%x]\n", ext_csd.power_class);
	printf("byte [191] is Cmd Set               : [0x%x]\n", ext_csd.cmd_set_rev);
	printf("byte [192] is Ext CSD Rev           : [0x%x]\n", ext_csd.ext_csd_rev);
	printf("byte [194] is CSD Structure         : [0x%x]\n", ext_csd.csd_structure);
	printf("byte [196] is Card Type             : [0x%x]\n", ext_csd.card_type);
	printf("byte [200] is pwr class 52_195      : [0x%x]\n", ext_csd.pwr_cl_52_195);
	printf("byte [201] is pwr class 26_195      : [0x%x]\n", ext_csd.pwr_cl_26_195);
	printf("byte [202] is pwr class 52_360      : [0x%x]\n", ext_csd.pwr_cl_52_360);
	printf("byte [203] is pwr class 26_360      : [0x%x]\n", ext_csd.pwr_cl_26_360);
	printf("byte [205] is min pref r 4_26       : [0x%x]\n", ext_csd.min_pref_r_4_26);
	printf("byte [206] is min pref w 4_26       : [0x%x]\n", ext_csd.min_pref_w_4_26);
	printf("byte [207] is min pref r 8_26_4_52  : [0x%x]\n", ext_csd.min_pref_r_8_26_4_52);
	printf("byte [208] is min pref w 8_26_4_52  : [0x%x]\n", ext_csd.min_pref_w_8_26_4_52);
	printf("byte [209] is min pref r 8_52       : [0x%x]\n", ext_csd.min_pref_r_8_52);
	printf("byte [210] is min pref w 8_52       : [0x%x]\n", ext_csd.min_pref_w_8_52);
	printf("byte [226] is boot size mult        : [0x%x]\n",ext_csd.boot_size_mult);
	printf("byte [225] is access size           : [0x%x]\n",ext_csd.access_size);
	printf("byte [224] is hc erase group        : [0x%x]\n",ext_csd.hc_erase_grp_size);
	printf("byte [221] is hc wp group size      : [0x%x]\n",ext_csd.hc_wp_grp_size);
	printf("byte [223] is erase_timeout_multi   : [0x%x]\n",ext_csd.erase_timeout_multi);
	printf("byte [222] is rel_wr_sec_cnt        : [0x%x]\n",ext_csd.rel_wr_sec_cnt);
	printf("byte [220] is sleep_cur_vcc         : [0x%x]\n",ext_csd.sleep_cur_vcc);
	printf("byte [219] is sleep_cur_vccq        : [0x%x]\n",ext_csd.sleep_cur_vccq);
	printf("byte [217] is slp_awk_timeout       : [0x%x]\n",ext_csd.slp_awk_timeout);
	printf("byte [215-212] is sec count         : [0x%x]\n", ext_csd.sec_count);
	printf("byte [241] is ini_timeout_ap        : [0x%x]\n",ext_csd.ini_timeout_ap);
	printf("byte [239] is pwr_cl_ddr_52_360     : [0x%x]\n",ext_csd.pwr_cl_ddr_52_360);
	printf("byte [238] is pwr_cl_ddr_52_195     : [0x%x]\n",ext_csd.pwr_cl_ddr_52_195);
	printf("byte [235] is min_perf_ddr_w8_52    : [0x%x]\n",ext_csd.min_perf_ddr_w8_52 );
	printf("byte [234] is min_perf_ddr_r8_52    : [0x%x]\n",ext_csd.min_perf_ddr_r8_52 );
	printf("byte [232] is trim_mult             : [0x%x]\n",ext_csd.trim_mult);
	printf("byte [231] is sec_support           : [0x%x]\n",ext_csd.sec_support);
	printf("byte [230] is sec_erase_mult        : [0x%x]\n",ext_csd.sec_erase_mult);
	printf("byte [229] is sec_trim_mult         : [0x%x]\n",ext_csd.sec_trim_mult );
	printf("byte [228] is boot_info             : [0x%x]\n", ext_csd.boot_info );
	printf("byte [226] is boot_size_mult        : [0x%x]\n",ext_csd.boot_size_mult       );
	printf("byte [247] is power_off_noti      : [0x%x]\n", ext_csd.power_off_long_time);
	printf("byte [504] is supported cmd set     : [0x%x]\n", ext_csd.s_cmd_set);
	printf("========================================================\n");
}

// Name        : sd_decode_mmc_csd
//
// Argument  : *raw_csd : pointer to mmc csd structure.
//
//Returns      : void; Status: void
//
//Description : This function will decode the CSD structure for the MMC card
//
#define READ_BITS(value,mask,shift,out)     \
{           \
	unsigned temp;          \
	temp = (value & mask);      \
	temp >>= shift;       \
	out = temp;     \
}

void emmc_decode_csd(unsigned int *raw_csd)
{
	//unsigned csd_struct = 0;
	unsigned m3_h,m3_l;


////////reading for all the mmc versions
//this is required for backward compatibility in code even if mmc version is >= 4.
	//reserved 3 bits
	{
		READ_BITS(raw_csd[0],0xc0000000,30,mmc_csd_v4.csd_struct);
		READ_BITS(raw_csd[0],0x3c000000,26,mmc_csd_v4.specs_ver);
		READ_BITS(raw_csd[0],0x03000000,24,mmc_csd_v4.reserve3);
		READ_BITS(raw_csd[0],0x00FF0000,16,mmc_csd_v4.taac);
		READ_BITS(raw_csd[0],0x0000FF00,8,mmc_csd_v4.nsac);
		READ_BITS(raw_csd[0],0x000000FF,0,mmc_csd_v4.tran_speed);

		READ_BITS(raw_csd[1],0xFFF00000,20,mmc_csd_v4.ccc);
		READ_BITS(raw_csd[1],0x000F0000,16,mmc_csd_v4.read_bl_len);
		READ_BITS(raw_csd[1],0x00008000,15,mmc_csd_v4.read_partial);
		READ_BITS(raw_csd[1],0x00004000,14,mmc_csd_v4.write_misalign);
		READ_BITS(raw_csd[1],0x00002000,13,mmc_csd_v4.read_misalign);
		READ_BITS(raw_csd[1],0x00001000,12,mmc_csd_v4.dsr_imp);
		READ_BITS(raw_csd[1],0x00000c00,10,mmc_csd_v4.reserve2);

		READ_BITS(raw_csd[1],0x000003ff,0 ,m3_h);          //62-73 bits=12 overlap
		READ_BITS(raw_csd[2],0xc0000000,30,m3_l);
		m3_h <<= 2; //to make upper part
		mmc_csd_v4.c_size = m3_h | m3_l;

		READ_BITS(raw_csd[2],0x38000000,27,mmc_csd_v4.vdd_r_curr_min);
		READ_BITS(raw_csd[2],0x07000000,24,mmc_csd_v4.vdd_r_curr_max);
		READ_BITS(raw_csd[2],0x00d00000,21,mmc_csd_v4.vdd_w_curr_min);
		READ_BITS(raw_csd[2],0x001c0000,18,mmc_csd_v4.vdd_w_curr_max);

		READ_BITS(raw_csd[2],0x00038000,15,mmc_csd_v4.c_size_mult);
		READ_BITS(raw_csd[2],0x00007c00,10,mmc_csd_v4.erase_grp_size);
		READ_BITS(raw_csd[2],0x000003e0,5 ,mmc_csd_v4.erase_grp_mult);
		READ_BITS(raw_csd[2],0x0000001F,0 ,mmc_csd_v4.wp_grp_size);
		READ_BITS(raw_csd[3],0x80000000,31,mmc_csd_v4.wp_grp_enable);
		READ_BITS(raw_csd[3],0x60000000,29,mmc_csd_v4.default_ecc);
		READ_BITS(raw_csd[3],0x1c000000,26,mmc_csd_v4.r2w_factor);
		READ_BITS(raw_csd[3],0x03c00000,22,mmc_csd_v4.write_bl_len);
		READ_BITS(raw_csd[3],0x00200000,21,mmc_csd_v4.write_partial);
		READ_BITS(raw_csd[3],0x001d0000,17,mmc_csd_v4.reserve1);
		READ_BITS(raw_csd[3],0x00010000,16,mmc_csd_v4.content_prot_app);
		READ_BITS(raw_csd[3],0x0000800 ,15,mmc_csd_v4.file_format_grp);
		READ_BITS(raw_csd[3],0x00004000,14,mmc_csd_v4.copy);
		READ_BITS(raw_csd[3],0x00002000,13,mmc_csd_v4.perm_write_protect);
		READ_BITS(raw_csd[3],0x00001000,12,mmc_csd_v4.tmp_write_prot);
		READ_BITS(raw_csd[3],0x00000c00,10,mmc_csd_v4.file_format);
		READ_BITS(raw_csd[3],0x00000300,8 ,mmc_csd_v4.ecc);
	}
}

void emmc_dump_csd(void)
{
		printf("\n==========DUMPING MMC CSD REGISTER==================\n");
		printf("csd_struct        : [0x%0X] \n",mmc_csd_v4.csd_struct);
		printf("specs_ver         : [0x%0X] \n",mmc_csd_v4.specs_ver);
		printf("reserve3          : [0x%0X] \n",mmc_csd_v4.reserve3);
		printf("taac              : [0x%0X] \n",mmc_csd_v4.taac);
		printf("nsac              : [0x%0X] \n",mmc_csd_v4.nsac);
		printf("tran_speed        : [0x%0X] \n",mmc_csd_v4.tran_speed);
		printf("ccc               : [0x%0X] \n",mmc_csd_v4.ccc);
		printf("read_bl_len       : [0x%0X] \n",mmc_csd_v4.read_bl_len);
		printf("read_partial      : [0x%0X] \n",mmc_csd_v4.read_partial);
		printf("write_misalign    : [0x%0X] \n",mmc_csd_v4.write_misalign);
		printf("read_misalign     : [0x%0X] \n",mmc_csd_v4.read_misalign);
		printf("dsr_imp           : [0x%0X] \n",mmc_csd_v4.dsr_imp);
		printf("reserve2          : [0x%0X] \n",mmc_csd_v4.reserve2);
		printf("c_size            : [0x%0X] \n",mmc_csd_v4.c_size);
		printf("vdd_r_curr_min    : [0x%0X] \n",mmc_csd_v4.vdd_r_curr_min);
		printf("vdd_r_curr_max    : [0x%0X] \n",mmc_csd_v4.vdd_r_curr_max);
		printf("vdd_w_curr_min    : [0x%0X] \n",mmc_csd_v4.vdd_w_curr_min);
		printf("vdd_w_curr_max    : [0x%0X] \n",mmc_csd_v4.vdd_w_curr_max);
		printf("c_size_mult       : [0x%0X] \n",mmc_csd_v4.c_size_mult);
		printf("erase_grp_size    : [0x%0X] \n",mmc_csd_v4.erase_grp_size);
		printf("erase_grp_mult    : [0x%0X] \n",mmc_csd_v4.erase_grp_mult);
		printf("wp_grp_size       : [0x%0X] \n",mmc_csd_v4.wp_grp_size);
		printf("wp_grp_enable     : [0x%0X] \n",mmc_csd_v4.wp_grp_enable);
		printf("default_ecc       : [0x%0X] \n",mmc_csd_v4.default_ecc);
		printf("r2w_factor        : [0x%0X] \n",mmc_csd_v4.r2w_factor);
		printf("write_bl_len      : [0x%0X] \n",mmc_csd_v4.write_bl_len);
		printf("write_partial     : [0x%0X] \n",mmc_csd_v4.write_partial);
		printf("reserve1          : [0x%0X] \n",mmc_csd_v4.reserve1);
		printf("content_prot_app  : [0x%0X] \n",mmc_csd_v4.content_prot_app);
		printf("file_format_grp   : [0x%0X] \n",mmc_csd_v4.file_format_grp);
		printf("copy              : [0x%0X] \n",mmc_csd_v4.copy);
		printf("perm_write_protect: [0x%0X] \n",mmc_csd_v4.perm_write_protect);
		printf("tmp_write_prot    : [0x%0X] \n",mmc_csd_v4.tmp_write_prot);
		printf("file_format       : [0x%0X] \n",mmc_csd_v4.file_format);
		printf("ecc               : [0x%0X] \n",mmc_csd_v4.ecc);
		printf("==================================================\n");
}



#endif // CC_EMMC_BOOT

