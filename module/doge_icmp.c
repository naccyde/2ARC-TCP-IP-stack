#include "doge_icmp.h"

static void doge_icmp_echoreply(struct doge_packet *dpkt)
{
        struct doge_icmphdr *icmphdr;
        struct doge_packet *reply;

        /* Prepare reply */
        reply = doge_create_packet();
        reply->lhdr = doge_create_ethhdr();
        doge_fill_ethhdr(reply->lhdr, dpkt->dev->dev_addr, &dpkt->lhdr->src_mac[0], DOGE_IP_PROTO);

        reply->dev = dpkt->dev;
        reply->lh_size = sizeof(struct ethhdr);

        /* IPHDR */
        reply->nh_proto = DOGE_IP_PROTO;
        reply->nh_size = sizeof(struct iphdr);
        reply->nhdr.iphdr = doge_create_iphdr();
        doge_fill_iphdr(reply->nhdr.iphdr, dpkt->nhdr.iphdr->dest_ip, dpkt->nhdr.iphdr->src_ip, DOGE_ICMP_PROTO);

        /* ICMPHDR */
        reply->th_proto = DOGE_ICMP_PROTO;
        reply->th_size = dpkt->th_size;
        icmphdr = doge_create_icmphdr();
        icmphdr->type = DOGE_ICMP_TYPE_ECHOREPLY;
        icmphdr->id = dpkt->thdr.icmphdr->id;
        icmphdr->sequence = dpkt->thdr.icmphdr->sequence;
        icmphdr->code = 0;
        icmphdr->data = dpkt->thdr.icmphdr->data;
        icmphdr->data_len = dpkt->thdr.icmphdr->data_len;

        reply->thdr.icmphdr = icmphdr;
        reply->nhdr.iphdr->tot_len = htons(sizeof(struct icmphdr) + icmphdr->data_len + dpkt->nh_size);
        
        doge_send(reply);
}

struct doge_icmphdr *doge_create_icmphdr(void)
{
        struct doge_icmphdr *icmphdr = (struct doge_icmphdr *)kmalloc(sizeof(struct doge_icmphdr), GFP_KERNEL);

        icmphdr->type = 0;
        icmphdr->code = 0;
        icmphdr->id = 0;
        icmphdr->sequence = 0;
        icmphdr->data = NULL;
        icmphdr->data_len = 0;

        return icmphdr;
}

void doge_kicmphdr_to_dicmphdr(int len, struct icmphdr *kicmphdr, struct doge_icmphdr *dicmphdr)
{
        unsigned char *data_ptr = (unsigned char *)(kicmphdr + 1);

        dicmphdr->type = kicmphdr->type;
        dicmphdr->code = kicmphdr->code;
        dicmphdr->id = kicmphdr->un.echo.id;
        dicmphdr->sequence = kicmphdr->un.echo.sequence;
        
        if (len > sizeof(struct icmphdr)) {
                dicmphdr->data_len = len - sizeof(struct icmphdr);
                dicmphdr->data = (unsigned char *)kmalloc(dicmphdr->data_len, GFP_KERNEL);
                memcpy(dicmphdr->data, data_ptr, dicmphdr->data_len);
        }
}

void doge_dicmphdr_to_kicmphdr(struct doge_icmphdr *dicmphdr, struct icmphdr *kicmphdr)
{
        unsigned char *data_ptr = (unsigned char *)(kicmphdr + 1);

        kicmphdr->type = dicmphdr->type;
        kicmphdr->code = dicmphdr->code;
        kicmphdr->un.echo.id = dicmphdr->id;
        kicmphdr->un.echo.sequence = dicmphdr->sequence;
        kicmphdr->checksum = 0;

        if (dicmphdr->data_len > 0)
                memcpy(data_ptr, dicmphdr->data, dicmphdr->data_len); 
}

void doge_handle_icmp(struct doge_packet *dpkt)
{
        struct doge_icmphdr *icmphdr = dpkt->thdr.icmphdr;

        switch (icmphdr->type) {
                case DOGE_ICMP_TYPE_ECHO:
                        doge_icmp_echoreply(dpkt);
                        return;
        }
}
