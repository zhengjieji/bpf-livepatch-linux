// CUSTOMIZE: test_my_syscall.c
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

#define SYS_my_syscall 548

int main() {
    int arg = 123;
    long res = syscall(SYS_my_syscall, arg);
    printf("Syscall returned: %ld\n", res);
    return 0;
}