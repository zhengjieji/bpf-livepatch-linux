// CUSTOMIZE: my_syscall.c
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <net/ip.h>

SYSCALL_DEFINE1(my_syscall, int, arg)
{
    printk(KERN_INFO "My syscall called with arg: %d\n", arg);
    bool ret = false;
    ret = dummy_icmp_global_allow();
    printk(KERN_INFO "dummy_icmp_global_allow returned: %d\n", ret);
    return 0;
}