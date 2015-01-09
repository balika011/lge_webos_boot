#define EMMC_PRINT			printf
#define READ_BITS(value,mask,shift,out)     \
{           \
	unsigned temp;          \
	temp = (value & mask);      \
	temp >>= shift;       \
	out = temp;     \
}

