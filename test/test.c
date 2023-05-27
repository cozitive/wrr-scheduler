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

int main(int argc, char *argv[])
{
	// If target # is not given, X_DEFAULT is target
    int x = (argc > 1) ? atoi(argv[1]) : X_DEFAULT;

	if (getuid() != 0) {
		printf("WRR turnaround time test should be run in root user\n");
		return 0;
	}
	
	printf("WRR turnaround time test: prime factoriazation of %d\n\n", x);

	pid_t pid = getpid();

	// Set task weight from 1 to 20 by each trial
	for (int weight = 1; weight <= MAX_WEIGHT; weight++) {
		if (sched_setweight(pid, weight) != 0) {
			perror("sched_setweight()");
			return 0;
		}

		struct timeval start, end;
		double elapsed = 0;
		
		while (sched_getweight(getpid()) != weight);
		
		// Do prime factorization 'TRIAL_COUNT' times and print average time of trial
		for (int i = 0; i < TRIAL_COUNT; i++) {
			gettimeofday(&start, NULL);
			factorize(x);
			gettimeofday(&end, NULL);
			elapsed += (double)(end.tv_usec - start.tv_usec) / 1000000 + (double)(end.tv_sec - start.tv_sec);
		}
		printf("[%d] weight %2d: %lf secs\n", weight, weight, elapsed/TRIAL_COUNT);
	}

	return 0;
}

void factorize(int x)
{
	int div = 2;
	while (x > 1) {
		if (x % div == 0) {
			x /= div;
		} else {
			div++;
		}
	}
}
