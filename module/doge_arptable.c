#include "doge_arptable.h"

static struct arptable_entry **arptable;

/* arptable_hash : Hash function */
static int arptable_hash(__be32 ip)
{
        return ip % 100;
}

/* arptable_last : Get the last entry of an arptable entry list */
static struct arptable_entry *arptable_last(int hash)
{
        struct arptable_entry *tmp = arptable[hash];

        while (tmp->next != NULL)
                tmp = tmp->next;

        return tmp;
}

/* free_list : Free the whole list following entry */
static void free_list(struct arptable_entry *entry)
{
        struct arptable_entry *tmp;

        while (tmp != NULL) {
                tmp = entry->next;
                kfree(entry);
                entry = tmp;
        }
}

/* doge_arptable_init : Init function, allocate the arptable */
void doge_arptable_init(void)
{
        arptable = (struct arptable_entry **)kcalloc(ARPTABLE_SIZE, sizeof(struct arptable_entry *), GFP_KERNEL);
}

/* doge_arptable_add : Add an entry to the ARP table */
void doge_arptable_add(__be32 new_ip, const unsigned char *new_mac)
{
        int hash;
        struct arptable_entry *tmp;
        struct arptable_entry *entry = (struct arptable_entry *)kmalloc(sizeof(struct arptable_entry), GFP_KERNEL);

        entry->ip = new_ip;
        memcpy(&entry->mac[0], new_mac, ETH_LEN);
        entry->next = NULL;

        hash = arptable_hash(new_ip);

        if (arptable[hash] == NULL) {
                arptable[hash] = entry;
        } else {
                tmp = arptable_last(hash);
                tmp->next = entry;
        }
}

/* doge_arptable_get : Get MAC addresse associate to ip entry */
unsigned char *doge_arptable_get(__be32 ip)
{
        struct arptable_entry *entry = arptable[arptable_hash(ip)];

        if (entry == NULL)
                return NULL;

        while (entry != NULL && entry->ip != ip)
                entry = entry->next;

        return entry->mac;
}

/* doge_arptable_clean : Clean the whole ARP table and allocate struct arptable_entry */
void doge_arptable_clean(void)
{
        int i;

        for (i = 0; i < ARPTABLE_SIZE; ++i)
                free_list(arptable[i]);

        kfree(arptable);
}
