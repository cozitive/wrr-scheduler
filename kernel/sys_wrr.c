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
	struct rq *rq;
	struct rq_flags rf;

	// pid must be positive
	if (pid < 0) {
		return -EINVAL;
	}

	// weight must be in valid range [1, 20]
	if (weight < 1 || weight > 20) {
		return -EINVAL;
	}

	rcu_read_lock();

	// find task with the given pid
	p = (pid != 0) ? find_task_by_vpid(pid) : current;
	if (p == NULL) {
		rcu_read_unlock();
		return -ESRCH;
	}

	// task's scheduling policy must be WRR
	if (p->policy != SCHED_WRR) {
		rcu_read_unlock();
		return -EINVAL;
	}

	// only administrator or task owner can set weight
	uid = (unsigned int)current_cred()->uid.val;
	if ((uid != 0) && (uid != p->cred->uid.val)) {
		rcu_read_unlock();
		return -EPERM;
	}

	// only administrator can increase weight
	wrr_se = &p->wrr;
	weight_diff = weight - wrr_se->weight;
	if ((uid != 0) && (weight_diff > 0)) {
		rcu_read_unlock();
		return -EPERM;
	}

	rq = task_rq_lock(p, &rf);
	rcu_read_unlock(); // No deadlock problem, because it is rcu read lock, not mutex

	// change weight
	wrr_rq = &rq->wrr;
	wrr_se->weight += weight_diff;
	wrr_rq->total_weight += weight_diff;

	task_rq_unlock(rq, p, &rf);

	return 0;
}

SYSCALL_DEFINE1(sched_getweight, pid_t, pid)
{
	struct task_struct *p;
	unsigned int ret_val; // return value

	// pid must be positive
	if (pid < 0) {
		return -EINVAL;
	}

	rcu_read_lock();

	// find task with the given pid
	p = (pid != 0) ? find_task_by_vpid(pid) : current;
	if (p == NULL) {
		rcu_read_unlock();
		return -ESRCH;
	}

	// task's scheduling policy must be WRR
	if (p->policy != SCHED_WRR) {
		rcu_read_unlock();
		return -EINVAL;
	}

	ret_val = p->wrr.weight;

	rcu_read_unlock();

	return ret_val;
}