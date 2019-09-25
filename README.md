# Note to remember:
Syncronization is nothing but aligning number of Tasks to achieve a desired behaviour. (Almost like pipe-lineing)

Where as mutual exclusion is avoiding a task to execute the critical section which is already owned by another task for execution.

Typically Semaphores are used to implement the synchronization between tasks and between tasks and interrupts. 

Mutex are the best choice to implement the mutual exclusion. That is protectcing access of a shared item. 

Sempahores also can be used to implement the mutual exclusion but it will introduce some serious design issues.

2 ways we can implement the mutual exclusion in FreeRTOS is:

1.Using Binary semaphore APIs
2.Using Mutex APIs

Priority inversion is a scenario in scheduling in which a high priority task is indirectly preempted by a lower priority task effectively inverting the relative priorities of the two tasks. This violates the priority model that high priority tasks can only be prevented from running by higher priority tasks and briefly by low priority tasks which will quickly complete their use of a resource shared by the high and low priority tasks.

Priorty inversion can happen if you use Binary semaphore for shared recourses ! That's the reason why Mutex is a better alternative to serialize the access to shared recourse. 
The biggest advantage of Mutex over binary semaphore is Priority Inheritance!

There are two types of semaphore: Binary and Counting Sempanhore.

Binary semaphore: If its value=1 then semaphore or key is available. If value=0 semaphore is not available.
Binary semaphore use cases: 1. Syncronization 2. Mutual Exclusion

Counting semaphore: its value can be initialized to any number so if value = 20, then it is analogous to having 20 semaphore keys.
Every time a task aquires a key, semaphore's value will be decremented by 1 until reaches 0.
Each time an event/interrupt occurs, the event/interrupt handler will "give" the sempahore, thus incrementing the semaphore count value.

There are two types of scheduling algorithm: Cooperative Scheduling and Preemptive Scheduling.

# FreeRTOS_NUCLEO_F446RE_Projects

STM32_HelloWorld uses cooperative scheduling which means that the System Handler doesn't trigger the PendSV Handler every 1ms. In cooperative scheduling the task manually should yield it's execution to another task. In this case we used taskYIELD Api to do it. We used SEGGER software to trace the signals and the operation behavior.

# STM32_FreeRTOS_Queue_Processing

In this project, we are using 4 tasks and 2 queues.  The first task in "menu_print_task" which takes care to print the "menu with commands" over UART to the console so that the user can take appropriate actions. But note that this task doesn't send the menu directly to the uart. It does it by sending the commands first to the QUEUE (uart_write_queue) and then the the second task which is "uart_write_task" will read the commands from the queue (uart_write_queue) in order to send that data to the UART peripheral.

Whenever user sends a command , the *uart_interrupt* will be triggered. The purpose of the interrupt is to collect the command from the user and it will save it. After this, the interrupt will actually notify (it will wake it up) the *cmd_handling_task* and this task will send the command to the second queue called *command_queue*. Finally the forth task called "cmd_processing_task" will read the data from the "command_queue" and it will take the appropiete actions based on the command code.

COMMANDS that will be used:
LED_ON,
LED_OFF,
LED_TOGGLE_START,
LED_TOGGLE_STOP,
LED_STATUS_READ,
RTC_DATETIME_READ

Notes:

The UART interrupt priority must be set equal or lower than configMAX_SYSCALL_INTERRUPT_PRIORITY because we will be using FreeRTOS APIs inside of it. Since configMAX_SYSCALL_INTERRUPT_PRIORITY is set to 5, we can use interrupt priority equal to 5 up to 15 but we can't use priority lower than 5. 
