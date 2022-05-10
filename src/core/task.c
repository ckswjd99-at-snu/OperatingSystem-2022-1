/********************************************************
 * Filename: core/task.c
 * 
 * Author: parkjy, RTOSLab. SNU.
 * 
 * Description: task management.
 ********************************************************/
#include <core/eos.h>

#define READY		1
#define RUNNING		2
#define WAITING		3

/*
 * Queue (list) of tasks that are ready to run.
 */
static _os_node_t *_os_ready_queue[LOWEST_PRIORITY + 1];

/*
 * Pointer to TCB of running task
 */
static eos_tcb_t *_os_current_task;

int32u_t eos_create_task(eos_tcb_t *task, addr_t sblock_start, size_t sblock_size, void (*entry)(void *arg), void *arg, int32u_t priority) {
	// create context inside stack
	addr_t stack_pointer = _os_create_context(sblock_start, sblock_size, entry, arg);

	// fill tcb
	task->stack_pointer = stack_pointer;
	task->stack_base = sblock_start;
	task->stack_size = sblock_size;
	task->entry = entry;
	task->arg = arg;

	// make queueing_node
	_os_node_t* queueing_node = (_os_node_t*)malloc(sizeof(_os_node_t));
  queueing_node->ptr_data = task;
	queueing_node->priority = priority;
	queueing_node->next = NULL;
	queueing_node->previous = NULL;

	task->queueing_node = queueing_node;

	// push in queue
	_os_add_node_tail(&(_os_ready_queue[priority]), task->queueing_node);

	return 0;
}

int32u_t eos_destroy_task(eos_tcb_t *task) {
}

void eos_schedule() {
	if (eos_get_current_task()) {
		addr_t saved_stack_ptr = _os_save_context();

		if (saved_stack_ptr == NULL) return;

		_os_current_task->stack_pointer = saved_stack_ptr;
		_os_add_node_tail(&_os_ready_queue[_os_current_task->queueing_node->priority], _os_current_task->queueing_node);
	}

	int32u_t priority = _os_get_highest_priority();
	_os_node_t* new_current_node = _os_ready_queue[priority];
	if (new_current_node) {
		_os_current_task = new_current_node->ptr_data;
		_os_remove_node(&_os_ready_queue[priority], new_current_node);
		_os_restore_context(_os_current_task->stack_pointer);
	}
}

eos_tcb_t *eos_get_current_task() {
	return _os_current_task;
}

void eos_change_priority(eos_tcb_t *task, int32u_t priority) {
}

int32u_t eos_get_priority(eos_tcb_t *task) {
}

void eos_set_period(eos_tcb_t *task, int32u_t period){
}

int32u_t eos_get_period(eos_tcb_t *task) {
}

int32u_t eos_suspend_task(eos_tcb_t *task) {
}

int32u_t eos_resume_task(eos_tcb_t *task) {
}

void eos_sleep(int32u_t tick) {
}

void _os_init_task() {
	PRINT("initializing task module.\n");

	/* init current_task */
	_os_current_task = NULL;

	/* init multi-level ready_queue */
	int32u_t i;
	for (i = 0; i < LOWEST_PRIORITY; i++) {
		_os_ready_queue[i] = NULL;
	}
}

void _os_wait(_os_node_t **wait_queue) {
}

void _os_wakeup_single(_os_node_t **wait_queue, int32u_t queue_type) {
}

void _os_wakeup_all(_os_node_t **wait_queue, int32u_t queue_type) {
}

void _os_wakeup_sleeping_task(void *arg) {
}
