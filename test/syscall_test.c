#include <errno.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "wrappers.h"

#define SCHED_WRR 7

void getweight_negative_pid_test() {
    printf("    negative PID: ");

    int ret = sched_getweight(-1);
    if ((ret != -1) || (errno != EINVAL)) {
        printf("FAIL (errno: %d)\n", errno);
        return;
    }

    printf("OK\n");
}

void getweight_invalid_pid_test() {
    printf("    invalid PID: ");

    int ret = sched_getweight(1000000);
    if ((ret != -1) || (errno != ESRCH)) {
        printf("FAIL (errno: %d)\n", errno);
        return;
    }

    printf("OK\n");
}

void getweight_invalid_policy_test() {
    printf("    invalid policy: ");

    int pid = getpid();
    struct sched_param param = { .sched_priority = 99 };
    if (sched_setscheduler(pid, SCHED_RR, &param) != 0) {
        printf("FAIL (setscheduler, errno: %d)\n", errno);
        return;
    }

    int ret = sched_getweight(pid);
    if ((ret != -1) || (errno != EINVAL)) {
        printf("FAIL (errno: %d)\n", errno);
        return;
    }

    param.sched_priority = 0;
    if (sched_setscheduler(pid, SCHED_WRR, &param) != 0) {
        printf("FAIL (setscheduler, errno: %d)\n", errno);
        exit(1);
    }

    printf("OK\n");
}

void getweight_error_test() {
    printf("getweight error test:\n");
    getweight_negative_pid_test();
    getweight_invalid_pid_test();
    getweight_invalid_policy_test();
}

void setweight_negative_pid_test() {
    printf("    negative PID: ");

    int ret = sched_setweight(-1, 1);
    if ((ret != -1) || (errno != EINVAL)) {
        printf("FAIL (errno: %d)\n", errno);
        return;
    }

    printf("OK\n");
}

void setweight_weight_overflow_test() {
    printf("    weight overflow: ");

    int ret = sched_setweight(getpid(), 0);
    if ((ret != -1) || (errno != EINVAL)) {
        printf("FAIL (errno: %d)\n", errno);
        return;
    }

    printf("OK\n");
}

void setweight_weight_underflow_test() {
    printf("    weight underflow: ");

    int ret = sched_setweight(getpid(), 21);
    if ((ret != -1) || (errno != EINVAL)) {
        printf("FAIL (errno: %d)\n", errno);
        return;
    }

    printf("OK\n");
}

void setweight_invalid_pid_test() {
    printf("    invalid PID: ");

    int ret = sched_setweight(1000000, 1);
    if ((ret != -1) || (errno != ESRCH)) {
        printf("FAIL (errno: %d)\n", errno);
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
        printf("FAIL (errno: %d)\n", errno);
        return;
    }

    printf("OK\n");
}

void setweight_invalid_policy_test() {
    printf("    invalid policy: ");

    int pid = getpid();
    struct sched_param param = { .sched_priority = 99 };
    if (sched_setscheduler(pid, SCHED_RR, &param) != 0) {
        printf("FAIL (setscheduler, errno: %d)\n", errno);
        return;
    }

    int ret = sched_setweight(pid, 1);
    if ((ret != -1) || (errno != EINVAL)) {
        printf("FAIL (errno: %d)\n", errno);
        return;
    }

    param.sched_priority = 0;
    if (sched_setscheduler(pid, SCHED_WRR, &param) != 0) {
        printf("FAIL (setscheduler, errno: %d)\n", errno);
        exit(1);
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
    setweight_invalid_policy_test();
}

void get_default_weight_test() {
    printf("get default weight test: ");

    int weight = sched_getweight(getpid());
    if (weight != 10) {
        printf("FAIL (weight: %d)\n", weight);
        return;
    }

    printf("OK\n");
}

void set_and_get_test() {
    printf("set and get test: ");

    int pid = getpid();
    if (sched_setweight(pid, 12) != 0) {
        printf("FAIL (errno: %d)\n", errno);
        return;
    }

    int weight = sched_getweight(pid);
    if (weight != 12) {
        printf("FAIL (weight: %d)\n", weight);
        return;
    }

    printf("OK\n");
}

void setscheduler_default_weight_test() {
    printf("setscheduler default weight test: ");

    int pid = getpid();
    if (sched_setweight(pid, 12) != 0) {
        printf("FAIL: (setweight, errno: %d)\n", errno);
        return;
    }

    struct sched_param param = { .sched_priority = 99 };
    if (sched_setscheduler(pid, SCHED_RR, &param) != 0) {
        printf("FAIL (setscheduler, errno: %d)\n", errno);
        return;
    }

    param.sched_priority = 0;
    if (sched_setscheduler(pid, SCHED_WRR, &param) != 0) {
        printf("FAIL (setscheduler, errno: %d)\n", errno);
        exit(1);
    }

    int weight = sched_getweight(pid);
    if (weight != 10) {
        printf("FAIL (weight: %d)\n", weight);
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
    setscheduler_default_weight_test();

    return 0;
}
