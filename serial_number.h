#ifndef _SERIAL_NUMBER_H_
#define _SERIAL_NUMBER_H_

#include <net/if.h>

#define IP_ADDR_LEN (16 + 1)
#define PCI_ADDR_LEN 256
#define SERIAL_NUMBER_LEN 30

#define MDEVS_TAVOR_CR 0x20 /*  Each device that maps to CR */
#define MDEVS_ALL 0xffffffff

struct sn_device_info {
    char devname[IFNAMSIZ];
    char ip_addr[IP_ADDR_LEN];
    char pci_addr[PCI_ADDR_LEN];
    char serial_number[SERIAL_NUMBER_LEN];
};

int mdevices_v_ul(struct sn_device_info *sndi, int len, int mask,
	int verbosity);

#endif

