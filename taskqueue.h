
#ifndef __TASK_QUEUE_H
#define __TASK_QUEUE_H

#include <bits/types/struct_timespec.h>
#include <time.h>
#include <sys/time.h>

#include "util.h"
#include "shared.h"

typedef struct task {
	void (*task)(void*);
	void* arg;
	time_spec_t time;
	int interval;
	debug(const char* task_name;)

	struct task* next;
} Task;

Task* get_new_task();
void add_task(Task* task);
Task* peek_task();
Task* pop_task();

#endif
