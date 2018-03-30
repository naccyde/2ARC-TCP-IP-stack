#ifndef _DOGE_NETHEAP_H
#define _DOGE_NETHEAP_H

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/netlink.h>

struct net_item {
        struct net_item *prvs;
        struct nlmsghdr *nlh;
        int pid;
        struct net_item *next;
};

void init_heap(void);
void add_heap(struct nlmsghdr *nlh, int pid);
void rmv_heap(int pid);
void heap_clean(void);

#endif
