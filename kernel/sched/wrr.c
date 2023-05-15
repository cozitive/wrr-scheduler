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
static int load_balance(int this_cpu, struct rq *this_rq,
			struct sched_domain *sd)
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

	int ld_moved, cur_ld_moved, active_balance = 0;
	struct sched_domain *sd_parent = sd->parent;
	struct sched_group *group; // Busiest group
	struct rq *busiest; // Busiest runqueue
	struct rq_flags rf;
	struct cpumask *cpus = this_cpu_cpumask_var_ptr(load_balance_mask);

	struct lb_env env = {
		.sd = sd,
		.dst_cpu = this_cpu,
		.dst_rq = this_rq,
		.dst_grpmask = sched_group_span(sd->groups),
		.idle = idle,
		.loop_break = sched_nr_migrate_break,
		.cpus = cpus,
		.fbq_type = all,
		.tasks = LIST_HEAD_INIT(env.tasks),
	};

	cpumask_and(cpus, sched_domain_span(sd), cpu_active_mask);

	schedstat_inc(sd->lb_count[idle]);

redo:
	if (!should_we_balance(&env)) {
		*continue_balancing = 0;
		goto out_balanced;
	}

	group = find_busiest_group(
		&env); // Invokes the find_busiest_group( ) function to analyze the workloads of the groups inside the scheduling domain.

	/* If find_busiest_group( ) did not find a group not including the local CPU that is significantly busier than the other groups in the scheduling domain, the function releases the this_rq->lock spin lock, tunes the parameters in the scheduling domain descriptor so as to delay the next invocation of load_balance( ) on the local CPU, and terminates. */
	if (!group) {
		schedstat_inc(sd->lb_nobusyg[idle]);
		goto out_balanced;
	}

	/* Invokes the find_busiest_queue( ) function to find the busiest CPUs in the group found in step 2. The function returns the descriptor address `busiest` of the corresponding runqueue. */
	busiest = find_busiest_queue(&env, group);
	if (!busiest) {
		schedstat_inc(sd->lb_nobusyq[idle]);
		goto out_balanced;
	}

	BUG_ON(busiest == env.dst_rq);

	schedstat_add(sd->lb_imbalance[idle], env.imbalance);

	env.src_cpu = busiest->cpu;
	env.src_rq = busiest;

	ld_moved = 0;
	if (busiest->nr_running > 1) {
		/*
		 * Attempt to move tasks. If find_busiest_group has found
		 * an imbalance but busiest->nr_running <= 1, the group is
		 * still unbalanced. ld_moved simply stays zero, so it is
		 * correctly treated as an imbalance.
		 */
		env.flags |= LBF_ALL_PINNED;
		env.loop_max =
			min(sysctl_sched_nr_migrate, busiest->nr_running);

	more_balance:
		rq_lock_irqsave(busiest, &rf);
		update_rq_clock(busiest);

		/*
		 * cur_ld_moved - load moved in current iteration
		 * ld_moved     - cumulative load moved across iterations
		 */
		cur_ld_moved = detach_tasks(&env);

		/*
		 * We've detached some tasks from busiest_rq. Every
		 * task is masked "TASK_ON_RQ_MIGRATING", so we can safely
		 * unlock busiest->lock, and we are able to be sure
		 * that nobody can manipulate the tasks in parallel.
		 * See task_rq_lock() family for the details.
		 */

		rq_unlock(busiest, &rf);

		if (cur_ld_moved) {
			attach_tasks(&env);
			ld_moved += cur_ld_moved;
		}

		local_irq_restore(rf.flags);

		if (env.flags & LBF_NEED_BREAK) {
			env.flags &= ~LBF_NEED_BREAK;
			goto more_balance;
		}

		/*
		 * Revisit (affine) tasks on src_cpu that couldn't be moved to
		 * us and move them to an alternate dst_cpu in our sched_group
		 * where they can run. The upper limit on how many times we
		 * iterate on same src_cpu is dependent on number of CPUs in our
		 * sched_group.
		 *
		 * This changes load balance semantics a bit on who can move
		 * load to a given_cpu. In addition to the given_cpu itself
		 * (or a ilb_cpu acting on its behalf where given_cpu is
		 * nohz-idle), we now have balance_cpu in a position to move
		 * load to given_cpu. In rare situations, this may cause
		 * conflicts (balance_cpu and given_cpu/ilb_cpu deciding
		 * _independently_ and at _same_ time to move some load to
		 * given_cpu) causing exceess load to be moved to given_cpu.
		 * This however should not happen so much in practice and
		 * moreover subsequent load balance cycles should correct the
		 * excess load moved.
		 */
		if ((env.flags & LBF_DST_PINNED) && env.imbalance > 0) {
			/* Prevent to re-select dst_cpu via env's CPUs */
			cpumask_clear_cpu(env.dst_cpu, env.cpus);

			env.dst_rq = cpu_rq(env.new_dst_cpu);
			env.dst_cpu = env.new_dst_cpu;
			env.flags &= ~LBF_DST_PINNED;
			env.loop = 0;
			env.loop_break = sched_nr_migrate_break;

			/*
			 * Go back to "more_balance" rather than "redo" since we
			 * need to continue with same src_cpu.
			 */
			goto more_balance;
		}

		/*
		 * We failed to reach balance because of affinity.
		 */
		if (sd_parent) {
			int *group_imbalance =
				&sd_parent->groups->sgc->imbalance;

			if ((env.flags & LBF_SOME_PINNED) && env.imbalance > 0)
				*group_imbalance = 1;
		}

		/* All tasks on this runqueue were pinned by CPU affinity */
		if (unlikely(env.flags & LBF_ALL_PINNED)) {
			cpumask_clear_cpu(cpu_of(busiest), cpus);
			/*
			 * Attempting to continue load balancing at the current
			 * sched_domain level only makes sense if there are
			 * active CPUs remaining as possible busiest CPUs to
			 * pull load from which are not contained within the
			 * destination group that is receiving any migrated
			 * load.
			 */
			if (!cpumask_subset(cpus, env.dst_grpmask)) {
				env.loop = 0;
				env.loop_break = sched_nr_migrate_break;
				goto redo;
			}
			goto out_all_pinned;
		}
	}

	if (!ld_moved) {
		schedstat_inc(sd->lb_failed[idle]);
		/*
		 * Increment the failure counter only on periodic balance.
		 * We do not want newidle balance, which can be very
		 * frequent, pollute the failure counter causing
		 * excessive cache_hot migrations and active balances.
		 */
		if (idle != CPU_NEWLY_IDLE)
			sd->nr_balance_failed++;

		if (need_active_balance(&env)) {
			unsigned long flags;

			raw_spin_lock_irqsave(&busiest->lock, flags);

			/*
			 * Don't kick the active_load_balance_cpu_stop,
			 * if the curr task on busiest CPU can't be
			 * moved to this_cpu:
			 */
			if (!cpumask_test_cpu(this_cpu,
					      &busiest->curr->cpus_allowed)) {
				raw_spin_unlock_irqrestore(&busiest->lock,
							   flags);
				env.flags |= LBF_ALL_PINNED;
				goto out_one_pinned;
			}

			/*
			 * ->active_balance synchronizes accesses to
			 * ->active_balance_work.  Once set, it's cleared
			 * only after active load balance is finished.
			 */
			if (!busiest->active_balance) {
				busiest->active_balance = 1;
				busiest->push_cpu = this_cpu;
				active_balance = 1;
			}
			raw_spin_unlock_irqrestore(&busiest->lock, flags);

			if (active_balance) {
				stop_one_cpu_nowait(
					cpu_of(busiest),
					active_load_balance_cpu_stop, busiest,
					&busiest->active_balance_work);
			}

			/* We've kicked active balancing, force task migration. */
			sd->nr_balance_failed = sd->cache_nice_tries + 1;
		}
	} else
		sd->nr_balance_failed = 0;

	if (likely(!active_balance)) {
		/* We were unbalanced, so reset the balancing interval */
		sd->balance_interval = sd->min_interval;
	} else {
		/*
		 * If we've begun active balancing, start to back off. This
		 * case may not be covered by the all_pinned logic if there
		 * is only 1 task on the busy runqueue (because we don't call
		 * detach_tasks).
		 */
		if (sd->balance_interval < sd->max_interval)
			sd->balance_interval *= 2;
	}

	goto out;

