/* Wrapper functions for custom syscalls. */

#include <unistd.h>

#define SYSCALL_SCHED_SETWEIGHT 294
#define SYSCALL_SCHED_GETWEIGHT 295

/// @brief Update the WRR weight of a task.
/// @param pid target task's PID. PID 0 indicates the calling task.
/// @param weight the new weight.
/// @return 0 on success, -1 on error.
long sched_setweight(pid_t pid, unsigned int weight);

/// @brief Query the WRR weight of a task. PID 0 indicates the calling task.
/// @param pid target task's PID.
/// @return the queried weight value on success, -1 on error.
long sched_getweight(pid_t pid);
