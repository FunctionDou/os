/*************************************************************************
    > File Name: include/sched.h
    > Author: Function_Dou
    > Mail: NOT
    > Created Time: 2018年11月01日 星期四 08时18分02秒
 ************************************************************************/

#ifndef INCLUDE_SCHED_H_
#define INCLUDE_SCHED_H_

#include "types.h"
#include "pmm.h"
#include "debug.h"

typedef enum task_state
{
	TASK_RUNNING = 0,		// 运行态
	TASK_WAIT = 1,			// 等待
	TASK_IDLE = 2,			// 空闲
	TASK_ZOMBLE = 3,		// 僵尸
	TASK_SLEEP = 4,			// 睡眠
	TASK_UNRUN = 5,
}task_state;

struct context 
{
	uint32_t esp;
	uint32_t ebp;
	uint32_t ebx;
	uint32_t esi;
	uint32_t edi;
	uint32_t eflags;
};

struct mm_struct
{
	pgd_t *pgd_ptr;
};

struct task_struct
{
	volatile task_state state;		// 进程(线程)的状态
	pid_t pid;						// PID
	void *stack;					// 栈地址
	struct mm_struct *mm;			// 页目录地址
	struct context context;			// 上下文切换保存寄存器值的
	struct task__struct *next;		// 进程链表
};

extern pid_t now_pid;

int32_t kernel_thread(int (*fn)(void *), void *);	// 内核线程
void kthread_exit();								// 线程清理函数

void init_sched();		// 初始化任务调度
void schedule();		// 任务调度
// 进行调度
void switch_to(struct context *prev, struct context *next);

extern struct task_struct * current;				// 当前运行
extern struct task_struct * sched_proc_head;		// 可调度队列
extern struct task_struct * wait_proc_head;			// 等待队列
extern struct task_struct * sleep_proc_head;		// 睡眠队列

extern uint32_t kern_stack_top;

#endif
