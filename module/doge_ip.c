#include "doge_ip.h"

struct doge_iphdr *doge_create_iphdr(void)
{
        struct doge_iphdr *iphdr = (struct doge_iphdr *)kmalloc(sizeof(struct doge_iphdr), GFP_KERNEL);

        iphdr->version = 4;
        iphdr->ihl = 5;
        iphdr->tos = 0;
        iphdr->tot_len = 0;
        iphdr->id = 0;
        iphdr->frag_offset = 0;
        iphdr->ttl = 64;
        iphdr->th_proto = 0;
        iphdr->src_ip = 0;
        iphdr->dest_ip = 0;

        return iphdr;
}

void doge_fill_iphdr(struct doge_iphdr *iphdr, __be32 src_ip, __be32 dest_ip, int th_proto)
{
        iphdr->src_ip = src_ip;
        iphdr->dest_ip = dest_ip;
        iphdr->th_proto = th_proto;
}

void doge_diphdr_to_kiphdr(struct doge_iphdr *dip, struct iphdr *kip)
{
        kip->version = dip->version;
        kip->ihl = dip->ihl;
        kip->tos = dip->tos;
        kip->tot_len = dip->tot_len;
        kip->id = dip->id;
        kip->frag_off = dip->frag_offset;
        kip->ttl = dip->ttl;
        kip->protocol = dip->th_proto;
        kip->check = 0;
        kip->saddr = dip->src_ip;
        kip->daddr = dip->dest_ip;

        kip->check = ip_fast_csum((unsigned char *)kip, kip->ihl);
}

void doge_kiphdr_to_diphdr(struct iphdr *kip, struct doge_iphdr *dip)
{
        dip->version = kip->version;
        dip->ihl = kip->ihl;
        dip->tos = kip->tos;
        dip->id = kip->id;
        dip->frag_offset = kip->frag_off;
        dip->ttl = kip->ttl;
        dip->th_proto = kip->protocol;
        dip->src_ip = kip->saddr;
        dip->dest_ip = kip->daddr;
}

void doge_handle_ip(struct doge_packet *dpkt)
{
        struct doge_iphdr *iphdr = dpkt->nhdr.iphdr;

        switch (iphdr->th_proto) {
                case DOGE_ICMP_PROTO:
                        doge_handle_icmp(dpkt);
                        break;
                case DOGE_TCP_PROTO:
                        doge_handle_tcp(dpkt);
                        break;
        }
}

