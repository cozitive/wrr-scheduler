#include <linux/kernel.h>
#include <linux/syscalls.h>

SYSCALL_DEFINE2(sched_setweight, pid_t, pid, unsigned int, weight)
{
	// TODO: implement this
	printk(KERN_INFO "sched_setweight");
	return 0;
}

SYSCALL_DEFINE1(sched_getweight, pid_t, pid)
{
	// TODO: implement this
	printk(KERN_INFO "sched_getweight");
	return 0;
}