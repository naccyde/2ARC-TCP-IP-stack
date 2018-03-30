#include "doge_klink.h"

/* compute_cksm : Compute chcksum for IP / TCP headers */
static void compute_cksm(struct sk_buff *skb)
{
        struct iphdr *iph = ip_hdr(skb);
        struct tcphdr *tcph = tcp_hdr(skb);
        int tcp_len = skb->len - (iph->ihl << 2);

        tcph->check = tcp_v4_check(tcp_len, iph->saddr, iph->daddr, csum_partial(tcph, tcp_len, 0));
        skb->ip_summed = CHECKSUM_NONE;
}

/* fill_th : Fill the given tranport header with the doge_packet transport header */
static int fill_th(int th_proto, struct doge_packet *dpkt, unsigned char *kthdr)
{
        __wsum csum;
        struct icmphdr *icmphdr;

        switch(th_proto) {
                case DOGE_TCP_PROTO:
                        doge_dtcphdr_to_ktcphdr(dpkt->thdr.tcphdr, (struct tcphdr *)kthdr);
                        return 1;
                case DOGE_ICMP_PROTO:
                        icmphdr = (struct icmphdr *)kthdr;
                        doge_dicmphdr_to_kicmphdr(dpkt->thdr.icmphdr, icmphdr);
                        csum = csum_partial(icmphdr, dpkt->th_size, 0);
                        icmphdr->checksum = csum_fold(csum);
                        return 1;
        }

        return 0;
}

/* fill_nh : Fill the given network header with the doge_packet network header */
static int fill_nh(int nh_proto, struct doge_packet *dpkt, unsigned char *knhdr)
{
        switch(nh_proto) {
                case DOGE_IP_PROTO:
                        doge_diphdr_to_kiphdr(dpkt->nhdr.iphdr, (struct iphdr *)knhdr);
                        return 1;
                case DOGE_ARP_PROTO:
                        doge_darphdr_to_karphdr(dpkt->nhdr.arphdr, (struct arphdr *)knhdr);
                        return 1;
        }

        return 0;
}

/* doge_packet_to_sk_buff : Convert a doge_packet to a kernel sk_buff */
struct sk_buff *doge_packet_to_sk_buff(struct doge_packet *dpkt)
{
        struct sk_buff *skb;
        unsigned char *thdr;
        unsigned char *nhdr;
        int len = 0 + LL_RESERVED_SPACE(dpkt->dev) + dpkt->dev->needed_tailroom + dpkt->th_size + dpkt->nh_size + dpkt->lh_size;

        if (! (skb = alloc_skb(1500, NET_IP_ALIGN))) {
                doge_print(KERN_ERR, "Can't allocate skb", NULL, __FUNCTION__);
                return NULL;
        }
     
        skb_reserve(skb, len);
        skb->dev = dpkt->dev;
        skb->protocol = dpkt->nh_proto;

        /* Transport header */
        thdr = (unsigned char *)skb_push(skb, dpkt->th_size);
        skb_reset_transport_header(skb);
        fill_th(dpkt->th_proto, dpkt, thdr);
        
        /* Network header */
        nhdr = (unsigned char *)skb_push(skb, dpkt->nh_size);
        skb_reset_network_header(skb);
        fill_nh(dpkt->nh_proto, dpkt, nhdr);

        if (skb_is_nonlinear(skb))
                skb_linearize(skb);

        if (dpkt->th_proto == DOGE_TCP_PROTO)
                compute_cksm(skb);

        /* Link header */
        dev_hard_header(skb, dpkt->dev, dpkt->nh_proto, &dpkt->lhdr->dest_mac[0], &dpkt->lhdr->src_mac[0], dpkt->dev->addr_len);

        return skb;
}
