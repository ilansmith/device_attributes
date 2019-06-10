#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MDEVS_TAVOR_CR 0x20 /*  Each device that maps to CR */
#define MDEVS_ALL 0xffffffff

int mdevices_v_ul(char *buf, int len, int mask, int verbosity);

int main(int argc, char **argv)
{
	char *buf;
	char *ptr;
	int ndevs;
	int i;

	buf = (char*)malloc(2048);
	if (!buf) {
		printf("couldn't allocate buffer\n");
		return -1;
	}

	ndevs = mdevices_v_ul(buf, 2048, MDEVS_ALL, 1);

	ptr = buf;
	for (i = 0; i < ndevs; i++) {
		size_t len = strlen(ptr);

		printf("%s\n", ptr);
		ptr += len + 1;
	}
	free(buf);

	return 0;
}

