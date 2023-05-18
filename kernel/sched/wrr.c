/*
 * Weighted Round-Robin Scheduling Class (mapped to the SCHED_WRR policy)
 */
#include "sched.h"

/// @brief Initialize a WRR runqueue.
/// @param wrr_rq WRR runqueue to initiate.
void init_wrr_rq(struct wrr_rq *wrr_rq) {
	INIT_LIST_HEAD(&wrr_rq->queue);
	wrr_rq->nr_running = 0;
	wrr_rq->total_weight = 0;
}

/// @brief Get the task_struct of a WRR scheduler entity.
static inline struct task_struct *wrr_task_of(struct sched_wrr_entity *wrr_se) {
	return container_of(wrr_se, struct task_struct, wrr);
}

/// @brief Get the runqueue of a WRR runqueue.
static inline struct rq *rq_of_wrr_rq(struct wrr_rq *wrr_rq) {
	return container_of(wrr_rq, struct rq, wrr);
}

/// @brief Get the runqueue of a WRR scheduler entity.
static inline struct rq *rq_of_wrr_se(struct sched_wrr_entity *wrr_se) {
	struct task_struct *p = wrr_task_of(wrr_se);
	return task_rq(p);
}

/// @brief Get the WRR runqueue of a WRR scheduler entity.
static inline struct wrr_rq *wrr_rq_of_se(struct sched_wrr_entity *wrr_se) {
	struct rq *rq = rq_of_wrr_se(wrr_se);
	return &rq->wrr;
}

static inline int on_wrr_rq(struct sched_wrr_entity *wrr_se) {
	return wrr_se->on_rq;
}

/// @brief Increment runqueue variables after the enqueue.
static inline void inc_wrr_tasks(struct sched_wrr_entity *wrr_se, struct wrr_rq *wrr_rq) {
	wrr_se->on_rq = 1;
	wrr_rq->nr_running += 1;
	wrr_rq->total_weight += wrr_se->weight;
}

/// @brief Decrement runqueue variables after the dequeue.
static inline void dec_wrr_tasks(struct sched_wrr_entity *wrr_se, struct wrr_rq *wrr_rq) {
	WARN_ON(!wrr_rq->nr_running);
	WARN_ON(!wrr_rq->total_weight);
	wrr_se->on_rq = 0;
	wrr_rq->nr_running -= 1;
	wrr_rq->total_weight -= wrr_se->weight;
}

/// @brief Enqueue a task to WRR runqueue.
/// @param rq a runqueue.
/// @param p a task to be enqueued to WRR runqueue of `rq`.
/// @param flags optional flags.
static void enqueue_task_wrr(struct rq *rq, struct task_struct *p, int flags) {
	struct sched_wrr_entity *wrr_se = &p->wrr;
	struct wrr_rq *wrr_rq = wrr_rq_of_se(wrr_se);

	if (on_wrr_rq(wrr_se))
		return;

	list_add_tail(&wrr_se->run_list, &wrr_rq->queue);

	inc_wrr_tasks(wrr_se, wrr_rq);
	add_nr_running(rq, 1);
}

/// @brief Dequeue a task from WRR runqueue.
/// @param rq a runqueue.
/// @param p a task to be dequeued from WRR runqueue of `rq`.
/// @param flags optional flags.
static void dequeue_task_wrr(struct rq *rq, struct task_struct *p, int flags) {
	struct sched_wrr_entity *wrr_se = &p->wrr;
	struct wrr_rq *wrr_rq = wrr_rq_of_se(wrr_se);

	if (!on_wrr_rq(wrr_se))
		return;

	list_del_init(&wrr_se->run_list);

	dec_wrr_tasks(wrr_se, wrr_rq);
	sub_nr_running(rq, 1);
}

