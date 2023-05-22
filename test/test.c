#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <sched.h>
#include <sys/time.h>
#include "wrappers.h"

#define X_DEFAULT 999999937
#define MAX_WEIGHT 20
#define TRIAL_COUNT 3

void factorize(int x);

int main(int argc, char *argv[])
{
    srand(time(NULL));
    int x = (argc > 1) ? rand() : X_DEFAULT;
	int pid = getpid();
    struct timeval start, end;
	double turnaround[MAX_WEIGHT];

	printf("WRR turnaround time test: prime factoriazation of %d\n\n", x);

	for (int weight = 1; weight <= MAX_WEIGHT; weight++) {
		int i = weight - 1;
		turnaround[i] = 0;
		printf("========== weight <%d> ==========\n", weight);
		sched_setweight(pid, weight);

		for (int cnt = 0; cnt < TRIAL_COUNT; cnt++) {
			gettimeofday(&start, NULL);
			factorize(x);
			gettimeofday(&end, NULL);

			double elapsed = (double)(end.tv_usec - start.tv_usec) / 1000000 + (double)(end.tv_sec - start.tv_sec);
			printf("\t[%d] %lf secs\n", cnt, elapsed);
			turnaround[i] += elapsed;
		}
		printf("---------------------------------\n");
		printf("sum: %lf secs\tavg: %lf secs\n\n", turnaround[i], turnaround[i] / TRIAL_COUNT);
	}

	FILE *f = fopen("out.txt", "w");
	for (int i = 0; i < 20; i++) {
		fprintf(f, "weight: %d\tsum: %lf\tavg: %lf\n", i + 1, turnaround[i], turnaround[i] / TRIAL_COUNT);
	}	
	fclose(f);

	return 0;
}

void factorize(int x)
{
	int div = 2;
	int is_first = 1;

	printf("%d = ", x);
	while (x > 1) {
		if (x % div == 0) {
			if (is_first) {
				printf("%d", div);
				is_first = 0;
			} else {
				printf(" * %d", div);
			}
			x /= div;
		} else {
			div++;
		}
	}
}
