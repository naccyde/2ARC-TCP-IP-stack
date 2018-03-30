#include <linux/kernel.h>
#include <linux/module.h>
#include "doge_arptable.h"
#include "doge_netfilter.h"
#include "doge_netheap.h"
#include "doge_netlink.h"
#include "doge_tcp_connections.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Victor Novince & Quentin Deslandes");

static int __init doge_init(void)
{
        doge_print(KERN_INFO, "Module working.", NULL, __FUNCTION__);
        
        doge_arptable_init();
        doge_tcpc_init();
        hook_init();
        init_heap();
        handle_msg();
        
        return 0;
}

static void __exit doge_exit(void)
{
        netlink_clean();
        heap_clean();
        hook_clean();
        doge_tcpc_clean();
        doge_arptable_clean();

        doge_print(KERN_INFO, "Exiting module.", NULL, __FUNCTION__);
}

module_init(doge_init);
module_exit(doge_exit);
