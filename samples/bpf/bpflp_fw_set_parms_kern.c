#include <bpf/bpf_helpers.h> 
#include <uapi/linux/bpf.h>
#include <bpf/bpf_tracing.h>
// #include <net/dummy_fw.h>

// Entry program for igmp_start_timer function
SEC("fentry/dummy_fw_set_parms")
int dummy_fw_set_parms_fentry(void *ctx)
{
    bpf_printk("fentry: dummy_fw_set_parms function called");
    return 0;
}

// Modify return program for igmp_start_timer function
SEC("fmod_ret/dummy_fw_set_parms")
int dummy_fw_set_parms_fmod_ret(void *ctx)
{
    bpf_printk("fmod_ret: dummy_fw_set_parms function called and return modified");
    return -1; // This will skip the original function logic
}

// Exit program for igmp_start_timer function
SEC("fexit/dummy_fw_set_parms")
int dummy_fw_set_parms_fexit(void *ctx)
{
    bpf_printk("fexit: dummy_fw_set_parms function called");
    return 0;
}

// Define license for the BPF program
char _license[] SEC("license") = "GPL";