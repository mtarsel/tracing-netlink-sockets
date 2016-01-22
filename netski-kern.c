/*
    Mick Tarsel 

Netlink sockets simple example. 

Look at RFC 3549

FROM "Understanding Linux Network Internals" by Christian Benvenuti

    netlink sockets are defined by 3 things:
        domain
        type 
        protocol arguments  

    With Netlink sockets, endpoints are usually identified by the ID of the
process that opened the sockets (PID), where the special value 0 identifies the
kernel. Among Netlink’s features is the ability to send both unicast and 
multicast messages: the destination endpoint address can be a PID, a multicast 
group ID, or a combination of the two.

    One of the advantages of Netlink over other user-kernel interfaces such as 
ioct is that the kernel can initiate a transmission instead of just returning 
information in answer to user-space requests.

Tested on Linux testski 3.19.0-15-generic #15-Ubuntu SMP Thu Apr 16 23:32:37 UTC 
2015 x86_64 x86_64 x86_64 GNU/Linux

my output went to /var/log/syslog and dmesg

*/
#define CREATE_TRACE_POINTS
#include "netski.h"

#include <linux/module.h>
#include <net/sock.h>
#include <linux/netlink.h>
#include <linux/skbuff.h>

#define DRIVER_AUTHOR "Mick Tarsel <mjtarsel@us.ibm.com>"
#define DRIVER_DESC "A basic module to learn more about netlink sockets."

/*you could use existing netlink socket protocol types, instead I add my own
 * netlink protocol type here
*/
#define NETLINK_USER 31


struct sock *nl_sk = NULL;

static void netski_recv_msg(struct sk_buff *skb)
{

    struct nlmsghdr *nlh; //netlink message header
    int pid;
    struct sk_buff *skb_out;
    int msg_size;
    char *msg = "Hello from kernel";
    int res;

    printk(KERN_INFO "Entering: %s\n", __func__);

    //we need to allocate a buffer large enough to hold netlink message
    //headers and payloads
    msg_size = strlen(msg);

    //netlink header is in skb data
    nlh = (struct nlmsghdr *)skb->data;
    
    printk(KERN_INFO "Netlink received msg payload: %s\n", 
        (char *)nlmsg_data(nlh));
    
    pid = nlh->nlmsg_pid; //pid of sending process 
	printk("%s: pid %d \n", __func__, pid);		
	
	//TODO showtime....
	trace_netski(pid, (char *)nlmsg_data(nlh));

    //skb_out is our buffer. if not big enough then lightly toss in a KERN_ERR
    skb_out = nlmsg_new(msg_size, 0);

    if (!skb_out){
        printk(KERN_ERR "Failed to allocate new skb\n");
        return;
    }
   
    //nlh now points to header of netlink message just recieved 
    nlh = nlmsg_put(skb_out, 0, 0, NLMSG_DONE, msg_size, 0);
    NETLINK_CB(skb_out).dst_group = 0; // not in multicast group 
    strncpy(nlmsg_data(nlh), msg, msg_size);

    res = nlmsg_unicast(nl_sk, skb_out, pid);

    if (res < 0)
        printk(KERN_INFO "Error while sending back to user\n");
}

static int __init netski_init(void)
{
    /*in newer kernel version, netlink_kernel_create has only 3 params so I
    made a struct to invoke callback function (netski_recv_msg)*/
    struct netlink_kernel_cfg cfg = {
        .groups = 0,
        .input = netski_recv_msg,
    };

    printk(KERN_INFO "Starting netski kernel module...\n");    
    
    nl_sk = netlink_kernel_create(&init_net, NETLINK_USER, &cfg);

    printk("Entering: %s\n", __func__);
    
    if (!nl_sk){
        printk(KERN_ALERT "Error creating socket.\n");
        return -69;
    }

    return 0;
}

static void __exit netski_exit(void)
{

    printk(KERN_INFO "exiting netski module. goodbye.\n");
    netlink_kernel_release(nl_sk);
}

//set kernel function macros
module_init(netski_init); 
module_exit(netski_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);  
MODULE_DESCRIPTION(DRIVER_DESC);
