#ifndef _DOGE_ETH_H
#define _DOGE_ETH_H

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/if_ether.h>
#include <linux/string.h>

#define ETH_LEN 6

struct doge_ethhdr {
        unsigned char src_mac[ETH_LEN];
        unsigned char dest_mac[ETH_LEN];
        u16 nh_proto;
};

struct doge_ethhdr *doge_create_ethhdr(void);
void doge_fill_ethhdr(struct doge_ethhdr *ethhdr, unsigned char *src, unsigned char *dest, int nh_proto);
void doge_fill_ethhdr_bdcast(struct doge_ethhdr *ethhdr, unsigned char *src, int nh_proto);
void doge_dethhdr_to_kethhdr(struct doge_ethhdr *deth, struct ethhdr *keth);
void doge_kethhdr_to_dethhdr(struct ethhdr *keth, struct doge_ethhdr *deth);

#endif
