There are two types of scheduling algorithm: Cooperative Scheduling and Preemptive Scheduling

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

The UART interrupt priority must be set equal or lower than configMAX_SYSCALL_INTERRUPT_PRIORITY because we will be using FreeRTOS APIs inside of it.
