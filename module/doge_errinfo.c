#include "doge_errinfo.h"

void doge_print(char *lvl, const char *msg, const char *ctt, const char *func)
{
        printk("<%s>[DOGE] %s : %s : %s \n", lvl, func, msg, ctt != NULL ? ctt : "");
}

void arphdr_infos(struct arphdr *arp)
{
        unsigned char *ptr = (unsigned char *)(arp + 1);

        doge_print(KERN_DEBUG, "Infos about an ARP header", NULL, __FUNCTION__);
        printk(KERN_INFO "\tstruct arphdr * at %p :\n", arp);
        printk(KERN_INFO "\t\t.ar_hrd = %d\n", arp->ar_hrd); /* Link layer protocol */
        printk(KERN_INFO "\t\t.ar_pro = %d\n", arp->ar_pro); /* Network layer protocol */
        printk(KERN_INFO "\t\t.ar_hln = %d\n", arp->ar_hln); /* Link layer addr len */
        printk(KERN_INFO "\t\t.ar_pln = %d\n", arp->ar_pln); /* Network layer addr len */
        printk(KERN_INFO "\t\t.ar_op  = %d\n", arp->ar_op); /* ARP opcode */

        printk(KERN_INFO "\t\t.ar_sha = %X:%X:%X:%X:%X:%X\n", *ptr, *(ptr+1), *(ptr+2), *(ptr+3), *(ptr+4), *(ptr+5));
        ptr += ETH_LEN;
        printk(KERN_INFO "\t\t.ar_sip = %pI4\n", ptr);
        ptr += 4;
        printk(KERN_INFO "\t\t.ar_tha = %X:%X:%X:%X:%X:%X\n", *ptr, *(ptr+1), *(ptr+2), *(ptr+3), *(ptr+4), *(ptr+5));
        ptr += ETH_LEN;
        printk(KERN_INFO "\t\t.ar_tip = %pI4\n", ptr);
}

void doge_arphdr_infos(struct doge_arphdr *arp)
{
        doge_print(KERN_DEBUG, "Infos about an ARP header", NULL, __FUNCTION__);
        printk(KERN_INFO "\tstruct doge_arphdr * at %p :\n", arp);
        printk(KERN_INFO "\t\t.hw_addr_format = %d\n", arp->hw_addr_format); /* Link layer protocol */
        printk(KERN_INFO "\t\t.pr_addr_format = %d\n", arp->pr_addr_format); /* Network layer protocol */
        printk(KERN_INFO "\t\t.hw_addr_len = %d\n", arp->hw_addr_len); /* Link layer addr len */
        printk(KERN_INFO "\t\t.pr_addr_len = %d\n", arp->pr_addr_len); /* Network layer addr len */
        printk(KERN_INFO "\t\t.opcode = %d\n", arp->opcode); /* ARP opcode */

        printk(KERN_INFO "\t\t.src_hw_addr = %X:%X:%X:%X:%X:%X\n", arp->src_hw_addr[0], arp->src_hw_addr[1], arp->src_hw_addr[2], arp->src_hw_addr[3], arp->src_hw_addr[4], arp->src_hw_addr[5]);
        printk(KERN_INFO "\t\t.src_proto_addr = %d\n", arp->src_proto_addr);

        printk(KERN_INFO "\t\t.det_hw_addr = %X:%X:%X:%X:%X:%X\n", arp->dest_hw_addr[0], arp->dest_hw_addr[1], arp->dest_hw_addr[2], arp->dest_hw_addr[3], arp->dest_hw_addr[4], arp->dest_hw_addr[5]);
        printk(KERN_INFO "\t\t.det_proto_addr = %d\n", arp->dest_proto_addr);
}

void doge_arptable_infos(struct hlist_head *arptable)
{
         
}
