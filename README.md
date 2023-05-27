# Weighted Round-Robin (WRR) Scheduler

This project implements a weighted round-robin(WRR) scheduler.

## Build & Execute Commands
### Kernel
```shell
sudo ./build-rpi3.sh
sudo ./setup-images.sh
```

### Turnaround Time Test
```shell
test/compile-mount-and-copy.sh
./qemu.sh

# in QEMU
./dummy 20 0    # dummy at CPU 0
./dummy 20 1    # dummy at CPU 1
./dummy 20 2    # dummy at CPU 2
./dummy 20 3    # dummy at CPU 3
./test          # turnaround time test
```
command line below is usage of `dummy` and `test` in QEMU, parameter* is optional
- ./dummy weight* CPU*
- ./test target_num*

## WRR Scheduler Class
### Data Structures
Some structs are added for representing WRR information.

#### `struct sched_wrr_entity`
It contains WRR-related information of a task.
```c
struct task_struct {
    ...
    struct sched_wrr_entity		wrr;
    ...
}
```
```c
struct sched_wrr_entity {
	unsigned int weight;
	unsigned int time_slice;
	struct list_head run_list;
	unsigned short on_rq;
};
```
- `weight`: weight of the task.
- `time_slice`: remaining ticks for execution.
- `run_list`: list node used in a runqueue.
- `on_rq`: 1 if task is on a runqueue, 0 otherwise.

#### `struct wrr_rq`
It represents runqueue of WRR scheduler.
```c
struct rq {
    ...
    struct wrr_rq 		wrr;
    ...
}
```
```c
struct wrr_rq {
	struct list_head queue;
	unsigned int nr_running;
	unsigned int total_weight;
};
```
- `queue`: the first node of the runqueue list.
- `nr_running`: number of running tasks.
- `total_weight`: total weight of tasks on the runqueue.

### WRR Scheduler Class: `wrr_sched_class`
`wrr_sched_class` locates in `kernel/sched/wrr.c`. Necessary scheduler interface functions are implemented for the WRR scheduler:
- `enqueue_task_wrr()`: enqueue a task to the WRR runqueue.
- `dequeue_task_wrr()`: dequeue a task from the WRR runqueue.
- `yield_task_wrr()`: requeue the current WRR task when yielding.
- `pick_next_task_wrr()`: pick the next task to execute from the WRR runqueue.
- `put_prev_task_wrr()`: requeue the previous WRR task on the runqueue.
- `select_task_rq_wrr()`: select a CPU with minimum total weight to exeucte a task.
  - RCU read lock is used to check total weight of all CPUs.
- `update_curr_wrr()`: update statistics of the current WRR task.
- `task_tick_wrr()`: update timeslice of the current WRR task at every tick.
- `get_rr_interval_wrr()`: return the WRR timeslice based on task's weight.

WRR scheduler replaces the default CFS scheduler:
- `kernel/sched/rt.c`
  - RT scheduler(`rt_sched_class`) points the WRR scheduler, instead of the CFS scheduler.
- `kernel/sched/core.c`
  - `sched_init()` initializes the WRR runqueue at scheduler initialization.
  - `init_idle()` initializes `struct sched_wrr_entity` of the idle process(`swapper`).
  - `sched_fork()` initializes `struct sched_wrr_entity` of the new task, and assigns it to the WRR scheduler if it follows neither DL or RT policy.
  - `rt_mutex_setprio()` assigns all normal tasks to the WRR scheduler.
  - `sched_setscheduler()` assigns all normal tasks to the WRR scheduler, and set the task's weight to default when newly scheduled as WRR.
  - `normalize_rt_tasks()` set the task's scheduler policy to WRR.
- `init/init_task.c`
  - `init_task` is assigned to the WRR scheduler.
- `kernel/kthread.c`
  - `kthread` is assigned to the WRR scheduler.

### WRR System Calls
There are system calls to update and query the weight of a WRR task.
- `sched_setweight(pid, weight)`: set the weight of a WRR task.
  - RCU read lock is used to read task data from all CPUs.
  - `task_rq_lock` is used to modify the weight of a task on the runqueue.
- `sched_getweight(pid)`: return the weight of a WRR task.
  - RCU read lock is used to read task data from all CPUs.

## Load Balancing


## Turnaround Time Test

- We take prime number `300000007` to prime factorization target number 
- All test repeated 5 times and we measured average of the execution time.

First, we executed prime factorization `test` alone. As a result, because there's rare interrupt to test, execution time was around 1.9 secs regardless of task weight

So, we decided to run spinning `dummy` tasks at background to all `CPU`s. Then run `test` and measured execution time. we did 3 trials with different `dummy` weight, 1, 10 and 20.

The graph below shows excution time based on task weight.
- The color of graph is differentiated by weight of dummy task.

<img width="789" alt="KakaoTalk_Photo_2023-05-27-19-01-36" src="test/plot.png">

## Lessons Learned
### Lock Is Important
We used two locks: RCU lock and task/runqueue spinlock.
- RCU lock: read shared data (runqueue of other CPUs)
- spinlock: write to shared data (task/runqueue of other CPUs)

Locks are used in multiprocessor functions.
- `select_task_rq_wrr()`: read all CPUs' total weights
- `sched_setweight()`: fetch a task from all CPUs, and change the task's weight and its WRR runqueue's total weight
- `sched_getweight()`: fetch a task from all CPUs
- `load_balance_wrr()`: read all CPUs' total weights, and move a task from its runqueue to other runqueue

The key was that RCU lock protects **only reading, not writing.** Even if RCU lock is held, appropriate spinlock must be acquired before the value of shared data change.

### Run**Queue**, Not RunStack!
At first, `requeue_task_wrr()` called wrong list API by mistake.
```c
static void requeue_task_wrr(struct rq *rq, struct task_struct *p)
{
	...
	list_move(&wrr_se->run_list, &wrr_rq->queue); // not list_move_tail
}
```
It was just a simple typo, but the side effect was too fatal. It made the runqueue operate like a stack; when a running task ran out of its timeslice, the timeslice is refilled and the recent task is inserted at the head again! Other tasks could not be executed before the current task terminates.

This small mistake destroyed the whole scheduling system:
- A single CPU could not perform multitasking.
- Interrupt during the task could not be handled.
- Kernel processes suffered from starvation.

We keenly realized the importance of choosing appropriate data structures.