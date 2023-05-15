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

/// @param this_cpu The index of the local CPU
/// @param this_rq The address of the descriptor of the local runqueue
/// @param sd Points to the descriptor of the scheduling domain to be checked
/// @param idle Either SCHED_IDLE (local CPU is idle) or NOT_IDLE
/// @param continue_balancing A pointer to a flag that is set to 1 if the function should be called again to check whether the domain is still unbalanced
static int load_balance(int this_cpu, struct rq *this_rq,
			struct sched_domain *sd, enum cpu_idle_type idle,
			int *continue_balancing)
{
	/*
		Load Balancing Algorithm
		1. Select two CPUs with the largest/smallest total weights
			- Call them max_cpu and min_cpu respectively
		2. Pick a single task with the largest weight, which satisfies the following conditions:
			- The task should not be running on a CPU
			- Migration should not make the total weight of min_cpu equal to or greater than that of max_cpu
			- The task’s CPU affinity should allow migrating the task to min_cpu
		3. Perform load balancing if a transferable task exists
			- There may be no such task (don’t print logs, then)
	 */

	int max_cpu, min_cpu;

	// 1. Select two CPUs with the largest/smallest total weights
	// 1-1. Find the CPU with the largest total weight
	max_cpu = find_busiest_group(sd, CPU_NEWLY_IDLE, idle, NULL);



}

/*
 * It checks each scheduling domain to see if it is due to be balanced,
 * and initiates a balancing operation if so.
 *
 * Balancing parameters are set up in init_sched_domains.
 */
static void rebalance_domains(struct rq *rq, enum cpu_idle_type idle)
{
	// LB_TODO
}

/*
 * idle_balance is called by schedule() if this_cpu is about to become
 * idle. Attempts to pull tasks from other CPUs.
 */
static int idle_balance(struct rq *this_rq, struct rq_flags *rf)
{
	// LB_TODO
}

/*
 * run_rebalance_domains is triggered when needed from the scheduler tick.
 * Also triggered for nohz idle balancing (with nohz_balancing_kick set).
 */
static __latent_entropy void run_rebalance_domains(struct softirq_action *h)
{
	struct rq *this_rq = this_rq();
	enum cpu_idle_type idle =
		this_rq->idle_balance ? CPU_IDLE : CPU_NOT_IDLE;

	/*
	 * If this CPU has a pending nohz_balance_kick, then do the
	 * balancing on behalf of the other idle CPUs whose ticks are
	 * stopped. Do nohz_idle_balance *before* rebalance_domains to
	 * give the idle CPUs a chance to load balance. Else we may
	 * load balance only within the local sched_domain hierarchy
	 * and abort nohz_idle_balance altogether if we pull some load.
	 */
	if (nohz_idle_balance(this_rq, idle))
		return;

	/* normal load balance */
	update_blocked_averages(this_rq->cpu);
	rebalance_domains(this_rq, idle);
}

/*
 * Trigger the SCHED_SOFTIRQ if it is time to do periodic load balancing.
 */
void trigger_load_balance(struct rq *rq)
{
	/* Don't need to rebalance while attached to NULL domain */
	if (unlikely(on_null_domain(rq)))
		return;

	/* trigger_load_balance() checks a timer and if balancing is due, it fires the soft irq with the corresponding flag SCHED_SOFTIRQ. */
	if (time_after_eq(jiffies, rq->next_balance))
		raise_softirq(
			SCHED_SOFTIRQ); // The function registered as irq handler is run_rebalance_domains()

	nohz_balancer_kick(rq);
}

static inline void update_next_balance(struct sched_domain *sd,
				       unsigned long *next_balance)
{
	unsigned long interval, next;

	/* used by idle balance, so cpu_busy = 0 */
	interval = msecs_to_jiffies(2000); // Load balance every 2000 ms
	next = sd->last_balance + interval;

	if (time_after(*next_balance, next))
		*next_balance = next;
}

__init void init_sched_wrr_class(void)
{
#ifdef CONFIG_SMP
	open_softirq(SCHED_SOFTIRQ, run_rebalance_domains);

#ifdef CONFIG_NO_HZ_COMMON
	nohz.next_balance = jiffies;
	nohz.next_blocked = jiffies;
	zalloc_cpumask_var(&nohz.idle_cpus_mask, GFP_NOWAIT);
#endif
#endif /* SMP */
}
