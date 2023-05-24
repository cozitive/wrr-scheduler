#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <sched.h>
#include <sys/time.h>
#include <sys/wait.h>
#include "wrappers.h"

#define X_DEFAULT 999999937
#define MAX_WEIGHT 20
#define TRIAL_COUNT 3

//double turnaround[MAX_WEIGHT];

void factorize(int x);

int main(int argc, char *argv[])
{
    int x = (argc > 1) ? atoi(argv[1]) : X_DEFAULT;

	if (getuid() != 0) {
		printf("WRR turnaround time test should be run in root user\n");
		return 0;
	}

	printf("WRR turnaround time test: prime factoriazation of %d\n\n", x);

	pid_t pid = getpid();
	for (int weight = 1; weight <= MAX_WEIGHT; weight++) {
		sched_setweight(pid, weight);

		struct timeval start, end;
		double elapsed;
		
		while (sched_getweight(getpid()) != weight);

		gettimeofday(&start, NULL);
		factorize(x);
		gettimeofday(&end, NULL);

		elapsed = (double)(end.tv_usec - start.tv_usec) / 1000000 + (double)(end.tv_sec - start.tv_sec);
		printf("[%d] weight %2d: %lf secs\n", weight, weight, elapsed);
		//turnaround[weight - 1] = elapsed;
	}

	return 0;
}

void factorize(int x)
{
	int div = 2;
	int is_first = 1;

	// printf("%d = ", x);
	while (x > 1) {
		if (x % div == 0) {
			if (is_first) {
				// printf("%d", div);
				is_first = 0;
			} else {
				// printf(" * %d", div);
			}
			x /= div;
		} else {
			div++;
		}
	}
}
