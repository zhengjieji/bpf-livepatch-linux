// for bpf program
#include <bpf/bpf_helpers.h> 
#include <uapi/linux/bpf.h>
#include <bpf/bpf_tracing.h>

/* klp-ccp: from net/ipv4/icmp.c */
#include <linux/module.h>
#include <linux/types.h>
#include <linux/jiffies.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <net/snmp.h>

/* klp-ccp: from include/net/ip.h */
static int (*klpe_sysctl_icmp_msgs_per_sec);
static int (*klpe_sysctl_icmp_msgs_burst);

/* klp-ccp: from net/ipv4/icmp.c */
#include <net/route.h>
#include <linux/skbuff.h>
#include <net/sock.h>
#include <linux/errno.h>
#include <linux/timer.h>
#include <linux/init.h>
#include <linux/uaccess.h>
#include <net/checksum.h>
#include <net/ip_fib.h>
#include <net/l3mdev.h>

// Define the global state

struct global_state {
    __u32 credit;
    __u32 stamp;
    struct bpf_spin_lock lock;
};

// Define the global state map
struct {
	__uint(type, BPF_MAP_TYPE_HASH);
	__uint(max_entries, 1);
	__type(key, int);
	__type(value, struct global_state);
} global_state_map SEC(".maps");

// Entry program
SEC("fentry/dummy_icmp_global_allow")
int BPF_PROG(dummy_icmp_global_allow_fentry)
{
    bpf_printk("fentry: function called");
    return 0;
}

// Modify the return value of the program
SEC("fmod_ret/dummy_icmp_global_allow")
int BPF_PROG(dummy_icmp_global_allow_fmod_ret, bool ret)
{
	u32 credit, delta, incr = 0; // now = (u32)jiffies;
    u32 now = (u32)bpf_jiffies64(); // use bpf helper function instead of accessting extern var
	bool rc = false;

    struct global_state zero = {}, *val; // initialize zeros

    int key = 0;
    int err = 0;

    val = bpf_map_lookup_elem(&global_state_map, &key);
    if (!val) {
        bpf_map_update_elem(&global_state_map, &key, &zero, 0);
        val = bpf_map_lookup_elem(&global_state_map, &key);
        if (!val) {
            bpf_printk("Failed to update global state");
            return err;
        }
    }

    // check if token bucket is empty and cannot be refilled without taking the spinlock.
    if (!val->credit) {
        delta = min_t(u32, now - val->stamp, HZ);
        if (delta < HZ / 50)
            return false;
    }

    // calculate the random value outside the spin lock
    u32 bpf_random_value = bpf_get_prandom_u32() % 3;

    // spin lock
    bpf_spin_lock(&val->lock);

    delta = min_t(u32, now - val->stamp, HZ);
    if (delta > HZ / 50) {
        // CHECK: is this correct? maybe also use a bpf map
        incr = *klpe_sysctl_icmp_msgs_per_sec * delta / HZ;
        if (incr)
            val->stamp = now;
    }
    credit = min_t(u32, val->credit + incr, *klpe_sysctl_icmp_msgs_burst); 
    if (credit) {
        credit = max_t(int, credit - bpf_random_value, 0); // credit = max(credit - prandom_u32_max(3), 0);
        // credit --; Fix CVE-2020-25705
        rc = true;
    }
    val->credit = credit;
    // bpf_map_update_elem(&global_state_map, &key, val, 0);

    // spin unlock
    bpf_spin_unlock(&val->lock);
 
    // bpf_printk("return 1 for true and 2 for false");
    return rc ? 1 : 2; // This will skip the original function logic
}

// Exit program
SEC("fexit/dummy_icmp_global_allow")
int BPF_PROG(dummy_icmp_global_allow_fexit, bool ret)
{
    bpf_printk("fexit: function returned");
    return 0;
}

// Define license for the BPF program
char _license[] SEC("license") = "GPL";