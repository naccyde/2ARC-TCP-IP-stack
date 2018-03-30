#ifndef _DOGE_TCP_H
#define _DOGE_TCP_H

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/tcp.h>
#include <linux/types.h>
#include "doge_network.h"
#include "doge_tcp_connections.h"

#define DOGE_TCP_PROTO IPPROTO_TCP
#define DOGE_TCP_CLOSED 0
#define DOGE_TCP_LISTEN 1
#define DOGE_TCP_SYN_RECVD 2
#define DOGE_TCP_ESTAB 3
#define DOGE_TCP_CLOSE_WAIT 4
#define DOGE_TCP_LAST_ACK 5
#define DOGE_TCP_SYN_SENT 6

#define DOGE_TCP_CWR (1 << 7)
#define DOGE_TCP_ECE (1 << 6)
#define DOGE_TCP_URG (1 << 5)
#define DOGE_TCP_ACK (1 << 4)
#define DOGE_TCP_PSH (1 << 3)
#define DOGE_TCP_RST (1 << 2)
#define DOGE_TCP_SYN (1 << 1)
#define DOGE_TCP_FIN (1)

struct doge_packet;

struct doge_tcphdr {
        __be16 src_port;
        __be16 dest_port;
        __be32 seq_number;
        __be32 ack_number;
        u8 data_offset:4;
        u8 reserved:4;
        u8 cwr:1;
        u8 ece:1;
        u8 urg:1;
        u8 ack:1;
        u8 psh:1;
        u8 rst:1;
        u8 syn:1;
        u8 fin:1;
        __be16 window;
        __be16 urg_ptr;
};

struct doge_tcphdr *doge_create_tcphdr(void);
void doge_dtcphdr_to_ktcphdr(struct doge_tcphdr *dtcp, struct tcphdr *ktcp);
void doge_ktcphdr_to_dtcphdr(struct tcphdr *ktcp, struct doge_tcphdr *dtcp);
void doge_handle_tcp(struct doge_packet *dpkt);
void doge_tcp_send(__be32 ip, __be16 port, char *data, int data_len);

#endif
