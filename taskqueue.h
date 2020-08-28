
#ifndef __TASK_QUEUE_H
#define __TASK_QUEUE_H

#include <bits/types/time_t.h>

typedef struct {
	void (*task)(void*);
	void* arg;
	time_t time;
	int interval;

	int next_index;	
} Task;

void add_task(Task* task);
Task* peek_task();
Task* pop_task();

#endif
