#include <bpf/bpf_helpers.h> 
#include <uapi/linux/bpf.h>
#include <bpf/bpf_tracing.h>
#include <net/ip.h>

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
    bpf_printk("fmod_ret: function called and return modified");
    return -1; // This will skip the original function logic
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