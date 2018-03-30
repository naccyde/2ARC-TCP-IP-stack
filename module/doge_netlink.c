#include "doge_netlink.h"

static struct sock *nl_sk;

static struct nlhdr *char_to_nlhdr(char *data)
{
        char *tmp = data;
        struct doge_nlhdr *nlhdr = (struct nlhdr *)kmalloc(sizeof(struct doge_nlhdr), GFP_KERNEL);
        char ip[20] = {0};
        char port[10] = {0};
        char opcode[5] = {0};
       
        nlhdr->data = NULL;
        nlhdr->data_len = 0;
       
        /* IP */
        for ( ; *tmp != '.' ; ++tmp) ;
        memcpy(&ip[0], data, tmp-data);
        data = ++tmp;
        
        /* Port */
        for ( ; *tmp != '.' ; ++tmp) ;
        memcpy(&port[0], data, tmp-data);
        data = ++tmp;

        /* OPCODE */
        for ( ; *tmp != '.' ; ++tmp) ;
        memcpy(&opcode[0], data, tmp-data);
        data = ++tmp;
        
        if (tmp == 0) 
                return nlhdr;
        
        nlhdr->data_len = strlen(data);
        nlhdr->data = (char *)kcalloc(nlhdr->data_len, 1, GFP_KERNEL);
        memcpy(nlhdr->data, data, nlhdr->data_len);
}

/* send_msg : Send a message to PID */
static void send_msg(struct nlmsghdr *nlh, int pid, char *msg, int size)
{
        struct sk_buff *skb;
        int res;

        skb = nlmsg_new(size, 0);

        if (! skb) {
                doge_print(KERN_ERR, "Could not allocate new skb.", NULL, __FUNCTION__);
                return;
        }

        nlh = nlmsg_put(skb, 0, 0, NLMSG_DONE, size, 0);
        NETLINK_CB(skb).dst_group = 0; /* Not in mcast group */
        strncpy(nlmsg_data(nlh), msg, size);

        res = nlmsg_unicast(nl_sk, skb, pid);

        if (res < 0)
                doge_print(KERN_ERR, "Can't send Netlink packet.", NULL, __FUNCTION__);

}

/* handle_recv_msg : Manage received Netlink messages */
static void handle_recv_msg(struct sk_buff *skb)
{
        struct nlmsghdr *nlh;
        struct doge_nlhdr *nlhdr = NULL;
        char *msg;

        nlh = (struct nlmsghdr *)skb->data;
        msg = (char *)nlmsg_data(nlh);

        nlhdr = char_to_nlhdr(msg);

        
}

/* handle_msg : Set up Netlink message handling method */
void handle_msg(void)
{
        struct netlink_kernel_cfg cfg = { .input = handle_recv_msg };
        
        nl_sk = netlink_kernel_create(&init_net, NETLINK_USER, &cfg);
        if (! nl_sk)
                doge_print(KERN_ERR, "Could not create Netlink socket.", NULL,  __FUNCTION__);
}

/* netlink_clean : Clean netlink socket messes */
void netlink_clean(void)
{
        netlink_kernel_release(nl_sk);       
}
