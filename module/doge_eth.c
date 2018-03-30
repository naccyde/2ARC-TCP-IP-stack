#include "doge_eth.h"

static unsigned char eth_bdcast[ETH_LEN] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

struct doge_ethhdr *doge_create_ethhdr(void)
{
        struct doge_ethhdr *ethhdr = (struct doge_ethhdr *)kmalloc(sizeof(struct doge_ethhdr), GFP_KERNEL);

        memset(&ethhdr->src_mac[0], 0, ETH_LEN);
        memset(&ethhdr->dest_mac[0], 0, ETH_LEN);
        ethhdr->nh_proto = 0;

        return ethhdr;
}

void doge_fill_ethhdr(struct doge_ethhdr *ethhdr, unsigned char *src, unsigned char *dest, int nh_proto)
{
        memcpy(&ethhdr->src_mac[0], src, ETH_LEN);
        memcpy(&ethhdr->dest_mac[0], dest, ETH_LEN);
        ethhdr->nh_proto = nh_proto;               
}

void doge_fill_ethhdr_bdcast(struct doge_ethhdr *ethhdr, unsigned char *src, int nh_proto)
{
        doge_fill_ethhdr(ethhdr, src, &eth_bdcast[0], nh_proto);
}

void doge_dethhdr_to_kethhdr(struct doge_ethhdr *deth, struct ethhdr *keth)
{         
        memcpy(&keth->h_dest[0], &deth->dest_mac[0], ETH_LEN);       
        memcpy(&keth->h_source[0], &deth->src_mac[0], ETH_LEN);

        keth->h_proto = htons(deth->nh_proto);
}

void doge_kethhdr_to_dethhdr(struct ethhdr *keth, struct doge_ethhdr *deth)
{
        memcpy(&deth->dest_mac[0], &keth->h_dest[0], ETH_LEN);
        memcpy(&deth->src_mac[0], &keth->h_source[0], ETH_LEN);

        deth->nh_proto = ntohs(keth->h_proto);
}
