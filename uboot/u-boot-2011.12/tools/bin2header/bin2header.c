#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <sys/stat.h>

void help();

int main(int argc, char **argv) {
	uint32_t size;
	FILE *f = fopen(argv[1], "r");
	FILE *fsave = fopen("public_key.h", "w");

	struct stat stbuf;

	stat(argv[1], &stbuf);
	if(f == NULL) {
		printf("cant open...\n");
		help();
		exit(-1);
	}
	size = stbuf.st_size;
	printf("/*\n");
	printf(" * array created from bin-file by bin2header (c) 2013 Nils Stec, <nils.stec@gmail.com>\n");
	printf(" * file '%s', filesize %dbytes\n", argv[1], size);
	printf(" *\n");
	printf(" */\n\n");

	uint32_t count = 0;
	uint8_t data;
	uint32_t x_count = 0;

	//printf("uint8_t %s[%d] = {\n", argv[2], size);	
	fprintf(fsave,"#ifndef CUSTOMER_PUB_MTKA5LR_KEY_H \n");
	fprintf(fsave,"#define CUSTOMER_PUB_MTKA5LR_KEY_H \n");
	fprintf(fsave," const unsigned char customer_pub_mtka5lr_key[] = {  \n");
	for(x_count = 0; x_count < 256; x_count++) 
		{
			data = getc(f);
			
			if (x_count%12 == 0)
				{
					fprintf(fsave,"\n");
				}
			if(x_count == 255)
				fprintf(fsave,"	0x%02X",data);
				else
				fprintf(fsave,"	0x%02X,",data);
			//printf("0x%02x", data);
	
		}
	
	fprintf(fsave,"\n }; \n");

fprintf(fsave,"#endif \n");
fclose(fsave);
printf("\n done");

	return 0;

}

void help() {
	printf("usage: bin2header <filename> <array-name>\n");
	printf("write to file: bin2header <filename> <array-name> > header.h\n");
	return;
}
