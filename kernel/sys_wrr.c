#include <linux/kernel.h>
#include <linux/syscalls.h>

SYSCALL_DEFINE2(sched_setweight, pid_t, pid, unsigned int, weight)
{
	// WRR_TODO: add permission error handling
	struct task_struct *p;
	p = find_task_by_vpid(pid);
	if (pid < 0) {
		printk(KERN_INFO "sched_setweight: pid should be positive");
		return -EINVAL;
	}
	if (weight < 1 || weight > 20) {
		printk(KERN_INFO
		       "sched_setweight: weight should be between 1 and 20");
		return -EINVAL;
	}
	if (p == NULL) {
		printk(KERN_INFO "sched_setweight: process not found");
		return -ESRCH;
	}
	if (p->policy != SCHED_WRR) {
		printk(KERN_INFO
				"sched_setweight: task policy is not WRR");
		return -EINVAL;
	}

	// WRR_TODO: -EINVAL if the task with the given PID is not under the SCHED_WRR policy.
	p->wrr.weight = weight;

	return 0;
}

SYSCALL_DEFINE1(sched_getweight, pid_t, pid)
{
	// WRR_TODO: add permission error handling
	struct task_struct *p;
	p = find_task_by_vpid(pid);
	if (pid < 0) {
		printk(KERN_INFO "sched_getweight: pid should be positive");
		return -EINVAL;
	}
	if (p == NULL) {
		printk(KERN_INFO "sched_getweight: process not found");
		return -ESRCH;
	}

	// WRR_TODO: -EINVAL if the task with the given PID is not under the SCHED_WRR policy.

	return p->wrr.weight;
}