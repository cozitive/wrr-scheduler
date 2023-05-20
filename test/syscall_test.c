#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include "wrappers.h"

void getweight_negative_pid_test() {
    printf("    negative PID: ");
    int ret = sched_getweight(-1);
    if ((ret != -1) || (errno != EINVAL)) {
        printf("FAIL\n");
        return;
    }
    printf("OK\n");
}

void getweight_invalid_pid_test() {
    printf("    invalid PID: ");
    int ret = sched_getweight(1000000);
    if ((ret != -1) || (errno != ESRCH)) {
        printf("FAIL\n");
        return;
    }
    printf("OK\n");
}

void getweight_error_test() {
    printf("getweight error test:\n");
    getweight_negative_pid_test();
    getweight_invalid_pid_test();
}

void setweight_negative_pid_test() {
    printf("    negative PID: ");
    int ret = sched_setweight(-1, 1);
    if ((ret != -1) || (errno != EINVAL)) {
        printf("FAIL\n");
        return;
    }
    printf("OK\n");
}

void setweight_weight_overflow_test() {
    printf("    weight overflow: ");
    int ret = sched_setweight(getpid(), 0);
    if ((ret != -1) || (errno != EINVAL)) {
        printf("FAIL\n");
        return;
    }
    printf("OK\n");
}

void setweight_weight_underflow_test() {
    printf("    weight underflow: ");
    int ret = sched_setweight(getpid(), 21);
    if ((ret != -1) || (errno != EINVAL)) {
        printf("FAIL\n");
        return;
    }
    printf("OK\n");
}

void setweight_invalid_pid_test() {
    printf("    invalid PID: ");
    int ret = sched_setweight(1000000, 1);
    if ((ret != -1) || (errno != ESRCH)) {
        printf("FAIL\n");
        return;
    }
    printf("OK\n");
}

void setweight_invalid_permission_test() {
    printf("    invalid permission: ");
    if (getuid() == 0) {    // root
        printf("no test (root user)\n");
        return;
    }
    int ret = sched_setweight(1, 20);   // systemd
    if ((ret != -1) || (errno != EPERM)) {
        printf("FAIL\n");
        return;
    }
    printf("OK\n");
}

void setweight_error_test() {
    printf("setweight error test:\n");
    setweight_negative_pid_test();
    setweight_weight_overflow_test();
    setweight_weight_underflow_test();
    setweight_invalid_pid_test();
    setweight_invalid_permission_test();
}

void get_default_weight_test() {
    printf("get default weight test: ");
    int pid = getpid();
    int weight = sched_getweight(pid);
    if (weight != 10) {
        printf("FAIL\n");
        return;
    }
    printf("OK\n");
}

void set_and_get_test() {
    printf("set and get test: ");
    int pid = getpid();
    if (sched_setweight(pid, 12) != 0) {
        printf("FAIL\n");
        return;
    }
    int weight = sched_getweight(pid);
    if (weight != 12) {
        printf("FAIL\n");
        return;
    }
    printf("OK\n");
}

int main(int argc, char *argv[])
{
    getweight_error_test();
    setweight_error_test();
    get_default_weight_test();
    set_and_get_test();

    return 0;
}
