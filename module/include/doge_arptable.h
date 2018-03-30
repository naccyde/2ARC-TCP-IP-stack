#ifndef _DOGE_ARPTABLE_H
#define _DOGE_ARPTABLE_H

#include <linux/kernel.h>
#include <linux/module.h>
#include "doge_eth.h"

#define ARPTABLE_SIZE 100

struct arptable_entry {
        __be32 ip;
        unsigned char mac[ETH_LEN];
        struct arptable_entry *next;
};

void doge_arptable_init(void);
void doge_arptable_add(__be32 new_ip, const unsigned char *new_mac);
unsigned char *doge_arptable_get(__be32 ip);
void doge_arptable_clean(void);

#endif
