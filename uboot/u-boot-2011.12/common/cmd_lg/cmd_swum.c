#include <swum.h>

extern int storage_get_partition(const char* name, storage_partition_t* info);
extern int storage_read(uint32_t offset, size_t len, void* buf);
extern int storage_write(uint32_t offset, size_t len, const void* buf);

int _read_emergency_partition()
{
	int *magic = NULL;
	int result = 0;
	ulong offset = 0;
	storage_partition_t partinfo = {0,};

	if(storage_get_partition("emer", &partinfo) < 0)
	{
		printf("emergency partition is not valid !!!\n");
		return -1;
	}

	offset = partinfo.offset;

	magic = (int*)malloc(sizeof(int));

	if(magic == NULL)
		return -1;

	if(storage_read(offset, sizeof(int), (void*)magic) < 0)
		return -1;

	result = *magic;

	if(magic)
		free(magic);

	return result;
}

int _write_emergency_partition(int magic)
{
	ulong offset = 0;
	storage_partition_t partinfo = {0,};

	if(storage_get_partition("emer", &partinfo) < 0)
	{
		printf("emergency partition is not valid !!!\n");
		return -1;
	}

	offset = partinfo.offset;
	if(storage_write(offset, sizeof(int), (void*)&magic) < 0)
	{
		printf("can not write emergency partition!!!\n");
		return -1;
	}

	return 0;
}

int get_swumode()
{
	uint32_t magic = 0;
	int swumode = 0;

	magic = _read_emergency_partition();

	if(magic == SWUM_MAGIC)
		swumode = 1;
	else if(magic < 0)
	{
		swumode = 0;
		printf("Error : _read_emergency_partition");
	}
	else
		swumode = 0;

	return swumode;
}

static int do_emergency_swum(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	if(argc == 2)
	{
		char* input = argv[1];

		if(!strcmp("on", input))
		{
			_write_emergency_partition(SWUM_MAGIC);
			printf("swu mode on (emergency mode)\n");
		}
		else if(!strcmp("off", input))
		{
			_write_emergency_partition(0);
			printf("swu mode off (emergency mode)\n");
		}
		else
			printf("wrong option. set 'on' or 'off'\n");
	}
	else if(argc == 1)
	{
		if(get_swumode())
			printf("current swu mode is 'on' \n");
		else
			printf("current swu mode is 'off' \n");
	}
	else
		printf("argument count is not valid \n");

	return 0;
}

U_BOOT_CMD(
	emergency, 2, 0 , do_emergency_swum,
	"emergency\t- Select swu mode on/off \n\n",
	"emergency [on/off]\n"
);
