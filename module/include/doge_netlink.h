#ifndef _DOGE_NETLINK_H
#define _DOGE_NETLINK_H

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/netlink.h>
#include <linux/skbuff.h>
#include <net/sock.h>
#include "doge_errinfo.h"
#include "doge_netheap.h"

#define NETLINK_USER 30

struct doge_nlhdr {
        __be32 ip;
        __be16 port;
        uint8_t opcode;
        char *data;
        uint8_t data_len;
};

void handle_msg(void);
void netlink_clean(void);

#endif
