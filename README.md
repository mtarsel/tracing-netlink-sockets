# tracing-netlink-sockets
Simple Kernel module to read netlink socket message sent to and from user space using TRACE_EVENT macro.

Look at netski.h for more info about trace_event macros or read about them here:
http://lwn.net/Articles/379903/


Make sure to enable trace points in your kernel 
https://www.kernel.org/doc/Documentation/trace/tracepoint-analysis.txt

Installation Instructions
====

$ make; make all

This will load the kernel module. 

Then execute the user space program.

$ ./netski-user

Check out dmesg and /var/log/syslog

When you are finished, remove the module:

$ make remove

This will also remove executables.

$ make clean 

With remove everything in the dir except for source files.

