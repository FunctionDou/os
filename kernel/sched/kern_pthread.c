/*************************************************************************
    > File Name: kernel/sched/kern_pthread.c
    > Author: Function_Dou
    > Mail: NOT
    > Created Time: 2018年11月01日 星期四 09时14分01秒
 ************************************************************************/

#include "sched.h"
#include "string.h"
#include "debug.h"
#include "kalloc.h"
#include "gdt.h"

pid_t now_pid = 0;

int32_t kernel_thread(int (*fn)(void *), void *arg)
{
	struct task_struct *new_task = (struct task_struct *)kmalloc(STACK_SIZE);
	assert(new_task != NULL, "kern_thread kmalloc error\n");

	bzero(new_task, sizeof(struct task_struct));

	new_task->state = TASK_WAIT;
	new_task->stack = current;
	new_task->pid = now_pid++;
	new_task->mm = NULL;

	uint32_t *stack_top = (uint32_t *)((uint32_t)new_task + STACK_SIZE);

	*(--stack_top) = (uint32_t)arg;
	*(--stack_top) = (uint32_t)kthread_exit;
	*(--stack_top) = (uint32_t)fn;

	new_task->context.esp = (uint32_t)new_task + STACK_SIZE - sizeof(uint32_t) * 3;

	// 设置新任务的标志寄存器未屏蔽中断
	new_task->context.eflags = 0x200;
	new_task->next = wait_proc_head;

	struct task_struct *tail = wait_proc_head;
	assert(tail != NULL, "Must init sched\n");
	while(tail->next != wait_proc_head)
	{
		tail = tail->next;
	}
	tail->next = new_task;

	return new_task->pid;
}

void kthread_exit()
{
	register uint32_t val asm("eax");

	printk("Thread exited with value %d\n", val);

	while(1)
		;
}

