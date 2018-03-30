#ifndef _DOGE_NETWORK_H
#define _DOGE_NETWORK_H

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/netdevice.h>
#include "doge_klink.h"
#include "doge_tcp.h"
#include "doge_ip.h"
#include "doge_arp.h"
#include "doge_eth.h"

struct doge_packet {
        struct net_device *dev;

        union {
                struct doge_tcphdr *tcphdr;
                struct doge_icmphdr *icmphdr;
                unsigned char *raw;
        } thdr;

        union {
                struct doge_iphdr *iphdr;
                struct doge_arphdr *arphdr;
                unsigned char *raw;
        } nhdr;

        struct doge_ethhdr *lhdr;
        unsigned char *data;
        int data_len;
        u8 th_proto;
        u16 nh_proto;
        u8 th_size;
        u8 nh_size;
        u8 lh_size;
};

struct doge_packet *doge_create_packet(void);
void doge_send(struct doge_packet *dpkt);
void doge_free_packet(struct doge_packet *dpkt);

#endif
