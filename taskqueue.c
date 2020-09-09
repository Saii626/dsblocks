#include "taskqueue.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdatomic.h>

#define TASKS_MAX_NUMBER 10

static Task task_list[TASKS_MAX_NUMBER];
static Task* root_node = NULL;
static atomic_int next_free_node = 0;


Task* get_new_task() {
	int index = atomic_fetch_add_explicit(&next_free_node, 1, memory_order_relaxed);
	if (index >= TASKS_MAX_NUMBER) return NULL;
	return &task_list[index];
}

void add_task(Task *task) {
	if (!root_node) {
		// When root is null
		task->next = NULL;
		root_node = task;
	} else if (timespec_cmp(&root_node->time, &task->time)) {
		// When new task is added to top of queue
		task->next = root_node;
		root_node = task;
	} else {
		Task* temp = root_node;

		while (temp->next && timespec_cmp(&task->time, &temp->next->time)) {
			temp = temp->next;
		}

		task->next = temp->next;
		temp->next = task;
	}
}

Task* peek_task() {
	return root_node;
}

Task* pop_task() {
	Task* node = root_node;
	if (node) root_node = root_node->next;
	return node;
}
