/********************************************************
 * Filename: core/timer.c
 *
 * Author: wsyoo, RTOSLab. SNU.
 * 
 * Description: 
 ********************************************************/
#include <core/eos.h>

static eos_counter_t system_timer;

int8u_t eos_init_counter(eos_counter_t *counter, int32u_t init_value) {
	counter->tick = init_value;
	counter->alarm_queue = NULL;
	return 0;
}

void eos_set_alarm(eos_counter_t* counter, eos_alarm_t* alarm, int32u_t timeout, void (*entry)(void *arg), void *arg) {
	_os_remove_node(&(counter->alarm_queue), &(alarm->alarm_queue_node));
	if(timeout == 0 || entry == NULL) return;

	alarm->timeout = timeout;
	alarm->handler = entry;
	alarm->arg = arg;
	alarm->alarm_queue_node.ptr_data = alarm;
	alarm->alarm_queue_node.priority = timeout;

	_os_add_node_priority(&(counter->alarm_queue), &(alarm->alarm_queue_node));
}

eos_counter_t* eos_get_system_timer() {
	return &system_timer;
}

void eos_trigger_counter(eos_counter_t* counter) {
	PRINT("tick\n");
	counter->tick += 1;
	while (counter->alarm_queue != NULL) {
		eos_alarm_t* alarm_head = counter->alarm_queue->ptr_data;
		if (counter->tick == alarm_head->timeout) {
			eos_set_alarm(counter, alarm_head, 0, NULL, NULL);
			(alarm_head->handler)(alarm_head->arg);
		}
		else {
			break;
		}
	}
	eos_schedule();
}

/* Timer interrupt handler */
static void timer_interrupt_handler(int8s_t irqnum, void *arg) {
	/* trigger alarms */
	eos_trigger_counter(&system_timer);
}

void _os_init_timer() {
	eos_init_counter(&system_timer, 0);

	/* register timer interrupt handler */
	eos_set_interrupt_handler(IRQ_INTERVAL_TIMER0, timer_interrupt_handler, NULL);
}
