#include <sys/syscall.h>
#include "wrappers.h"

long sched_setweight(pid_t pid, unsigned int weight)
{
    return syscall(SYSCALL_SCHED_SETWEIGHT, pid, weight);
}

long sched_getweight(pid_t pid)
{
    return syscall(SYSCALL_SCHED_GETWEIGHT, pid);
}
