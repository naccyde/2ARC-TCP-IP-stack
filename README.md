# TCP / IP stack

As part of a year-end project, we created a TCP/IP stack as a module for the
Linux kernel. Our stack then communicates with our library in user space via a
Netlink socket. As far as packet management is concerned, our LKM receives all
incoming packets through Netfilter hooks in order to manage them. All packages
not managed by our stack are sent to the Kernel stack. The implemented protocols
are ARP, RARP, ICMP, IP, Ethernet and TCP (in part).
