#include "taskqueue.h"
#include <pthread.h>
#include <stdlib.h>

static Task* volatile root_node = NULL;

void add_task(Task *task) {
	if (!root_node) {
		// When root is null
		task->next = NULL;
		root_node = task;
	} else if (root_node->time > task->time) {
		// When new task is added to top of queue
		task->next = root_node;
		root_node = task;
	} else {
		Task* temp = root_node;

		while (temp->next && temp->next->time < task->time) {
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

