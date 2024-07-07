// for bpf

#define KBUILD_MODNAME "foo"

#include <bpf/bpf_helpers.h> 
#include <uapi/linux/bpf.h>
#include <bpf/bpf_tracing.h>
#include <net/dummy_fw.h>

// for livepatching
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/errno.h>
#include <linux/skbuff.h>
#include <net/netlink.h>
#include <net/act_api.h>
#include <net/pkt_cls.h>
#include <net/sch_generic.h>

#define HTSIZE 256

struct fw_head {
	u32			mask;
	struct fw_filter __rcu	*ht[HTSIZE];
	struct rcu_head		rcu;
};

// Entering the program
SEC("fentry/dummy_fw_set_parms")
// int dummy_fw_set_parms_fentry(void *ctx)
int BPF_PROG(dummy_fw_set_parms_fentry, struct net *net, struct tcf_proto *tp,
             struct fw_filter *f, unsigned long base, u32 flags,
             struct netlink_ext_ack *extack)
{
    bpf_printk("fw_set_parms: net=%p, tp=%p, f=%p, base=%lu, flags=%u, extack=%p\n", net, tp, f, base, flags, extack);
    return 0;
}

// Modify the return value of the program
SEC("fmod_ret/dummy_fw_set_parms")
// int dummy_fw_set_parms_fmod_ret(struct dummy_fw_set_parms_args *args, int ret)
// int dummy_fw_set_parms_fmod_ret(void *ctx)
int BPF_PROG(dummy_fw_set_parms_fmod_ret, struct net *net, struct tcf_proto *tp,
             struct fw_filter *f, unsigned long base, u32 flags,
             struct netlink_ext_ack *extack, int ret)
{
    // print args->base
    // bpf_printk("fmod_ret: flags: %u\n", flags);
    // pr_info("dummy_fw_set_parms_fmod_ret: original ret=%d\n", ret); // not supported
    bpf_printk("fmod_ret: function called and return modified, ret=%d\n", ret);
    return -1; // This will skip the original function logic
}

// Exit the program
SEC("fexit/dummy_fw_set_parms")
int dummy_fw_set_parms_fexit(void *ctx)
{
    bpf_printk("fexit: function called");
    return 0;
}

// Define license for the BPF program
char _license[] SEC("license") = "GPL";