// CUSTOMIZE: my_syscall.c
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <net/dummy_fw.h>

SYSCALL_DEFINE1(my_syscall, int, arg)
{
    struct net *net = NULL;
    struct tcf_proto *tp = NULL;
    struct fw_filter *f = NULL;
    struct nlattr *tb[0];
    struct nlattr *tca[0];
    unsigned long base = 0;
    u32 flags = 0;
    struct netlink_ext_ack *extack = NULL;

    int ret;

    struct dummy_fw_set_parms_args *args = kmalloc(sizeof(struct dummy_fw_set_parms_args), GFP_KERNEL);
    if (!args) {
        pr_info("my_syscall: Error: kmalloc failed\n");
        return -ENOMEM;
    }

    args->net = net;
    args->tp = tp;
    args->f = f;
    args->tb = tb;
    args->tca = tca;
    args->base = base;
    args->flags = flags;
    args->extack = extack;

    pr_info("my_syscall: Calling dummy_fw_set_parms with arg=%d\n", arg);
    ret = dummy_fw_set_parms(args);

    return ret;
}