out_balanced:
	/*
	 * We reach balance although we may have faced some affinity
	 * constraints. Clear the imbalance flag if it was set.
	 */
	if (sd_parent) {
		int *group_imbalance = &sd_parent->groups->sgc->imbalance;

		if (*group_imbalance)
			*group_imbalance = 0;
	}

out_all_pinned:
	/*
	 * We reach balance because all tasks are pinned at this level so
	 * we can't migrate them. Let the imbalance flag set so parent level
	 * can try to migrate them.
	 */
	schedstat_inc(sd->lb_balanced[idle]);

	sd->nr_balance_failed = 0;

out_one_pinned:
	/* tune up the balancing interval */
	if (((env.flags & LBF_ALL_PINNED) &&
	     sd->balance_interval < MAX_PINNED_INTERVAL) ||
	    (sd->balance_interval < sd->max_interval))
		sd->balance_interval *= 2;

	ld_moved = 0;
out:
	return ld_moved;
}

/*
 * It checks each scheduling domain to see if it is due to be balanced,
 * and initiates a balancing operation if so.
 *
 * Balancing parameters are set up in init_sched_domains.
 */
/* rebalance_domains() then walks up the domain hierarchy and calls load_balance() if the domain has the SD_LOAD_BALANCE flag set and its balancing interval is expired. The balancing interval of a domain is in jiffies and updated after each balancing run. */
/// @param rq The runqueue of the current CPU
static void rebalance_domains(struct rq *rq)
{
	int cpu = rq->cpu; // The current CPU
	unsigned long interval;
	struct sched_domain *sd; // Pointer to the current scheduling domain
	/* Earliest time when we have to do rebalance again */
	unsigned long next_balance = jiffies + 60 * HZ;
	int update_next_balance = 0;
	int need_serialize, need_decay = 0;
	u64 max_cost = 0;

	rcu_read_lock();
	for_each_domain(cpu, sd)
	{
		/*
		 * Decay the newidle max times here because this is a regular
		 * visit to all the domains. Decay ~1% per second.
		 */
		if (time_after(jiffies, sd->next_decay_max_lb_cost)) {
			sd->max_newidle_lb_cost =
				(sd->max_newidle_lb_cost * 253) / 256;
			sd->next_decay_max_lb_cost = jiffies + HZ;
			need_decay = 1;
		}
		max_cost += sd->max_newidle_lb_cost;

		if (!(sd->flags & SD_LOAD_BALANCE))
			continue;

		/*
		 * Stop the load balance at this level. There is another
		 * CPU in our sched group which is doing load balancing more
		 * actively.
		 */
		if (!continue_balancing) {
			if (need_decay)
				continue;
			break;
		}

		interval = get_sd_balance_interval(sd, idle != CPU_IDLE);

		need_serialize = sd->flags & SD_SERIALIZE;
		if (need_serialize) {
			if (!spin_trylock(&balancing))
				goto out;
		}

		if (time_after_eq(jiffies, sd->last_balance + interval)) {
			if (load_balance(cpu, rq, sd, idle,
					 &continue_balancing)) {
				/*
				 * The LBF_DST_PINNED logic could have changed
				 * env->dst_cpu, so we can't know our idle
				 * state even if we migrated tasks. Update it.
				 */
				idle = idle_cpu(cpu) ? CPU_IDLE : CPU_NOT_IDLE;
			}
			sd->last_balance = jiffies;
			interval =
				get_sd_balance_interval(sd, idle != CPU_IDLE);
		}
		if (need_serialize)
			spin_unlock(&balancing);
	out:
		if (time_after(next_balance, sd->last_balance + interval)) {
			next_balance = sd->last_balance + interval;
			update_next_balance = 1;
		}
	}
	if (need_decay) {
		/*
		 * Ensure the rq-wide value also decays but keep it at a
		 * reasonable floor to avoid funnies with rq->avg_idle.
		 */
		rq->max_idle_balance_cost =
			max((u64)sysctl_sched_migration_cost, max_cost);
	}
	rcu_read_unlock();

	/*
	 * next_balance will be updated only when there is a need.
	 * When the cpu is attached to null domain for ex, it will not be
	 * updated.
	 */
	if (likely(update_next_balance)) {
		rq->next_balance = next_balance;
	}
}

/*
 * run_rebalance_domains is triggered when needed from the scheduler tick.
 */
static __latent_entropy void run_rebalance_domains(struct softirq_action *h)
{
	struct rq *this_rq = this_rq();

	update_blocked_averages(this_rq->cpu);
	rebalance_domains(this_rq);
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
	open_softirq(SCHED_SOFTIRQ_WRR, run_rebalance_domains);
#endif /* SMP */
}
