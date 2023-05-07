/*
 * Weighted Round-Robin Scheduling Class (mapped to the SCHED_WRR policy)
 */
#include "sched.h"


const struct sched_class wrr_sched_class = {
	.next = &idle_sched_class,
	.enqueue_task = enqueue_task_wrr,
	.dequeue_task = dequeue_task_wrr,
	.yield_task = yield_task_wrr,
	.check_preempt_curr = check_preempt_curr_wrr,
	.pick_next_task = pick_next_task_wrr,
	.put_prev_task = put_prev_task_wrr,
}