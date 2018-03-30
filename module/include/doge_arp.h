#ifndef _DOGE_ARP_H
#define _DOGE_ARP_H

#include <linux/kernel.h>
#include <linux/module.h>
#include <net/arp.h>
#include "doge_network.h"
#include "doge_eth.h"
#include "doge_arptable.h"
#include "doge_data.h"

#define DOGE_ARP_REQUEST ARPOP_REQUEST
#define DOGE_ARP_REPLY ARPOP_REPLY
#define DOGE_ARP_PROTO ETH_P_ARP

struct doge_packet;

struct doge_arphdr {
       __be16 hw_addr_format;
       __be16 pr_addr_format;
       u8 hw_addr_len;
       u8 pr_addr_len;
       u16 opcode;
       unsigned char src_hw_addr[ETH_LEN];
       __be32 src_proto_addr;
       unsigned char dest_hw_addr[ETH_LEN];
       __be32 dest_proto_addr;
};

struct doge_arphdr *doge_create_arphdr(void);
void doge_darphdr_to_karphdr(struct doge_arphdr *darphdr, struct arphdr *karphdr);
void doge_karphdr_to_darphdr(struct arphdr *karphdr, struct doge_arphdr *darphdr);
void doge_arp_request(__be32 ip);
void doge_handle_arp(struct doge_packet *dpkt);

#endif 
