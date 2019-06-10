#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "serial_number.h"

int main(int argc, char **argv)
{
	struct sn_device_info sndi[10];
	int ndevs;
	int i;

	ndevs = mdevices_v_ul(sndi, 10, MDEVS_ALL, 1);

	for (i = 0; i < ndevs; i++) {
		printf("%-18s %-15s %-20s %s\n", sndi[i].ip_addr,
			sndi[i].devname, sndi[i].pci_addr,
			sndi[i].serial_number);
	}

	return 0;
}

