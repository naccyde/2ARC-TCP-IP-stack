#include "doge_tcp_connections.h"

static struct doge_tcp_connection **tcp_table;

static int hash(__be32 ip)
{
        return ip % 100;
}

void doge_tcpc_init(void)
{
        tcp_table = (struct doge_tcp_connection **)kmalloc(TCP_TABLE_SIZE * sizeof(struct doge_tcp_connect *), GFP_KERNEL);
        memset(tcp_table, 0, TCP_TABLE_SIZE * sizeof(struct doge_tcp_connect *));
}

void doge_tcpc_add(struct doge_tcp_connection *tcpc)
{        
        struct doge_tcp_connection *list = tcp_table[hash(tcpc->ip)];

        if (list == NULL) {
                tcp_table[hash(tcpc->ip)] = tcpc;
                return;
        }

        while (list->next != NULL)
                list = list->next;

        list->next = tcpc;
}

void doge_tcpc_rmv(struct doge_tcp_connection *tcpc)
{
        struct doge_tcp_connection *tmp = NULL;
        struct doge_tcp_connection *list = tcp_table[hash(tcpc->ip)];

        if (list == NULL)
                return;

        /* First in list */
        if (list->ip == tcpc->ip && list->port == tcpc->port) {
                tcp_table[hash(tcpc->ip)] = list->next;
                kfree(list);
                return;
        }

        /* Cross list */
        while (list->next != NULL && list->next->ip != tcpc->ip && list->next->port != tcpc->port)
                list = list->next;
        
      
        if (list->next == NULL)
                return;

        tmp = list;
        tmp->next = list->next;
        kfree(list);
}

struct doge_tcp_connection *doge_tcpc_get(__be32 ip, __be16 port)
{
        struct doge_tcp_connection *list = tcp_table[hash(ip)];

        if (list == NULL)
                return NULL;

        while (list->ip != ip && list->port != port && list->next != NULL)
                list = list->next;
       
        if (list->ip != ip && list->port != port)
                return NULL;

        return list;
}

void doge_tcpc_clean(void)
{
        int i;
        struct doge_tcp_connection *list = NULL;
        struct doge_tcp_connection *tmp = NULL;

        for (i = 0; i < TCP_TABLE_SIZE; ++i) {
                if (! (list = tcp_table[i]))
                        return;
                
                while (list != NULL) {
                        tmp = list;
                        list = list->next;
                        kfree(tmp);
                }
        }

        kfree(tcp_table);
}
