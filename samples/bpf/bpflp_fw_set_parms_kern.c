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
int dummy_fw_set_parms_fentry(void *ctx)
{
    bpf_printk("fentry: function called");
    return 0;
}

// Modify the return value of the program
SEC("fmod_ret/dummy_fw_set_parms")
int dummy_fw_set_parms_fmod_ret(struct dummy_fw_set_parms_args *args, int ret)
{
    // print args->base
    bpf_printk("fmod_ret: flags: %u\n", args->flags);
    bpf_printk("fmod_ret: function called and return modified");
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