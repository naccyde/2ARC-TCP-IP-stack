#ifndef _DOGE_ERRINFO_H
#define _DOGE_ERRINFO_H

#include <linux/kernel.h>
#include <linux/module.h>
#include <net/arp.h>
#include "doge_arp.h"
#include "doge_eth.h"

struct doge_arphdr;

void doge_print(char *lvl, const char *msg, const char *ctt, const char *func);
void arphdr_infos(struct arphdr *arp);
void doge_arphdr_infos(struct doge_arphdr *arp);
void doge_arptable_infos(struct hlist_head *arptable);

#endif
