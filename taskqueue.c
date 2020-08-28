#include "taskqueue.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdatomic.h>

#define MAX_TASK_LENGTH 10
#define NULL_INDEX -1

static Task task_list[MAX_TASK_LENGTH];
static atomic_int next_free_pos = 0;
static atomic_int root_pos = NULL_INDEX;

static int null_test = NULL_INDEX;

Task* get_new_task() {
	int alloted_slot = atomic_fetch_add(&next_free_pos, 1);
	if (alloted_slot == MAX_TASK_LENGTH) return NULL;
	task_list[alloted_slot].next_index = NULL_INDEX;
	return &task_list[alloted_slot];
}

void add_task(Task *task) {
	int task_index = task - task_list;
	
	// When root is null
	if (atomic_compare_exchange_strong(&root_pos, &null_test, task_index)) return;

	// When new task is added to top of queue
	int temp = atomic_load(&root_pos);
	while (task_list[temp].time > task->time) {
		task->next_index = temp;
		if(atomic_compare_exchange_strong(&root_pos, &temp, task_index)) return;
		temp = atomic_load(&root_pos);
	}

	// When new task will be added in the middle. We can get away with this as
	// the tasks are never removed from middle, always from the begining
	while ((temp = task_list[temp].next_index) != NULL_INDEX && task_list[temp].time < task->time) {}

	task->next = temp->next;
	temp->next = task;
}

Task* peek_task() {
	return root_node;
}

Task* pop_task() {
	Task* node = root_node;
	if (node) root_node = root_node->next;
	return node;
}

