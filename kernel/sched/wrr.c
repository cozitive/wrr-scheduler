/*
 * Weighted Round-Robin Scheduling Class (mapped to the SCHED_WRR policy)
 */
#include "sched.h"

/**
 * Initialize a WRR runqueue.
 */
void init_wrr_rq(struct wrr_rq *wrr_rq) {
	INIT_LIST_HEAD(wrr_rq->queue);
	wrr_rq->bit = 0;
	wrr_rq->nr_running = 0;
}

/**
 * Enqueue a task to WRR runqueue.
 */
static void enqueue_task_wrr(struct rq *rq, struct task_struct *p, int flags) {
	// TODO: add `p` to `rq->wrr`
}

/**
 * Dequeue a task from WRR runqueue.
 */
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

#ifdef CONFIG_SMP
static int select_task_rq_wrr(struct task_struct *p, int task_cpu, int sd_flag, int flags) {
	// WRR_TODO
}

static void migrate_task_rq_wrr(struct task_struct *p, int new_cpu) {
	// WRR_TODO
}

static void task_woken_wrr(struct rq *this_rq, struct task_struct *task) {
	// WRR_TODO
}

static void rq_online_wrr(struct rq *rq) {
	// WRR_TODO
}

static void rq_offline_wrr(struct rq *rq) {
	// WRR_TODO
}
#endif

static void set_curr_task_wrr(struct rq *rq) {
	// WRR_TODO
}

static void task_tick_wrr(struct rq *rq, struct task_struct *p, int queued) {
	// WRR_TODO
}

static void task_fork_wrr(struct task_struct *p) {
	// WRR_TODO
}

static void task_dead_wrr(struct task_struct *p) {
	// WRR_TODO
}

static void switched_from_wrr(struct rq *this_rq, struct task_struct *task) {
	// WRR_TODO
}

static void switched_to_wrr(struct rq *this_rq, struct task_struct *task) {
	// WRR_TODO
}

static void prio_changed_wrr(struct rq *this_rq, struct task_struct *task, int oldprio) {
	// WRR_TODO
}

/**
 * Returns WRR timeslice based on task's weight.
 */
static unsigned int get_rr_interval_wrr(struct rq *rq, struct task_struct *task) {
	return task->wrr->weight * 10;
}

static void update_curr_wrr(struct rq *rq) {
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

#ifdef CONFIG_SMP
	.select_task_rq = select_task_rq_wrr,
	.migrate_task_rq = migrate_task_rq_wrr,
	.task_woken = task_woken_wrr,
	.set_cpus_allowed = set_cpus_allowed_common,
	.rq_online = rq_online_wrr,
	.rq_offline = rq_offline_wrr,
#endif

	.set_curr_task = set_curr_task_wrr,
	.task_tick = task_tick_wrr,
	.task_fork = task_fork_wrr,
	.task_dead = task_dead_wrr,
	.switched_from = switched_from_wrr,
	.switched_to = switched_to_wrr,
	.prio_changed = prio_changed_wrr,
	.get_rr_interval = get_rr_interval_wrr,
	.update_curr = update_curr_wrr,
};
