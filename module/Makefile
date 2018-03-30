ifneq ($(KERNLRELEASE),)
include Kbuild

else
obj-m += dogemod.o
dogemod-objs += doge_tcp_connections.o doge_icmp.o doge_network.o doge_dlink.o doge_netlink.o doge_arp.o doge_ip.o doge_arptable.o doge_eth.o doge_tcp.o doge_klink.o doge_netfilter.o doge_errinfo.o doge_netheap.o doge.o
EXTRA_CFLAGS=-I$(PWD)/include

KDIR ?= /lib/modules/$(shell uname -r)/build

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

i:
	sudo insmod dogemod.ko

r:
	sudo rmmod dogemod

clean:
	make -C $(KDIR) M=$(PWD) clean

endif
