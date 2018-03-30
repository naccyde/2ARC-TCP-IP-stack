#include "doge_network.h"

/* doge_create_packet : Create and return an empty doge_packet */
struct doge_packet *doge_create_packet(void)
{
        struct doge_packet *pkt = kmalloc(sizeof(struct doge_packet), GFP_KERNEL);

        pkt->dev = NULL;
        pkt->thdr.raw = NULL;
        pkt->nhdr.raw = NULL;
        pkt->lhdr = NULL;
        pkt->data = NULL;
        pkt->data_len = 0;
        pkt->th_proto = 0;
        pkt->nh_proto = 0;
        pkt->th_size = 0;
        pkt->nh_size = 0;
        pkt->lh_size = 0;

        return pkt;
}

void doge_send(struct doge_packet *dpkt)
{
        struct sk_buff *skb = doge_packet_to_sk_buff(dpkt);

        printk("Sending\n");
        if (skb != NULL) 
                dev_queue_xmit(skb);

        doge_free_packet(dpkt);
}

/* doge_free_packet : Free all allocated space in a doge_packet */
void doge_free_packet(struct doge_packet *dpkt)
{
        if (dpkt->thdr.raw != NULL) {
                if (dpkt->th_proto == DOGE_ICMP_PROTO && dpkt->thdr.icmphdr->data != NULL) { 
                        //kfree(dpkt->thdr.icmphdr->data);
                }
                kfree(dpkt->thdr.raw);
        }

        if (dpkt->nhdr.raw != NULL)
                kfree(dpkt->nhdr.raw);
        
        kfree(dpkt->lhdr);
        kfree(dpkt);
}
