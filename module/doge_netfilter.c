#include "doge_netfilter.h"

static struct nf_hook_ops arp_hook;
static struct nf_hook_ops ip_hook;

static void handle_packets(struct sk_buff *skb, struct net_device *dev)
{
        struct doge_packet *dpkt = sk_buff_to_doge_packet(skb, dev);
        
        if (dpkt == NULL)
                return;

        switch (dpkt->lhdr->nh_proto) {
                case DOGE_ARP_PROTO:
                        doge_handle_arp(dpkt);
                        break;
                case DOGE_IP_PROTO:
                        doge_handle_ip(dpkt);
                        break;
        }

        doge_free_packet(dpkt);
}

/* arp_pkt_handling : function that handle each received ARP packet */
static unsigned int arp_pkt_handling(unsigned int hooknum, struct sk_buff *skb, struct net_device *in, const struct net_device *out, int (*okfn)(struct sk_buff *))
{
        handle_packets(skb, in);
        return NF_DROP;
}

/* ip_pkt_handling : function that handle each received IP packet */
static unsigned int ip_pkt_handling(unsigned int hooknum, struct sk_buff *skb, struct net_device *in, const struct net_device *out, int (*okfn)(struct sk_buff *))
{
        handle_packets(skb, in);
        return NF_DROP;
}

/* arp_hook_init : prepare Netfilter ARP hook */
static void arp_hook_init(void)
{
        arp_hook.hook = (nf_hookfn *)arp_pkt_handling;
        arp_hook.hooknum = NF_INET_PRE_ROUTING;
        arp_hook.pf = NFPROTO_ARP;
        arp_hook.priority = NF_IP_PRI_FIRST;

        nf_register_hook(&arp_hook);
}

/* ip_hook_init : prepare Netfilter IP hook */
static void ip_hook_init(void)
{
        ip_hook.hook = (nf_hookfn *)ip_pkt_handling;
        ip_hook.hooknum = NF_INET_PRE_ROUTING;
        ip_hook.pf = NFPROTO_IPV4;
        ip_hook.priority = NF_IP_PRI_FIRST;

        nf_register_hook(&ip_hook);
}

/* arp_hook_clean : clean ARP hook messes */
static void arp_hook_clean(void)
{
        nf_unregister_hook(&arp_hook);
}

/* ip_hook_clean : clean IP hook messes */
static void ip_hook_clean(void)
{
        nf_unregister_hook(&ip_hook);
}

/* hook_init : Init and register hooks */
void hook_init(void)
{
        arp_hook_init();
        ip_hook_init();
}

/* hook_clean : Clean hooks messes */
void hook_clean(void)
{
        arp_hook_clean();
        ip_hook_clean();
}

