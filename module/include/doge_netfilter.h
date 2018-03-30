#ifndef _DOGE_NETFILTER_H
#define _DOGE_NETFILTER_H

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/netfilter_bridge.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <net/arp.h>
#include <asm-generic/types.h>
#include "doge_errinfo.h"
#include "doge_klink.h"
#include "doge_dlink.h"

void hook_init(void);
void hook_clean(void);

#endif
