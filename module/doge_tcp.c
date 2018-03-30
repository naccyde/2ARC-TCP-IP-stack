#include "doge_tcp.h"

struct doge_tcphdr *doge_create_tcphdr(void)
{
        struct doge_tcphdr *tcphdr = (struct doge_tcphdr *)kmalloc(sizeof(struct doge_tcphdr), GFP_KERNEL);

        memset(tcphdr, 0, sizeof(struct doge_tcphdr));

        return tcphdr;
}

void doge_dtcphdr_to_ktcphdr(struct doge_tcphdr *dtcp, struct tcphdr *ktcp)
{
        ktcp->source = dtcp->src_port;
        ktcp->dest = dtcp->dest_port;
        ktcp->seq = dtcp->seq_number;
        ktcp->ack_seq = dtcp->ack_number;
        ktcp->doff = dtcp->data_offset;
        ktcp->res1 = dtcp->reserved;
        ktcp->cwr = dtcp->cwr;
        ktcp->ece = dtcp->ece;
        ktcp->urg = dtcp->urg;
        ktcp->ack = dtcp->ack;
        ktcp->psh = dtcp->psh;
        ktcp->rst = dtcp->rst;
        ktcp->syn = dtcp->syn;
        ktcp->fin = dtcp->fin;
        ktcp->window = dtcp->window;
        ktcp->check = 0;
        ktcp->urg_ptr = dtcp->urg_ptr;
}

void doge_ktcphdr_to_dtcphdr(struct tcphdr *ktcp, struct doge_tcphdr *dtcp)
{
        dtcp->src_port = ktcp->source;
        dtcp->dest_port = ktcp->dest;
        dtcp->seq_number = ktcp->seq;
        dtcp->ack_number = ktcp->ack_seq;
        dtcp->data_offset = ktcp->doff;
        dtcp->reserved = ktcp->res1;
        dtcp->cwr = ktcp->cwr;
        dtcp->ece = ktcp->ece;
        dtcp->urg = ktcp->urg;
        dtcp->ack = ktcp->ack;
        dtcp->psh = ktcp->psh;
        dtcp->rst = ktcp->rst;
        dtcp->syn = ktcp->syn;
        dtcp->fin = ktcp->fin;
        dtcp->window = ktcp->window;
        dtcp->urg_ptr = ktcp->urg_ptr;        
}

void doge_handle_tcpsyn(struct doge_packet *dpkt)
{
        struct doge_tcphdr *tcphdr;
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
        doge_fill_iphdr(reply->nhdr.iphdr, dpkt->nhdr.iphdr->dest_ip, dpkt->nhdr.iphdr->src_ip, DOGE_TCP_PROTO);
        
        /* TCPHDR */
        reply->th_proto = DOGE_TCP_PROTO;
        reply->th_size = sizeof(struct tcphdr);
        tcphdr = doge_create_tcphdr();
        tcphdr->src_port = htons(22);
        tcphdr->dest_port = dpkt->thdr.tcphdr->src_port;
        tcphdr->seq_number = htonl(21415);
        tcphdr->ack_number = htonl(ntohl(dpkt->thdr.tcphdr->seq_number) + 1);
        tcphdr->data_offset = 5;
        tcphdr->ack = 1;
        tcphdr->syn = 1;
        tcphdr->window = dpkt->thdr.tcphdr->window;
        tcphdr->urg_ptr = htons(0);
       
        reply->thdr.tcphdr = tcphdr;
        reply->nhdr.iphdr->tot_len = htons(reply->nh_size + reply->th_size); 
        doge_send(reply);
}

static struct doge_packet *create_prefilled_packet(struct doge_packet *dpkt)
{
        struct doge_tcphdr *tcphdr;
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
        doge_fill_iphdr(reply->nhdr.iphdr, dpkt->nhdr.iphdr->dest_ip, dpkt->nhdr.iphdr->src_ip, DOGE_TCP_PROTO);
        
