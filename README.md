# SNUOS 2022-1

Project in **SNU ECE Introduction to OS (430.318)** lecture, at 2022 spring.

Based on **eOS(Educational Operating System)** skeleton code, I implemented some functionalities introduced below.

All the codes are written in **C**, partially **Inline Assembly**.

## Environments
* Ubuntu 16.04.6 LTS 32bit

## Build
```
$ git clone https://github.com/ckswjd99/SNUOS-2022-1
$ cd SNUOS-2022-1
$ cd src
$ make prepare
$ make all
$ ./eos
```

## Usage
* Put user codes inside `src/user/work.c`, each tasks as a function.
* You must provide stack and TCB for each tasks.
* Create task and manage scheduling inside function `eos_user_main`.
* User can use every OS functions with prefix `eos_`.
* Functions with prefix `_os_` is only for OS operations, **not for users**.

[Example 1](), [Example 2](), [Example 3]()

## Functionalities
### Interrupt
* `eos_disable_interrupt`: Disable whole interrupts.
* `eos_enable_interrupt`: Enable whole interrupts.

[More info]() about mechanism

### Context Switch
Inside `src/hal/linux/context.c`,
* `_os_create_context`: Create a context.
* `_os_save_context`, `_os_restore_context`: Save and restore a context.

Inside `src/core/task.c`,
* `eos_create_task`: Create task.
* `eos_schedule`: Yeild CPU and schedule another task.

[More info]() about implementation

### Periodic Scheduling
Inside `src/core/task.c`,
* `eos_set_period`: Set period of a task to wake up and execute.
* `eos_sleep`: For an amount of time, put task into waiting queue. When time's up, timer raises interrupt and put task into ready queue.
* `_os_wakeup_sleeping_task`: Put task which is inside waiting queue, into ready queue.

Inside `src/core/timer.c`,
* `eos_set_alarm`: When time's up, run handler callback function.
* `eos_trigger_counter`: Increase counter value of counter struct, then check alarms if it's expired.

[More info]() about implementation

### Semaphore
Inside `src/core/sync.c`,
* `eos_init_semaphore`: Initialize semaphore.
* `eos_acquire_semaphore`: Try to acquire a semaphore. Set its waiting type according to `timeout` parameter.
* `eos_release_semaphore`: Release a semaphore.

[More info]() about implementation

### Message Queue
Inside `src/core/comm.c`,
* `eos_init_mqueue`: Initialize message queue.
* `eos_send_message`: Send a message to the message queue.
* `eos_receive_message`: Receive a message from the message queue.

[More info]() about implementation