#ifndef _DOGE_TCP_CONNECTION_H
#define _DOGE_TCP_CONNECTION_H

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/string.h>

#define TCP_TABLE_SIZE 100

struct doge_tcp_connection {
        u8 state;
        __be32 ip;
        __be16 port;
        struct doge_tcp_connection *next;
};

void doge_tcpc_init(void);
void doge_tcpc_add(struct doge_tcp_connection *tcpc);
void doge_tcpc_rmv(struct doge_tcp_connection *tcpc);
struct doge_tcp_connection *doge_tcpc_get(__be32 ip, __be16 port);
void doge_tcpc_clean(void);

#endif
