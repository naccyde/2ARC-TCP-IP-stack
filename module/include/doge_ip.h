#ifndef _DOGE_IP_H
#define _DOGE_IP_H

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/ip.h>
#include "doge_network.h"
#include "doge_icmp.h"
#include "doge_tcp.h"

#define DOGE_IP_PROTO ETH_P_IP

struct doge_iphdr {
        u8 version:4;
        u8 ihl:4;
        u8 tos;
        __be16 tot_len;
        __be16 id;
        __be16 frag_offset;
        u8 ttl;
        u8 th_proto;
        __be32 src_ip;
        __be32 dest_ip;
};

struct doge_iphdr *doge_create_iphdr(void);
void doge_fill_iphdr(struct doge_iphdr *iphdr, __be32 src_ip, __be32 dest_ip, int th_proto);
void doge_diphdr_to_kiphdr(struct doge_iphdr *dip, struct iphdr *kip);
void doge_kiphdr_to_diphdr(struct iphdr *kip, struct doge_iphdr *dip);
void doge_handle_ip(struct doge_packet *dpkt);

#endif
