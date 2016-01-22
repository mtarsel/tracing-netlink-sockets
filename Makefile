obj-m += netski-kern.o

CFLAGS_netski-kern.o= -I$(src)

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules $(CFLAGS); \
        
	@if [ -f netski-kern.ko ];  then \
                sudo insmod netski-kern.ko; \
                printf "\n Kernel module loaded succesfully. \n \n"; \
                lsmod | grep netski_kern; \
                printf "\n"; \
                lsmod;  \
        fi; \

#       bash -c "sudo -i echo 1 > /sys/kernel/debug/tracing/events/netski/enable"
#       bash -c "sudo -i echo 1 > /sys/kernel/debug/tracing/events/netski/netski/enable"        
#       echo ""
#       $(info "Enabled trace points succesfully.");

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean; \
	rm ./netski-user
#       bash -c "sudo -i echo 1 > /sys/kernel/debug/tracing/events/netski/enable"
#       bash -c "sudo -i echo 0 > /sys/kernel/debug/tracing/events/netski/netski/enable"        

remove:
	sudo rmmod netski_kern; \
	lsmod; 
	$(info "Removed kernel module succesfully.")

