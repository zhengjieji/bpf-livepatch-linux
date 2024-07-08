// CUSTOMIZE: my_syscall.c
#include <linux/kernel.h>
#include <linux/syscalls.h>

SYSCALL_DEFINE1(my_syscall, int, arg)
{
    printk(KERN_INFO "My syscall called with arg: %d\n", arg);
    return 0;
}