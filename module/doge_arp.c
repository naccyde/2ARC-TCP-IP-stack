#include "doge_arp.h"

/* doge_arp_reply : Send an ARP response to dpkt */
static void doge_arp_reply(struct doge_packet *dpkt)
{
        struct doge_arphdr *arphdr;
        struct doge_packet *reply;

        if (be32_IP != dpkt->nhdr.arphdr->dest_proto_addr)
                return;

        /* Prepare reply */
        reply = doge_create_packet();
        reply->lhdr = doge_create_ethhdr();
        doge_fill_ethhdr(reply->lhdr, dpkt->dev->dev_addr, &dpkt->lhdr->src_mac[0], DOGE_ARP_PROTO);

        reply->dev = dpkt->dev;
        reply->nh_proto = dpkt->nh_proto;
        reply->nh_size = dpkt->nh_size;
        reply->lh_size = dpkt->lh_size;

        /* ARPHDR */
        arphdr = doge_create_arphdr();
        arphdr->opcode = DOGE_ARP_REPLY;
        memcpy(&arphdr->src_hw_addr[0], dpkt->dev->dev_addr, ETH_LEN);
        arphdr->src_proto_addr = be32_IP;
        memcpy(&arphdr->dest_hw_addr[0], &dpkt->nhdr.arphdr->src_hw_addr[0], ETH_LEN);
        arphdr->dest_proto_addr = dpkt->nhdr.arphdr->src_proto_addr;

        reply->nhdr.arphdr = arphdr;

        doge_send(reply);
}

/* doge_create_arphdr : Create and prefiled arphdr */
struct doge_arphdr *doge_create_arphdr(void)
{
        struct doge_arphdr *arphdr = (struct doge_arphdr *)kmalloc(sizeof(struct doge_arphdr), GFP_KERNEL);

        arphdr->hw_addr_format = htons(1);
        arphdr->pr_addr_format = htons(DOGE_IP_PROTO);
        arphdr->hw_addr_len = ETH_LEN;
        arphdr->pr_addr_len = 4;
        arphdr->opcode = 0;

        memset(&arphdr->src_hw_addr[0], 0, ETH_LEN);
        arphdr->src_proto_addr = 0;
        memset(&arphdr->dest_hw_addr[0], 0, ETH_LEN);
        arphdr->dest_proto_addr = 0;

        return arphdr;
}

/* doge_darphdr_to_karphdr : Convert a doge arphdr to a kernel arphdr */
void doge_darphdr_to_karphdr(struct doge_arphdr *darphdr, struct arphdr *karphdr)
{
        unsigned char *arp_ptr;
        arp_ptr = (unsigned char *)(karphdr + 1);

        karphdr->ar_hrd = darphdr->hw_addr_format;
        karphdr->ar_pro = darphdr->pr_addr_format;
        karphdr->ar_hln = darphdr->hw_addr_len;
        karphdr->ar_pln = darphdr->pr_addr_len;
        karphdr->ar_op = htons(darphdr->opcode);

        memcpy(arp_ptr, &darphdr->src_hw_addr[0], ETH_LEN);
        arp_ptr += ETH_LEN;
        memcpy(arp_ptr, &darphdr->src_proto_addr, 4);
        arp_ptr += 4;
        memcpy(arp_ptr, &darphdr->dest_hw_addr[0], ETH_LEN);
        arp_ptr += ETH_LEN;
        memcpy(arp_ptr, &darphdr->dest_proto_addr, ETH_LEN);
}

/* doge_karphdr_to_darphdr : Convert a kernel arphdr to a doge arphdr */
void doge_karphdr_to_darphdr(struct arphdr *karphdr, struct doge_arphdr *darphdr)
{
        unsigned char *arp_ptr;
        arp_ptr = (unsigned char *)(karphdr + 1);

        darphdr->hw_addr_format = karphdr->ar_hrd;
        darphdr->pr_addr_format = karphdr->ar_pro;
        darphdr->hw_addr_len = karphdr->ar_hln;
        darphdr->pr_addr_len = karphdr->ar_pln;
        darphdr->opcode = ntohs(karphdr->ar_op);

        memcpy(&darphdr->src_hw_addr, arp_ptr, ETH_LEN);
        arp_ptr += ETH_LEN;
        memcpy(&darphdr->src_proto_addr, arp_ptr, 4);
        arp_ptr += 4;
        memcpy(&darphdr->dest_hw_addr, arp_ptr, ETH_LEN);
        arp_ptr += ETH_LEN;
        memcpy(&darphdr->dest_proto_addr, arp_ptr, 4);
}

/* doge_arp_request : Send an ARP request to IP */
void doge_arp_request(__be32 ip)
{
        struct doge_arphdr *arphdr;
        unsigned char bdcast_addr[ETH_LEN] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
        struct net_device *dev = dev_get_by_name(&init_net, IFACE);
        struct doge_packet *dpkt = doge_create_packet();

        dpkt->lhdr = doge_create_ethhdr();
        doge_fill_ethhdr_bdcast(dpkt->lhdr, dev->dev_addr, DOGE_ARP_PROTO);
        
        dpkt->dev = dev;
        dpkt->nh_proto = DOGE_ARP_PROTO;
        dpkt->nh_size = sizeof(struct arphdr) + 2 * ETH_LEN + 2 *4;
        dpkt->lh_size = sizeof(struct ethhdr);
       
        /* ARPHDR */
        arphdr = doge_create_arphdr();
        arphdr->opcode = DOGE_ARP_REQUEST;
        memcpy(&arphdr->src_hw_addr[0], &dev->dev_addr[0], ETH_LEN);
        arphdr->src_proto_addr = be32_IP;
        memcpy(&arphdr->dest_hw_addr[0], &bdcast_addr[0], ETH_LEN);
        arphdr->dest_proto_addr = ip;

        dpkt->nhdr.arphdr = arphdr;
        doge_send(dpkt);
}

/* doge_handle_arp : Main function, handle each received ARP packet */
void doge_handle_arp(struct doge_packet *dpkt)
{
        struct doge_arphdr *arphdr = dpkt->nhdr.arphdr;

        switch (arphdr->opcode) {
                case DOGE_ARP_REQUEST:
                        doge_arp_reply(dpkt);
                        break;

                case DOGE_ARP_REPLY:
                        doge_arptable_add(arphdr->src_proto_addr, &arphdr->src_hw_addr[0]);
                        break;
        }
}
