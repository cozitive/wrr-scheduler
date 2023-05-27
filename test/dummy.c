#define _GNU_SOURCE
#include <stdio.h>
#include "wrappers.h"
#include <unistd.h>
#include <stdlib.h>
#include <sched.h>

#define MAX_WEIGHT 20

// Dummy does nothing, only spins
int main(int argc, char *argv[])
{
    if (getuid() != 0) {
        printf("should be run in root user\n");
        return 0;
    }

    // Set dummy's weight to given weight or default MAX_WEIGHT
    int weight = (argc > 1) ? atoi(argv[1]) : MAX_WEIGHT;
    int pid = getpid();
    if (sched_setweight(pid, weight) != 0) {
        perror("sched_setweight()");
        return 0;
    }

    // If there's given CPU #, set only affinity to given CPU
    if (argc == 3) {
        int cpu = atoi(argv[2]);

        cpu_set_t mask;
        CPU_ZERO(&mask);
        CPU_SET(cpu, &mask);

        if (sched_setaffinity(getpid(), sizeof(mask), &mask) != 0) {
            perror("sched_setaffinity()");
            return EXIT_FAILURE;
        }
    }
    while(1);
}