/*
 * Weighted Round-Robin Scheduling Class (mapped to the SCHED_WRR policy)
 */
#include "sched.h"


static void enqueue_task_wrr(struct rq *rq, struct task_struct *p, int flags) {
	// WRR_TODO
}

static void dequeue_task_wrr(struct rq *rq, struct task_struct *p, int flags) {
	// WRR_TODO
}

static void yield_task_wrr(struct rq *rq) {
	// WRR_TODO
}

static void check_preempt_curr_wrr(struct rq *rq, struct task_struct *p, int flags) {
	// WRR_TODO
}

static void struct task_struct *pick_next_task_wrr(struct rq *rq, struct task_struct *prev, struct rq_flags *rf) {
	// WRR_TODO
}

static void put_prev_task_wrr(struct rq *rq, struct task_struct *p) {
	// WRR_TODO
}

const struct sched_class wrr_sched_class = {
	.next = &fair_sched_class,
	.enqueue_task = enqueue_task_wrr,
	.dequeue_task = dequeue_task_wrr,
	.yield_task = yield_task_wrr,
	.check_preempt_curr = check_preempt_curr_wrr,
	.pick_next_task = pick_next_task_wrr,
	.put_prev_task = put_prev_task_wrr,
};