static void requeue_task_wrr(struct rq *rq, struct task_struct *p) {
	struct sched_wrr_entity *wrr_se = &p->wrr;
	struct wrr_rq *wrr_rq = &rq->wrr;

	if (on_wrr_rq(wrr_se)) {
		list_move(&wrr_se->run_list , &wrr_rq->queue);
	}
}

static void yield_task_wrr(struct rq *rq) {
	requeue_task_wrr(rq, rq->curr);
}

static void check_preempt_curr_wrr(struct rq *rq, struct task_struct *p, int flags) {}

/// @brief Pick next task from WRR runqueue.
/// @param rq a runqueue.
/// @param prev previously executed task.
/// @param rf runqueue flags.
static struct task_struct *pick_next_task_wrr(struct rq *rq, struct task_struct *prev, struct rq_flags *rf) {
	struct wrr_rq *wrr_rq = &rq->wrr;
	struct sched_wrr_entity *wrr_se;

	put_prev_task(rq, prev);

	wrr_se = list_first_entry_or_null(&wrr_rq->queue, struct sched_wrr_entity, run_list);
	if (!wrr_se)
		return NULL;

	return wrr_task_of(wrr_se);
}

static void put_prev_task_wrr(struct rq *rq, struct task_struct *prev) {}

#ifdef CONFIG_SMP
static int select_task_rq_wrr(struct task_struct *p, int cpu, int sd_flag, int flags) {
	unsigned int min_cpu_index = -1;
	unsigned int min_total_weight = UINT_MAX;

	rcu_read_lock();
	for_each_online_cpu(cpu) {
		struct wrr_rq *wrr_rq = &cpu_rq(cpu)->wrr;
		if (cpumask_test_cpu(cpu, &p->cpus_allowed) && (wrr_rq->total_weight < min_total_weight)) {
			min_cpu_index = cpu;
			min_total_weight = wrr_rq->total_weight;
		}
	}
	rcu_read_unlock();

	return min_cpu_index;
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

static void set_curr_task_wrr(struct rq *rq) {}

static void update_curr_wrr(struct rq *rq) {
	struct task_struct *curr = rq->curr;
	u64 delta_exec;
	u64 now;

	if (curr->sched_class != &wrr_sched_class)
		return;

	now = rq_clock_task(rq);
	delta_exec = now - curr->se.exec_start;
	if (unlikely((s64)delta_exec <= 0))
		return;

	schedstat_set(curr->se.statistics.exec_max,
		      max(curr->se.statistics.exec_max, delta_exec));

	curr->se.sum_exec_runtime += delta_exec;
	account_group_exec_runtime(curr, delta_exec);

	curr->se.exec_start = now;
	cgroup_account_cputime(curr, delta_exec);
}

static void task_tick_wrr(struct rq *rq, struct task_struct *p, int queued) {
	struct sched_wrr_entity *wrr_se = &p->wrr;

	update_curr_wrr(rq);

	if (p->policy != SCHED_WRR)
		return;

	if (--wrr_se->time_slice) {
		return;
	}

	wrr_se->time_slice = wrr_se->weight * 10;

	if (wrr_se->run_list.prev != wrr_se->run_list.next) {
		requeue_task_wrr(rq, p);
		resched_curr(rq);
	}
}

static void task_fork_wrr(struct task_struct *p) {
	p->wrr.on_rq = 0;
}

/// @brief Return WRR timeslice based on task's weight.
static unsigned int get_rr_interval_wrr(struct rq *rq, struct task_struct *task) {
	return task->wrr.weight * 10;
}

static void prio_changed_wrr(struct rq *rq, struct task_struct *p, int oldprio) {}

static void switched_from_wrr(struct rq *rq, struct task_struct *p) {}

static void switched_to_wrr(struct rq *rq, struct task_struct *p) {}

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
	.get_rr_interval = get_rr_interval_wrr,
	.prio_changed = prio_changed_wrr,
	.switched_from = switched_from_wrr,
	.switched_to = switched_to_wrr,
	.update_curr = update_curr_wrr,
};
