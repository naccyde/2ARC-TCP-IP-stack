#ifndef _DOGE_KLINK_H
#define _DOGE_KLINK_H

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/tcp.h>
#include <linux/ip.h>
#include <linux/if_arp.h>
#include <net/tcp.h>
#include "doge_errinfo.h"
#include "doge_tcp.h"
#include "doge_icmp.h"
#include "doge_ip.h"
#include "doge_arp.h"
#include "doge_eth.h"

struct doge_packet;

struct sk_buff *doge_packet_to_sk_buff(struct doge_packet *packet);

#endif
