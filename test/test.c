#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <sched.h>
#include <sys/time.h>
#include <sys/wait.h>
#include "wrappers.h"

#define X_DEFAULT 300000007
#define MAX_WEIGHT 20
#define TRIAL_COUNT 5

void factorize(int x);

/// @brief Do prime factorization with different WRR weight, and measure each turnaround time.
int main(int argc, char *argv[])
{
    // The program can only be executed by root user.
    if (getuid() != 0) {
        printf("WRR turnaround time test should be executed by root user\n");
        return 0;
    }

    // If target # is not given, X_DEFAULT is target.
    int x = (argc > 1) ? atoi(argv[1]) : X_DEFAULT;

    printf("WRR turnaround time test: prime factoriazation of %d\n", x);

    pid_t pid = getpid();

    // Set task weight from 1 to 20 by each trial.
    for (int weight = 1; weight <= MAX_WEIGHT; weight++) {
        if (sched_setweight(pid, weight) != 0) {
            perror("sched_setweight()");
            return 0;
        }
        while (sched_getweight(pid) != weight);

        printf("===========================================\n");
        printf("[weight %d]\n", weight);

        // Do prime factorization 'TRIAL_COUNT' times and print average time of trial.
        struct timeval start, end;
        double total = 0;
        for (int i = 0; i < TRIAL_COUNT; i++) {
            gettimeofday(&start, NULL);
            factorize(x);
            gettimeofday(&end, NULL);
            double elapsed = (double)(end.tv_usec - start.tv_usec) / 1000000 + (double)(end.tv_sec - start.tv_sec);
            total += elapsed;
            printf("[%d] %lf secs\n", i + 1, elapsed);
        }
        printf("[avg] %lf secs\n", total / TRIAL_COUNT);
    }

    return 0;
}

/// @brief Calculate prime factorization of `x`.
void factorize(int x)
{
    int div = 2;
    int is_first = 1;

    printf("%d =", x);
    while (x > 1) {
        if (x % div == 0) {
            if (!is_first) {
                printf(" *");
            }
            x /= div;
            is_first = 0;
            printf(" %d", div);
        } else {
            div++;
        }
    }
    printf("\t");
}
