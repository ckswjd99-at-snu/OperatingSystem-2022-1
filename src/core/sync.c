/********************************************************
 * Filename: core/sync.c
 * 
 * Author: wsyoo, RTOSLab. SNU.
 * 
 * Description: semaphore, condition variable management.
 ********************************************************/
#include <core/eos.h>

#ifndef TCB_STATE
#define TCB_STATE
#define READY		1
#define RUNNING		2
#define WAITING		3
#endif

void eos_init_semaphore(eos_semaphore_t *sem, int32u_t initial_count, int8u_t queue_type) {
	PRINT("sem: %p\n", sem);
	/* initialization */
	sem->count = initial_count;
	sem->wait_queue = NULL;
	sem->queue_type = queue_type;
}

int32u_t eos_acquire_semaphore(eos_semaphore_t *sem, int32s_t timeout) {
	PRINT("requested semaphore: %p, timeout: %d\n", sem, timeout);
	eos_disable_interrupt();
	
	if (sem->count > 0) {	// acquire success case
		sem->count--;
		eos_enable_interrupt();
		return 1;
	}
	eos_enable_interrupt();

	if (timeout == -1) return 0;
	else if (timeout == 0) {
		eos_tcb_t* current_task = eos_get_current_task();
		current_task->state = WAITING;

		// push current task to the wait queue of the semaphore
			if (sem->queue_type == 0) {
				_os_add_node_tail(&(sem->wait_queue), current_task->queueing_node);
			}
			else {
				_os_add_node_priority(&(sem->wait_queue), current_task->queueing_node);
			}

		while(1) {
			PRINT("now set alarm and wait\n");

			eos_schedule();

			PRINT("return from schedule\n");

			// when some task returned semaphore and called me
			// check semaphore and return

			eos_disable_interrupt();
			if(sem->count > 0) {
				sem->count--;
				_os_remove_node(&(sem->wait_queue), current_task->queueing_node);
				eos_enable_interrupt();
				return 1;
			}
			eos_enable_interrupt();
		}
	}
	else if (timeout > 0) {
		eos_tcb_t* current_task = eos_get_current_task();
		current_task->state = WAITING;

		while(1) {
			// push current task to the wait queue of the semaphore
			if (sem->queue_type == 0) {
				_os_add_node_tail(&(sem->wait_queue), current_task->queueing_node);
			}
			else {
				_os_add_node_priority(&(sem->wait_queue), current_task->queueing_node);
			}

			int32u_t timeout_count = timeout;
			eos_counter_t* sys_timer = eos_get_system_timer();
			eos_alarm_t* alarm = (eos_alarm_t*)malloc(sizeof(eos_alarm_t));
			eos_set_alarm(sys_timer, alarm, sys_timer->tick+1, _os_wakeup_sleeping_task, current_task);

			eos_schedule();

			// when some task returned semaphore and called me
			// check semaphore and return
			_os_remove_node(&(sem->wait_queue), current_task->queueing_node);

			eos_disable_interrupt();
			if(sem->count > 0) {
				sem->count--;
				eos_enable_interrupt();
				return 1;
			}
			
			if (--timeout == 0) {
				eos_enable_interrupt();
				return 0;
			}
			eos_enable_interrupt();
		}
	}


	return 0;
}

void eos_release_semaphore(eos_semaphore_t *sem) {
	eos_disable_interrupt();
	sem->count++;
	if (sem->wait_queue == NULL) {
		eos_enable_interrupt();
		return;
	}
	eos_enable_interrupt();

	eos_tcb_t* wake_up_task = (eos_tcb_t*)(sem->wait_queue->ptr_data);
	_os_remove_node(&sem->wait_queue, wake_up_task->queueing_node);
	_os_wakeup_sleeping_task(wake_up_task);
}

void eos_init_condition(eos_condition_t *cond, int32u_t queue_type) {
	/* initialization */
	cond->wait_queue = NULL;
	cond->queue_type = queue_type;
}

void eos_wait_condition(eos_condition_t *cond, eos_semaphore_t *mutex) {
	/* release acquired semaphore */
	eos_release_semaphore(mutex);
	/* wait on condition's wait_queue */
	_os_wait(&cond->wait_queue);
	/* acquire semaphore before return */
	eos_acquire_semaphore(mutex, 0);
}

void eos_notify_condition(eos_condition_t *cond) {
	/* select a task that is waiting on this wait_queue */
	_os_wakeup_single(&cond->wait_queue, cond->queue_type);
}
