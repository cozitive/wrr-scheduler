/*
 * Weighted Round-Robin Scheduling Class (mapped to the SCHED_WRR policy)
 */
#include "sched.h"

extern static inline bool move_entity(unsigned int flags);

/// @brief Initialize a WRR runqueue.
/// @param wrr_rq WRR runqueue to initiate.
void init_wrr_rq(struct wrr_rq *wrr_rq) {
	INIT_LIST_HEAD(wrr_rq->queue);
	wrr_rq->bit = 0;
	wrr_rq->nr_running = 0;
}

/// @brief Get the task_struct of a WRR scheduler entity.
static inline struct task_struct *wrr_task_of(struct sched_wrr_entity *wrr_se) {
	return container_of(wrr_se, struct task_struct, wrr);
}

/// @brief Get the runqueue of a WRR runqueue.
static inline struct rq *rq_of_wrr_rq(struct wrr_rq *wrr_rq) {
	return container_of(wrr_rq, struct rq, wrr)
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

#ifdef CONFIG_SMP
static void enqueue_pushable_task_wrr(struct rq *rq, struct task_struct *p) {
	// TODO
}

static void dequeue_pushable_task_wrr(struct rq *rq, struct task_struct *p) {
	// TODO
}
#endif

static inline void enqueue_pushable_task_wrr(struct rq *rq, struct task_struct *p) {}

static inline void dequeue_pushable_task_wrr(struct rq *rq, struct task_struct *p) {}

/// @brief Increment runqueue variables after the enqueue.
static inline void inc_wrr_tasks(struct sched_wrr_entity *wrr_se, struct wrr_rq *wrr_rq) {
	wrr_rq->nr_running += 1;
	// TODO: SMP
}

/// @brief Decrement runqueue variables after the dequeue.
static inline void dec_wrr_tasks(struct sched_wrr_entity *wrr_se, struct wrr_rq *wrr_rq) {
	WARN_ON(!wrr_rq->nr_running);
	wrr_rq->nr_running -= 1;
	// TODO: SMP
}

/// @brief Enqueue a task to WRR runqueue.
/// @param rq a runqueue.
/// @param p a task to be enqueued to WRR runqueue of `rq`.
/// @param flags optional flags.
static void enqueue_task_wrr(struct rq *rq, struct task_struct *p, int flags) {
	struct sched_wrr_entity *wrr_se = &p->wrr;
	struct wrr_rq *wrr_rq = wrr_rq_of_se(wrr_se);

	// enqueue `wrr_se` to WRR runqueue
	if (move_entity(flags)) {
		// assume `wrr_se` is not in the queue
		WARN_ON_ONCE(wrr_se->on_rq);

		// if ENQUEUE_HEAD, insert `wrr_se` at the head
        // if !ENQUEUE_HEAD, insert `wrr_se` at the tail
		if (flags & ENQUEUE_HEAD)
            list_add(&wrr_se->run_list, wrr_rq->queue);
        else
            list_add_tail(&wrr_se->run_list, wrr_rq->queue);

		// set flag
		wrr_rq->bit = 1;
		wrr_se->on_rq = 1;

		// increment runqueue variables
		inc_wrr_tasks(wrr_se, wrr_rq);

		// SMP
		if (!task_current(rq, p) && p->nr_cpus_allowed > 1) {
			enqueue_pushable_task_wrr(rq, p);
		}
	}
}

/// @brief Dequeue a task from WRR runqueue.
/// @param rq a runqueue.
/// @param p a task to be dequeued from WRR runqueue of `rq`.
/// @param flags optional flags.
static void dequeue_task_wrr(struct rq *rq, struct task_struct *p, int flags) {
	struct sched_wrr_entity *wrr_se = &p->wrr;
	struct wrr_rq *wrr_rq = wrr_rq_of_se(wrr_se);

	// dequeue `wrr_se` from WRR runqueue
	if (on_wrr_rq(wrr_se) && move_entity(flags)) {
		// assume `wrr_se` is in the queue
		WARN_ON_ONCE(!wrr_se->on_rq);

		// dequeue `wrr_se`
		list_del_init(&wrr_se->run_list);

		// set flag
		if (list_empty(wrr_rq->queue))
			wrr_rq->bit = 0;
		wrr_se->on_rq = 0;

		// decrement runqueue variables
		dec_wrr_tasks(wrr_se, wrr_rq);
	}

	// SMP
	dequeue_pushable_task_wrr(rq, p);
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

/// @brief Return WRR timeslice based on task's weight.
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

static __latent_entropy void run_load_balance_wrr(struct softirq_action *h)
{
	struct rq *this_rq = this_rq();
	load_balance_wrr(this_rq);
}

/// find_busiest_queue 참고
static int load_balance_wrr(struct rq *rq)
{
	int curr_cpu, max_cpu = -1, min_cpu = -1;
	struct rq *rq;
	unsigned long next_balance = jiffies + msecs_to_jiffies(2000);

	// Iterate over all online cpus
	for_each_online_cpu(curr_cpu)
	{
		if (max_cpu == -1) max_cpu = curr_cpu;
		if (min_cpu == -1) min_cpu = curr_cpu;
		rq = cpu_rq(curr_cpu);
	}
}

/*
 * Trigger the SCHED_SOFTIRQ_WRR if it is time to do periodic load balancing.
 */
void trigger_load_balance_wrr(struct rq *rq)
{
	/* Don't need to rebalance while attached to NULL domain */
	if (unlikely(on_null_domain(rq)))
		return;

	/* trigger_load_balance_wrr() checks a timer and if balancing is due, it fires the soft irq with the corresponding flag SCHED_SOFTIRQ_WRR. */
	if (time_after_eq(jiffies, rq->next_balance_wrr))
		raise_softirq(
			SCHED_SOFTIRQ_WRR);
}

// static inline void update_next_balance_wrr(struct sched_domain *sd,
// 				       unsigned long *next_balance_wrr)
// {
// 	unsigned long interval, next;

// 	/* used by idle balance, so cpu_busy = 0 */
// 	interval = msecs_to_jiffies(2000); // Load balance every 2000 ms
// 	next = sd->last_balance + interval;

// 	if (time_after(*next_balance_wrr, next))
// 		*next_balance_wrr = next;
// }

__init void init_sched_wrr_class(void)
{
#ifdef CONFIG_SMP
	open_softirq(SCHED_SOFTIRQ_WRR, run_load_balance_wrr);
#endif /* SMP */
}
