#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

static long supported_dev_ids[] = {
    /* CX2 */
    0x6340, 0x634a, 0x6368, 0x6372,
    0x6732, 0x673c, 0x6750, 0x675a,
    0x676e, 0x6746, 0x6764,
    /*****/
    0x1003,     //Connect-X3
    0x1007,     //Connect-X3Pro
    0x1011,     //Connect-IB
    0x1013,     //Connect-X4
    0x1015,     //Connect-X4Lx
    0x1017,     //Connect-X5
    0x1019,     //Connect-X5Ex
    0x101b,     //Connect-X6
    0x101d,     //Connect-X6DX
    0xc738,     //SwitchX
    0xcb20,     //Switch-IB
    0xcb84,     //Spectrum
    0xcf08,     //Switch-IB2
    0xd2f0,     //Quantum
    0xcf6c,     //Spectrum2
    0xa2d2,     //MT416842 Family BlueField integrated ConnectX-5 network controller
    -1
};

static long live_fish_id_database[] = {
    0x191,
    0x246,
    0x249,
    0x24b,
    0x24d,
    0x24e,
    0x1F6,
    0x1F8,
    0x1FF,
    0x247,
    0x209,
    0x20b,
    0x20d,
    0x20f,
    0x211,
    0x212,
    -1
};

int is_supported_devid(long devid)
{
    int i = 0;
    while (supported_dev_ids[i] != -1) {
        if (devid == supported_dev_ids[i]) {
            return 1;
        }
        i++;
    }
    i = 0;
    while (live_fish_id_database[i] != -1) {
        if (devid == live_fish_id_database[i]) {
            return 1;
        }
        i++;
    }
    return 0;
}

int is_supported_device(char *devname)
{

    char fname[64] = {0};
    char inbuf[64] = {0};
    FILE *f;
    int ret_val = 0;
    snprintf(fname, sizeof(fname) - 1, "/sys/bus/pci/devices/%s/device", devname);
    f = fopen(fname, "r");
    if (f == NULL) {
        //printf("-D- Could not open file: %s\n", fname);
        return 1;
    }
    if (fgets(inbuf, sizeof(inbuf), f)) {
        long devid = strtol(inbuf, NULL, 0);
        ret_val = is_supported_devid(devid);
    }
    fclose(f);
    return ret_val;
}

int mdevices_v_ul(char *buf, int len, int mask, int verbosity)
{

#define MDEVS_TAVOR_CR  0x20
#define MLNX_PCI_VENDOR_ID  0x15b3

    FILE *f;
    DIR *d;
    struct dirent *dir;
    int pos = 0;
    int sz;
    int rsz;
    int ndevs = 0;

    if (!(mask & MDEVS_TAVOR_CR)) {
        return 0;
    }

    char inbuf[64] = {0};
    char fname[64] = {0};

    d = opendir("/sys/bus/pci/devices");
    if (d == NULL) {
        return -2;
    }

    while ((dir = readdir(d)) != NULL) {
        if (dir->d_name[0] == '.') {
            continue;
        }
        sz = strlen(dir->d_name);
        if (sz > 2 && strcmp(dir->d_name + sz - 2, ".0") && !verbosity) {
            continue;
        } else if (sz > 4 && strcmp(dir->d_name + sz - 4, "00.0") && !verbosity) {
            // Skip virtual functions
            char physfn[64] = {0};
            DIR *physfndir;
            snprintf(physfn, sizeof(physfn) - 1, "/sys/bus/pci/devices/%.34s/physfn", dir->d_name);
            if ((physfndir = opendir(physfn)) != NULL) {
                closedir(physfndir);
                continue;
            }
        }
        snprintf(fname, sizeof(fname) - 1, "/sys/bus/pci/devices/%.34s/vendor", dir->d_name);
        f = fopen(fname, "r");
        if (f == NULL) {
            ndevs = -2;
            goto cleanup_dir_opened;
        }
        if (fgets(inbuf, sizeof(inbuf), f)) {
            long venid = strtoul(inbuf, NULL, 0);
            if (venid == MLNX_PCI_VENDOR_ID && is_supported_device(dir->d_name)) {
                rsz = sz + 1; //dev name size + place for Null char
                if ((pos + rsz) > len) {
                    ndevs = -1;
                    goto cleanup_file_opened;
                }
                memcpy(&buf[pos], dir->d_name, rsz);
                pos += rsz;
                ndevs++;
            }
        }
        fclose(f);
    }
    closedir(d);

    return ndevs;

cleanup_file_opened:
    fclose(f);
cleanup_dir_opened:
    closedir(d);
    return ndevs;
}

