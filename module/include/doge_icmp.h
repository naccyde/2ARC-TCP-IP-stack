#ifndef _DOGE_ICMP_H
#define _DOGE_ICMP_H

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/icmp.h>
#include "doge_network.h"

#define DOGE_ICMP_PROTO IPPROTO_ICMP

#define DOGE_ICMP_TYPE_ECHO ICMP_ECHO
#define DOGE_ICMP_TYPE_ECHOREPLY ICMP_ECHOREPLY

struct doge_packet;

struct doge_icmphdr {
        u8 type;
        u8 code;
        u8 cksm;
        __be32 id;
        __be32 sequence;
        unsigned char *data;
        int data_len;
};

struct doge_icmphdr *doge_create_icmphdr(void);
void doge_kicmphdr_to_dicmphdr(int len, struct icmphdr *kicmphdr, struct doge_icmphdr *dicmphdr);
void doge_dicmphdr_to_kicmphdr(struct doge_icmphdr *dicmphdr, struct icmphdr *kicmphdr);
void doge_handle_icmp(struct doge_packet *dpkt);

#endif
