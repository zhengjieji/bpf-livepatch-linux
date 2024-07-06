#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <bpf/bpf.h>
#include <bpf/libbpf.h>

int main(int argc, char **argv) {
    // Define bpf_link variables to store the attached BPF program links
    struct bpf_link *link_fentry = NULL;
    struct bpf_link *link_fmod_ret = NULL;
    struct bpf_link *link_fexit = NULL;

    // Define bpf_program variables
    struct bpf_program *prog_fentry;
    struct bpf_program *prog_fmod_ret;
    struct bpf_program *prog_fexit;

    // Define bpf_object variable
    struct bpf_object *obj;
    
    // BPF object file name
    const char filename[] = "bpflp_fw_set_parms_kern.o";

    // Open the BPF object file
    obj = bpf_object__open_file(filename, NULL);
    if (libbpf_get_error(obj)) {
        fprintf(stderr, "Error: opening BPF object file %s\n", filename);
        return 1;
    }

    // Find the BPF program by name in the object file
    prog_fentry = bpf_object__find_program_by_name(obj, "dummy_fw_set_parms_fentry");
    if (!prog_fentry) {
        fprintf(stderr, "Error: finding program 'dummy_fw_set_parms_fentry' in the object file failed\n");
        goto cleanup;
    }

    prog_fmod_ret = bpf_object__find_program_by_name(obj, "dummy_fw_set_parms_fmod_ret");
    if (!prog_fmod_ret) {
        fprintf(stderr, "Error: finding program 'dummy_fw_set_parms_fmod_ret' in the object file failed\n");
        goto cleanup;
    }

    prog_fexit = bpf_object__find_program_by_name(obj, "dummy_fw_set_parms_fexit");
    if (!prog_fexit) {
        fprintf(stderr, "Error: finding program 'dummy_fw_set_parms_fexit' in the object file failed\n");
        goto cleanup; 
    }

    // Load the BPF object file
    printf("Before Loading\n");
    if (bpf_object__load(obj)) {
        fprintf(stderr, "Error: loading BPF object file %s failed\n", filename);
        goto cleanup;
    }

    // Attach the BPF programs
    printf("Before Attaching\n");
    link_fentry = bpf_program__attach(prog_fentry);
    if (libbpf_get_error(link_fentry)) {
        fprintf(stderr, "Error: attaching program 'dummy_fw_set_parms_fentry' failed: %ld\n", libbpf_get_error(link_fentry));
        link_fentry = NULL;
        goto cleanup;
    } else {
        printf("Program 'dummy_fw_set_parms_fentry' attached successfully\n");
    }

    link_fmod_ret = bpf_program__attach(prog_fmod_ret);
    if (libbpf_get_error(link_fmod_ret)) {
        fprintf(stderr, "Error: attaching program 'dummy_fw_set_parms_fmod_ret' failed: %ld\n", libbpf_get_error(link_fmod_ret));
        link_fmod_ret = NULL;
        goto cleanup;
    } else {
        printf("Program 'dummy_fw_set_parms_fmod_ret' attached successfully\n");
    }

    link_fexit = bpf_program__attach(prog_fexit);
    if (libbpf_get_error(link_fexit)) {
        fprintf(stderr, "Error: attaching program 'dummy_fw_set_parms_fexit' failed: %ld\n", libbpf_get_error(link_fexit));
        link_fexit = NULL;
        goto cleanup;
    } else {
        printf("Program 'dummy_fw_set_parms_fexit' attached successfully\n");
    }

    // Keep the program running
    while (1) {
        // Do nothing, keep the program running
    }

cleanup:
    // Clean up resources
    bpf_link__destroy(link_fentry);
    bpf_link__destroy(link_fmod_ret);
    bpf_link__destroy(link_fexit);
    bpf_object__close(obj);
    return 0;
}