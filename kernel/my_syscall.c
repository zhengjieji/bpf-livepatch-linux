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

    pr_info("my_syscall: Calling dummy_fw_set_parms with arg=%d\n", arg);
    ret = dummy_fw_set_parms(net, tp, f, tb, tca, base, flags, extack);

    return ret;
}