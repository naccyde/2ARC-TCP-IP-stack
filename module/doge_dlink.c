#include "doge_dlink.h"

static int fill_nh(int nh_proto, struct doge_packet *dpkt, struct sk_buff *skb)
{
        switch (nh_proto) {
                case DOGE_IP_PROTO:
                        dpkt->nh_size = sizeof(struct iphdr);
                        dpkt->nhdr.iphdr = doge_create_iphdr();
                        doge_kiphdr_to_diphdr(ip_hdr(skb), dpkt->nhdr.iphdr);
                        dpkt->th_proto = dpkt->nhdr.iphdr->th_proto;
                        return 1;
                case DOGE_ARP_PROTO:
                        dpkt->nh_size = arp_hdr_len(dpkt->dev);
                        dpkt->nhdr.arphdr = doge_create_arphdr();
                        doge_karphdr_to_darphdr(arp_hdr(skb), dpkt->nhdr.arphdr);
                        return 1;
        }

        return 0;
}

static int fill_th(int th_proto, struct doge_packet *dpkt, struct sk_buff *skb)
{
        struct iphdr *iphdr;

        switch (th_proto) {
                case DOGE_TCP_PROTO:
                        dpkt->th_size = sizeof(struct tcphdr);
                        dpkt->thdr.tcphdr = doge_create_tcphdr();
                        doge_ktcphdr_to_dtcphdr(tcp_hdr(skb), dpkt->thdr.tcphdr);
                        return 1;
                case DOGE_ICMP_PROTO:
                        iphdr = ip_hdr(skb);
                        dpkt->th_size = ntohs(iphdr->tot_len) - (iphdr->ihl << 2);
                        dpkt->thdr.icmphdr = doge_create_icmphdr();
                        doge_kicmphdr_to_dicmphdr(dpkt->th_size, icmp_hdr(skb), dpkt->thdr.icmphdr);
                        return 1;
        }

        return 0;
}

static void fill_data(struct doge_packet *dpkt, struct sk_buff *skb)
{
        dpkt->data_len = skb->data_len;
        
        if (dpkt->data_len) {
                dpkt->data = (unsigned char *)kmalloc(sizeof(skb->data_len), GFP_KERNEL);
                memcpy(dpkt->data, skb->data, skb->data_len);
        }
}

struct doge_packet *sk_buff_to_doge_packet(struct sk_buff *skb, struct net_device *dev)
{
        struct doge_packet *dpkt = doge_create_packet();
        struct ethhdr *keth = eth_hdr(skb);

        dpkt->dev = dev;
        dpkt->lhdr = (struct doge_ethhdr *)kmalloc(sizeof(struct doge_ethhdr), GFP_KERNEL);
        dpkt->nh_proto = ntohs(keth->h_proto);
        doge_kethhdr_to_dethhdr(keth, dpkt->lhdr);

        if (! fill_nh(dpkt->nh_proto, dpkt, skb))
                return dpkt;

        if (! fill_th(dpkt->th_proto, dpkt, skb))
                return dpkt;
        
        fill_data(dpkt, skb);

        return dpkt;
}
