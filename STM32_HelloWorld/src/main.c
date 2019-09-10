/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/
#include<stdio.h>
#include<stdint.h>
#include<string.h>
#include "stm32f4xx.h"
#include "FreeRTOS.h"  //this is the first library you must include !
#include "task.h"

#define AVAILABLE 1
#define NON_AVAILABLE 0
//some macros
uint8_t UART_ACCESS_KEY = AVAILABLE;

TaskHandle_t xTaskHandle1=NULL;
TaskHandle_t xTaskHandle2=NULL;
void vTask1_handler(void *pvParameter);
void vTask2_handler(void *pvParameter);



static void prvSetupHardware(void);
static void prvSetupUart(void);

void printmsg(char *msg);
char usr_msg[250] = "This is a hello world application starting \r\n";



int main(void)
{

	DWT->CTRL |= 0x1;	//enable cycle counting for SEGGER

	//1. Reset the RCC clock configuration to the default reset state. The projects set the system clock to 180Mhz by default but we don't want that.
	//HSI ON, PLL OFF, HSE OFF, system clock = 16Mhz, cpu_clock = 16Mhz
	RCC_DeInit();

	//2.Update the SystemCoreClock variable
	SystemCoreClockUpdate();

	prvSetupHardware();
	//3. let's create 2 tasks:
	//usr_msg = "This is a hello world application starting \r\n";
	printmsg(usr_msg);

	//Start recording
	SEGGER_SYSVIEW_Conf();
	SEGGER_SYSVIEW_Start();

	xTaskCreate(vTask1_handler,"Task-1",configMINIMAL_STACK_SIZE,NULL,2,&xTaskHandle1);
	xTaskCreate(vTask2_handler,"Task-2",configMINIMAL_STACK_SIZE,NULL,2,&xTaskHandle2);

	vTaskStartScheduler();
	for(;;);
}

void vTask1_handler(void *pvParameter)
{
	while(1)
	{
		if(UART_ACCESS_KEY == AVAILABLE)
		{
			UART_ACCESS_KEY = NON_AVAILABLE;
			printmsg("Hello-world:From Task-1 \r\n");
			UART_ACCESS_KEY = AVAILABLE;
			SEGGER_SYSVIEW_Print("Task1 is yielding");
			traceISR_EXIT_TO_SCHEDULER();
			taskYIELD();
		}

	}
}

void vTask2_handler(void *pvParameter)
{
	while(1)
	{
		if(UART_ACCESS_KEY == AVAILABLE)
		{
			UART_ACCESS_KEY = NON_AVAILABLE;
			printmsg("Hello-world:From Task-2 \r\n");
			UART_ACCESS_KEY = AVAILABLE;
			SEGGER_SYSVIEW_Print("Task2 is yielding");
			traceISR_EXIT_TO_SCHEDULER();
			taskYIELD();
		}
	}

}

static void prvSetupUart(void)
{
	GPIO_InitTypeDef gpio_uart_pins;
	USART_InitTypeDef uart2_init;
	//1. Enable the UART2 and GPIOA Peripheral Clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);

	//PA2 is UART2_TX, PA3 is UART2_RX

	//2. Alternate function configuration of MCU pins to behave as UART2 TX and RX
	memset(&gpio_uart_pins,0,sizeof(gpio_uart_pins)); //since gpio_uart_pins is a local structure, its value can contain garbage values so it's better to set all its elements to 0
	memset(&uart2_init,0,sizeof(gpio_uart_pins));

	gpio_uart_pins.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	gpio_uart_pins.GPIO_Mode = GPIO_Mode_AF;
	gpio_uart_pins.GPIO_OType = GPIO_OType_PP;
	gpio_uart_pins.GPIO_PuPd = GPIO_PuPd_UP;
	gpio_uart_pins.GPIO_Speed = GPIO_Fast_Speed;
	GPIO_Init(GPIOA,&gpio_uart_pins);

	//3. AF mode settings for the pins
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //PA2(tx) with alternate function 7
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);	//PA3(rx) ,AF=7


	//4. UART parameter initializations
	uart2_init.USART_BaudRate = 115200;
	uart2_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //we don't use hardware flow control
	uart2_init.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	uart2_init.USART_Parity = USART_Parity_No;
	uart2_init.USART_StopBits = USART_StopBits_1;
	uart2_init.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2,&uart2_init);

	//5. enable the UART2 peripheral
	USART_Cmd(USART2,ENABLE);
}

static void prvSetupHardware(void)
{
	prvSetupUart();
}

void printmsg(char *msg)
{
	while(*msg != '\0')
	{
		while( !USART_GetFlagStatus(USART2,USART_FLAG_TXE) );
		USART_SendData(USART2, *(msg++) );
	}

}
