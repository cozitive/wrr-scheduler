#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/unistd.h>
#include <linux/sched.h>
#include <sched.h>
#include <sys/time.h>
#include "wrappers.h"

double total_weight[20];
int trial_num = 3;

int main(int argc, char *argv[])
{
    srand(time(NULL));
    int x = 999999937;
	int pid = getpid();
    struct timeval start, end;

	printf("trial: pid[%d]\n", pid);

	for (int weight = 0; weight < 20; weight++) {
		total_weight[weight] = 0;
		printf("==== weight <%d> ====\n", weight+1);
		sched_setweight(pid, weight+1);

		for (int cnt = 0; cnt < trial_num; cnt++) {
			gettimeofday(&start, NULL);
            //if there's no input, x is default 999999937
			if (argc > 1){ x = rand(); }
			else{ x = 999999937; }

			int div = 2;
			while (x > 1) {
       			if (x % div == 0) {
            		printf("%d ", div);
            		x /= div;
        		} else {
            		div += 2;
        		}
    		}
			gettimeofday(&end, NULL);

			double elapsed = (double)(end.tv_usec - start.tv_usec) / 1000000 + (double)(end.tv_sec - start.tv_sec);
			printf("\t[%d] %lf\n", cnt, elapsed);
			total_weight[weight] += elapsed;
		}
		printf("----------\n");
		printf("sum: %lf\tavg: %lf\n\n", total_weight[weight], total_weight[weight]/trial_num);
	}

	FILE *f = fopen("out.txt", "w");
	for (int i = 0; i < 20; i++) {
		fprintf(f, "weight: %d\tsum: %lf\tavg: %lf\n", i+1, total_weight[i], total_weight[i]/trial_num);
	}	
	fclose(f);

	return 0;
}