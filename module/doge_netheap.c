#include "doge_netheap.h"

static struct net_item *fst;
static struct net_item *act;
static struct net_item *lst;
static int n_items;

/* next_item : Get next item in act pointer */
static int next_item(void)
{
        struct net_item *tmp = act->next;

        if (tmp != act) {
                act = act->next;
                return 1;
        }

        return 0;
}

/* seek : Seek for item with provided PID */
static int seek(int pid)
{
        act = fst;

        do {
                if (act->nlh == NULL)
                        return 0;

                if (act->pid == pid)
                        return 1;
        } while (next_item());

        return 0;
}

/* init_heap : Init Netlink heap */
void init_heap(void)
{
        fst = kmalloc(sizeof(struct net_item), 0);
        fst->prvs = fst;
        fst->nlh = NULL;
        fst->pid = 0;
        fst->next = fst;

        act = fst;
        lst = fst;

        n_items = 0;
}

/* add_heap : Add item at the end of the heap */
void add_heap(struct nlmsghdr *nlh, int pid)
{
        struct net_item *tmp;

        lst->nlh = nlh;
        lst->pid = pid;
        
        /* Create last item in heap */
        tmp = (struct net_item *)kmalloc(sizeof(struct net_item), 0);
        tmp->prvs = lst;
        tmp->nlh = NULL;
        tmp->pid = 0;
        tmp->next = tmp;
        
        lst->next = tmp;

        lst = tmp;
        ++n_items;
}

/* rmv_heap : Remove an item from the heap */
void rmv_heap(int pid)
{
        if (seek(pid)) {
                struct net_item *to_rmv = act;

                if (fst == to_rmv) {
                        fst = fst->next;
                        fst->prvs = fst;
                } else if (lst == to_rmv) {
                        lst = lst->prvs;
                        lst->next = lst;
                } else {
                        struct net_item *bfr = to_rmv->prvs;
                        struct net_item *aftr = to_rmv->next;
                        bfr->next = aftr;
                        aftr->prvs = bfr;
                }
        
                kfree(to_rmv);
                --n_items;
        }
}

/* clean_heap : Clean heap data */
void heap_clean(void)
{
        while (n_items--) {
                act = lst;        

                lst = lst->prvs;
                kfree(act);       
        }

        kfree(fst);
}
