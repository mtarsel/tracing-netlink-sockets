/*
	Mick Tarsel

DONT FORGET TO ENABLE YOUR TRACEPOINTS
=====================================
echo 1 > /sys/kernel/debug/tracing/events/netski/enable
echo 1 > /sys/kernel/debug/tracing/events/netski/netski/enable

./netski-user

TO SEE YOUR RESULTS:
=====================
cat /sys/kernel/debug/tracing/trace | grep netski
dmesg will show the PID you're looking for

This program creates a tracepoint called netski. It is a good idea to name the
header file and the tracepoint the same. You HAVE to name them the same if you
are not going to include the header in the default location include/trace/events

For more info about how this works, the third part of a LWN article will explain
this code http://lwn.net/Articles/383362/

For more of an intro about Tracepoints and to understand why the struct is
defined in such a way, then read the first article of the LWN Tutorial
http://lwn.net/Articles/379903/ */

#undef TRACE_SYSTEM
#define TRACE_SYSTEM netski

#if !defined(_TRACE_NETSKI_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_NETSKI_H

#include <linux/tracepoint.h>



/*
 * Tracepoint for netski kernel module:
 */
TRACE_EVENT(netski,

    TP_PROTO(int pid, const char* msg),

    TP_ARGS(pid, msg),

    TP_STRUCT__entry(
        __field(    pid_t,     pid     )
        __string(    msg,     msg     )
    ),

    TP_fast_assign(
        __assign_str(msg, msg);
        __entry->pid = pid;
    ),

    TP_printk("da pid=%d MSG:%s",
        __entry->pid, __get_str(msg))
);

#endif /*_TRACE_NETSKI_H */
#undef TRACE_INCLUDE_PATH
#define TRACE_INCLUDE_PATH .
#define TRACE_INCLUDE_FILE netski
#include <trace/define_trace.h>
