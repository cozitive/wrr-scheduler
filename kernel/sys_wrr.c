#include <linux/cred.h>
#include <linux/kernel.h>
#include <linux/smp.h>
#include <linux/syscalls.h>

#include "sched/sched.h"

SYSCALL_DEFINE2(sched_setweight, pid_t, pid, unsigned int, weight)
{
	unsigned int uid;
	struct task_struct *p;
	struct sched_wrr_entity *wrr_se;
	struct wrr_rq *wrr_rq;
	int weight_diff;

	// pid must be positive
	if (pid < 0) {
		return -EINVAL;
	}

	// weight must be in valid range [1, 20]
	if (weight < 1 || weight > 20) {
		return -EINVAL;
	}

	// find task with the given pid
	p = (pid != 0) ? find_task_by_vpid(pid) : &init_task;
	if (p == NULL) {
		return -ESRCH;
	}

	// task's scheduling policy must be WRR
	if (p->policy != SCHED_WRR) {
		return -EINVAL;
	}

	// only administrator or task owner can set weight
	uid = (unsigned int)current_cred()->uid.val;
	if ((uid != 0) && (uid != p->cred->uid.val)) {
		return -EPERM;
	}

	// only administrator can increase weight
	wrr_se = &p->wrr;
	weight_diff = weight - wrr_se->weight;
	if ((uid != 0) && (weight_diff > 0)) {
		return -EPERM;
	}

	// change weight
	wrr_rq = &task_rq(p)->wrr;
	wrr_se->weight += weight_diff;
	wrr_rq->total_weight += weight_diff;

	return 0;
}

SYSCALL_DEFINE1(sched_getweight, pid_t, pid)
{
	struct task_struct *p;

	// pid must be positive
	if (pid < 0) {
		return -EINVAL;
	}

	// find task with the given pid
	p = (pid != 0) ? find_task_by_vpid(pid) : &init_task;
	if (p == NULL) {
		return -ESRCH;
	}

	// task's scheduling policy must be WRR
	if (p->policy != SCHED_WRR) {
		return -EINVAL;
	}

	return p->wrr.weight;
}