        /* TCPHDR */
        reply->th_proto = DOGE_TCP_PROTO;
        tcphdr = doge_create_tcphdr();
        tcphdr->src_port = dpkt->thdr.tcphdr->dest_port;
        tcphdr->dest_port = dpkt->thdr.tcphdr->src_port;
        tcphdr->seq_number = dpkt->thdr.tcphdr->ack_number;
        tcphdr->ack_number = htonl(ntohl(dpkt->thdr.tcphdr->seq_number) + 1);
        tcphdr->data_offset = 5;
        tcphdr->window = dpkt->thdr.tcphdr->window;
        tcphdr->urg_ptr = htons(0);
       
        reply->thdr.tcphdr = tcphdr;

        return reply;
}

static void new_connection(struct doge_packet *dpkt)
{
        struct doge_packet *reply = create_prefilled_packet(dpkt);
        struct doge_tcphdr *tcphdr = reply->thdr.tcphdr;
        struct doge_tcp_connection *tcpc = (struct doge_tcp_connection *)kmalloc(sizeof(struct doge_tcp_connection), GFP_KERNEL);
        
        reply->th_size = sizeof(struct tcphdr);

        tcphdr->seq_number = 200;
        tcphdr->syn = 1;
        tcphdr->ack = 1;
        tcphdr->window = dpkt->thdr.tcphdr->window;
        
        tcpc->state = DOGE_TCP_SYN_RECVD;
        tcpc->ip = reply->nhdr.iphdr->dest_ip;
        tcpc->port = tcphdr->dest_port;
        tcpc->next = NULL;
        doge_tcpc_add(tcpc);
      
        reply->nhdr.iphdr->tot_len = htons(reply->nh_size + reply->th_size); 
        doge_send(reply);
}

static void close_finack(struct doge_packet *dpkt, struct doge_tcp_connection *tcpc)
{
        struct doge_packet *reply = create_prefilled_packet(dpkt);
        struct doge_tcphdr *tcphdr = reply->thdr.tcphdr;
     
        reply->th_size = sizeof(struct tcphdr);

        tcphdr->ack = 1;
        tcphdr->window = dpkt->thdr.tcphdr->window;

        doge_tcpc_rmv(tcpc);

        reply->nhdr.iphdr->tot_len = htons(reply->nh_size + reply->th_size);
        doge_send(reply);
}

void doge_tcp_send(__be32 ip, __be16 port, char *data, int data_len)
{
}


void doge_handle_tcp(struct doge_packet *dpkt)
{
        struct doge_iphdr *iphdr = dpkt->nhdr.iphdr;
        struct doge_tcphdr *tcphdr = dpkt->thdr.tcphdr;
        struct doge_tcp_connection *tcpc = doge_tcpc_get(iphdr->src_ip, tcphdr->src_port);
        u8 flags = (tcphdr->cwr << 7) + (tcphdr->urg << 6) + (tcphdr->urg << 5) + (tcphdr->ack << 4) + (tcphdr->psh << 3) + (tcphdr->rst << 2) + (tcphdr->syn << 1) + (tcphdr->fin);

        if (! tcpc && flags != DOGE_TCP_SYN) {
                return;
        } else if (! tcpc && flags == DOGE_TCP_SYN) {
                printk("Syn ack\n");
                new_connection(dpkt);
                return;
        }

        switch (tcpc->state) {
                case DOGE_TCP_SYN_RECVD:
                        if (flags == DOGE_TCP_ACK)
                                tcpc->state = DOGE_TCP_ESTAB;
                        printk("ESTAB\n");
                        break;
                case DOGE_TCP_ESTAB:
                        if (flags == (DOGE_TCP_FIN | DOGE_TCP_ACK))
                                close_finack(dpkt, tcpc);
                        printk("Fin ack\n");
                        break;
                case DOGE_TCP_CLOSE_WAIT:
                        break;
                case DOGE_TCP_LAST_ACK:
                        break;
                case DOGE_TCP_CLOSED:
                        break;
        }

        
}
