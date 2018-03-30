#ifndef _DOGE_DLINK_H
#define _DOGE_DLINK_H

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include "doge_tcp.h"
#include "doge_icmp.h"
#include "doge_ip.h"
#include "doge_arp.h"
#include "doge_eth.h"

struct doge_packet *sk_buff_to_doge_packet(struct sk_buff *skb, struct net_device *dev);

#endif
