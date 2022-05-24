#include <core/eos.h>
#define STACK_SIZE 8096

int8u_t stack1[STACK_SIZE]; 
int8u_t stack2[STACK_SIZE]; 
int8u_t stack3[STACK_SIZE];
eos_tcb_t tcb1; 
eos_tcb_t tcb2; 
eos_tcb_t tcb3;

void task1() {
	while(1) { PRINT("A\n"); eos_sleep(0); }// ‘A’ 출력 후 다음 주기까지 기다림
}

void task2() {
	while(1) { PRINT("B\n"); eos_sleep(0); } // ‘B’ 출력 후 다음 주기까지 기다림
}

void task3() {
	while(1) { PRINT("C\n"); eos_sleep(0); }// ‘C’ 출력 후 다음 주기까지 기다림
}

void eos_user_main() {
	eos_create_task(&tcb1, stack1, STACK_SIZE, task1, NULL, 1);
	eos_set_period(&tcb1, 2);

	eos_create_task(&tcb2, stack2, STACK_SIZE, task2, NULL, 10);
	eos_set_period(&tcb2, 4);

	eos_create_task(&tcb3, stack3, STACK_SIZE, task3, NULL, 50);
	eos_set_period(&tcb3, 8);
}
