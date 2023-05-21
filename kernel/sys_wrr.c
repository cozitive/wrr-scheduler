#include <linux/cred.h>
#include <linux/kernel.h>
#include <linux/smp.h>
#include <linux/syscalls.h>

#include "sched/sched.h"

SYSCALL_DEFINE2(sched_setweight, pid_t, pid, unsigned int, weight)
{
	struct task_struct *p;
	struct sched_wrr_entity *wrr_se;
	struct wrr_rq *wrr_rq;
	unsigned int old_weight;

	if (pid < 0) {
		printk(KERN_DEBUG "sched_setweight: pid should be positive");
		return -EINVAL;
	}

	if (weight < 1 || weight > 20) {
		printk(KERN_DEBUG
		       "sched_setweight: weight should be between 1 and 20");
		return -EINVAL;
	}

	p = find_task_by_vpid(pid);
	if (p == NULL) {
		printk(KERN_DEBUG "sched_setweight: process not found");
		return -ESRCH;
	}

	if (p->policy != SCHED_WRR) {
		printk(KERN_DEBUG "sched_getweight: process policy is not SCHED_WRR");
		return -EINVAL;
	}

	if (((int)current_cred()->uid.val != 0) && (current->pid != p->pid)) {
		printk(KERN_DEBUG "sched_setweight: no permission to process");
		return -EPERM;
	}

	wrr_se = &p->wrr;
	wrr_rq = &task_rq(p)->wrr;

	old_weight = wrr_se->weight;
	wrr_se->weight = weight;
	wrr_rq->total_weight += (weight - old_weight);
	WARN_ON(wrr_rq->total_weight < 0);

	return 0;
}

SYSCALL_DEFINE1(sched_getweight, pid_t, pid)
{
	struct task_struct *p;

	if (pid < 0) {
		printk(KERN_DEBUG "sched_getweight: pid should be positive");
		return -EINVAL;
	}

	p = find_task_by_vpid(pid);
	if (p == NULL) {
		printk(KERN_DEBUG "sched_getweight: process not found");
		return -ESRCH;
	}

	if (p->policy != SCHED_WRR) {
		printk(KERN_DEBUG "sched_getweight: process policy is not SCHED_WRR");
		return -EINVAL;
	}

	return p->wrr.weight;
}