#include <core/eos.h>
#define STACK_SIZE 8096

int8u_t stack1[STACK_SIZE]; // stack for task1
int8u_t stack2[STACK_SIZE]; // stack for task2
int8u_t stack3[STACK_SIZE]; // stack for task3

eos_tcb_t tcb1; // tcb for task1
eos_tcb_t tcb2; // tcb for task2
eos_tcb_t tcb3; // tcb for task3

void task1() {
	while(1) { PRINT ("A\n"); eos_sleep(0); } // ‘A’ 출력 후 다음 주기까지 기다림
}

void task2() {
	while(1) { PRINT("B\n"); eos_sleep(0); }// ‘B’ 출력 후 다음 주기까지 기다림
}

void task3() {
	while(1) { PRINT("C\n"); eos_sleep(0); }// ‘C’ 출력 후 다음 주기까지 기다림
}

void eos_user_main() {
	eos_create_task(&tcb1, stack1, STACK_SIZE, task1, NULL, 1); // 태스크 1 생성
	eos_set_period(&tcb1, 2);

	eos_create_task(&tcb2, stack2, STACK_SIZE, task2, NULL, 10); // 태스크 2 생성
	eos_set_period(&tcb2, 4);

	eos_create_task(&tcb3, stack3, STACK_SIZE, task3, NULL, 50); // 태스크 3 생성
	eos_set_period(&tcb3, 8);
}
