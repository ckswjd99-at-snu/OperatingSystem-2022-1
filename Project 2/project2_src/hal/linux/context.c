#include <core/eos.h>
#include <core/eos_internal.h>
#include "emulator_asm.h"

typedef struct _os_context {
	/* low address */
	int32u_t edi;
	int32u_t esi;
	int32u_t ebx;
	int32u_t edx;
	int32u_t ecx;
	int32u_t eax;
	int32u_t _eflags;
	int32u_t eip;
	/* high address */
} _os_context_t;

void print_context(addr_t context) {
	if(context == NULL) return;
	_os_context_t *ctx = (_os_context_t *)context;
	//PRINT("reg1  =0x%x\n", ctx->reg1);
	//PRINT("reg2  =0x%x\n", ctx->reg2);
	//PRINT("reg3  =0x%x\n", ctx->reg3);
	//...`
}

addr_t _os_create_context(addr_t stack_base, size_t stack_size, void (*entry)(void *), void *arg) {
	int32u_t* stack_pointer = (int32u_t*)((int8u_t*)stack_base + stack_size);  
	
	*(stack_pointer - 1) = (int32u_t) arg;
	*(stack_pointer - 2) = (int32u_t) NULL;
	*(stack_pointer - 3) = (int32u_t) entry;
	*(stack_pointer - 4) = (int32u_t) 1;

	*(stack_pointer - 5) = (int32u_t) NULL;
	*(stack_pointer - 6) = (int32u_t) NULL;
	*(stack_pointer - 7) = (int32u_t) NULL;
	*(stack_pointer - 8) = (int32u_t) NULL;
	*(stack_pointer - 9) = (int32u_t) NULL;
	*(stack_pointer - 10) = (int32u_t) NULL;
	return (addr_t)(stack_pointer - 10);
}

void _os_restore_context(addr_t sp) {
	__asm__ __volatile__ ("\
		movl %0, %%esp;\
		popl %%edi;\
		popl %%esi;\
		popl %%ebx;\
		popl %%edx;\
		popl %%ecx;\
		popl %%eax;\
		popl _eflags;\
		movl 4(%%esp), %%ebp;\
		ret;\
	"::"m" (sp));
}

addr_t _os_save_context() {
	// eax에 esp를 넣는 부분이, return 값을 넣어주는 부분이다. 
	__asm__ __volatile__ ("\
		pushl $resume_eip;\
		pushl _eflags;\
		pushl %%eax;\
		pushl %%ecx;\
		pushl %%edx;\
		pushl %%ebx;\
		pushl %%esi;\
		pushl %%edi;\
		movl %%esp, %%eax;\
		pushl 4(%%ebp);\
		pushl 0(%%ebp);\
		movl %%esp, %%ebp;\
		resume_eip: \
		leave;\
		ret;\
	"::);
}
