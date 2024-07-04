#include <bpf/bpf_helpers.h> 
#include <uapi/linux/bpf.h>
#include <bpf/bpf_tracing.h>
#include <linux/igmp.h>
#include <linux/timer.h>

// Entry program for igmp_start_timer function
SEC("fentry/igmp_start_timer")
int igmp_start_timer_fentry(void *ctx)
{
    bpf_printk("fentry: igmp_start_timer function called");
    return 0;
}

// Modify return program for igmp_start_timer function
SEC("fmod_ret/igmp_start_timer")
int igmp_start_timer_fmod_ret(void *ctx)
{
    bpf_printk("fmod_ret: igmp_start_timer function called and return modified");
    // print context and see what is inside
    return -1; // This will skip the original function logic
}

// Exit program for igmp_start_timer function
SEC("fexit/igmp_start_timer")
int igmp_start_timer_fexit(void *ctx)
{
    bpf_printk("fexit: igmp_start_timer function called");
    return 0;
}

// Define license for the BPF program
char _license[] SEC("license") = "GPL";