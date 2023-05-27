#include <stdio.h>
#include "wrappers.h"
#include <unistd.h>
#include <stdlib.h>
#include <sched.h>

#define MAX_WEIGHT 20

int main(int argc, char *argv[])
{
	if (getuid() != 0) {
		printf("should be run in root user\n");
		return 0;
	}
    int pid = getpid();
    sched_setweight(pid, MAX_WEIGHT);
    while(1);
}