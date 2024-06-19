#include <bpf/bpf_helpers.h>
#include <linux/version.h>
#include <uapi/linux/bpf.h>
#include <bpf/bpf_tracing.h>

/*
 * Here is the idea
 * when the function fib6_run_gc is triggered
 * fentry will be executed first and modify
 * function then fmod_ret will return 1 this
 * will skip the original function logic and
 * then fexit will do nothing for now
 */
SEC("fentry/ip6_dst_gc")
int fentry_func(struct dst_ops *ops) {
    bpf_printk("BPF live patch entry: ip6_dst_gc\n");
	return 0;
}

SEC("fmod_ret/ip6_dst_gc")
int fmod_ret_func(struct dst_ops *ops) {
    return 1; // Skip the original function
}

SEC("fexit/ip6_dst_gc")
int fexit_func(struct dst_ops *ops) {
	// Can be empty for now
	bpf_printk("BPF live patch exit: ip6_dst_gc\n");
	return 0;
}

char _license[] SEC("license") = "GPL";
u32 _version SEC("version") = LINUX_VERSION_CODE;
