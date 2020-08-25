
#ifndef __TASK_QUEUE_H
#define __TASK_QUEUE_H

#include <bits/types/time_t.h>
#include <pthread.h>

typedef struct task {
	void (*task)(void*);
	void* arg;
	time_t time;
	int interval;

	struct task* next;
} Task;

void add_task(Task* task);
Task* peek_task();
Task* pop_task();

#endif
