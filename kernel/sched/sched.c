/*************************************************************************
    > File Name: kernel/sched/sched.c
    > Author: Function_Dou
    > Mail: NOT
    > Created Time: 2018年11月01日 星期四 08时49分46秒
 ************************************************************************/

#include "sched.h"

// 初始化每个链表
struct task_struct * current = NULL;
struct task_struct * sched_proc_head = NULL;
struct task_struct * wait_proc_head = NULL;
struct task_struct * sleep_proc_head = NULL;

extern uint32_t kern_stack_top;

void init_sched()
{
	// 内核栈中申请一个栈的大小, 8k
	current = (struct task_struct *)(kern_stack_top - STACK_SIZE);
	// 进程的初始化
	current->state = TASK_WAIT;	
	current->pid = now_pid++;
	current->mm = NULL;			// 内核线程不需要指向页目录空间, 内核线程只需要使用上一个进程的少部分页
	current->next = current;
	current->stack = current;

	wait_proc_head = current;
}

void schedule()
{
	if(current)
	{
		if(current != current->next)
		{
			struct task_struct *prev = current;
			current = current->next;
			switch_to(&(prev->context), &(current->context));
		}
	}
}

