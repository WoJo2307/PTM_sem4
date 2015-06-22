#include <stdio.h>
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_exti.h"
#include "misc.h"

int a=0;
int i2=0;

GPIO_InitTypeDef GPIO_InitStructure;
void intic(){
	// wlaczenie taktowania wybranego portu
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	// wlaczenie taktowania wybranego uk³adu USART
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	// konfiguracja linii Tx
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART3);

	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	// konfiguracja linii Rx
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART3);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	USART_InitTypeDef USART_InitStructure;
	// predkosc transmisji (mozliwe standardowe opcje: 9600, 19200, 38400,57600, 115200, ...)
	USART_InitStructure.USART_BaudRate = 9600;
	// d³ugoœæ s³owa (USART_WordLength_8b lub USART_WordLength_9b)
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// liczba bitów stopu (USART_StopBits_1, USART_StopBits_0_5,USART_StopBits_2, USART_StopBits_1_5)
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// sprawdzanie parzystoœci (USART_Parity_No, USART_Parity_Even,	USART_Parity_Odd)
	USART_InitStructure.USART_Parity = USART_Parity_No;
	// sprzêtowa kontrola przep³ywu (USART_HardwareFlowControl_None,USART_HardwareFlowControl_RTS, USART_HardwareFlowControl_CTS,USART_HardwareFlowControl_RTS_CTS)
	USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None;
	// tryb nadawania/odbierania (USART_Mode_Rx, USART_Mode_Rx )
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// konfiguracja
	USART_Init(USART3, &USART_InitStructure);

	// wlaczenie ukladu USART
	USART_Cmd(USART3, ENABLE);
	}
//////////////////////////////////Wys³anie/odebranie danych za pomoc¹ interfejsu USART////////////////////////////////////////


	///////////////Wys³anie danych przez port szeregowy odbywa sie nastêpuj¹co:
	// wyslanie danych
	//USART_SendData(USART3, 'A');
	// czekaj az dane zostana wyslane
	//while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET) {}

	/////////////odbieranie danych
	uint8_t usartGetChar(void)
	{
		int i=0;
	// czekaj na odebranie danych
	while (USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET) {
		i++;
		if(i>400000)
		{
			return 5;
		}
	}
	return USART_ReceiveData(USART3);

	}
	void init_pin(){
		GPIO_SetBits(GPIOA, GPIO_Pin_4 | GPIO_Pin_6|GPIO_Pin_5 | GPIO_Pin_7);
	}
	void lewo(){
		GPIO_SetBits(GPIOA, GPIO_Pin_4 | GPIO_Pin_6| GPIO_Pin_2 | GPIO_Pin_3);
		GPIO_ResetBits(GPIOA, GPIO_Pin_5 | GPIO_Pin_7);
		//wlaczone + --i en'y obu silników
	}

	void prawo(){
		GPIO_SetBits(GPIOA, GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_2 | GPIO_Pin_3);
		GPIO_ResetBits(GPIOA, GPIO_Pin_4 | GPIO_Pin_6);
		//wlaczone - --i en'y obu silników
	}

	void przod(){
		GPIO_SetBits(GPIOA, GPIO_Pin_4 | GPIO_Pin_7 | GPIO_Pin_3| GPIO_Pin_2 );
		GPIO_ResetBits(GPIOA, GPIO_Pin_5 | GPIO_Pin_6);
		//wlaczone + 1. silnika i - 2. --oraz en'y obu silników
	}

	void tyl(){
		GPIO_SetBits(GPIOA, GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_2 | GPIO_Pin_3);
		GPIO_ResetBits(GPIOA, GPIO_Pin_4 | GPIO_Pin_7);
		//wlaczone - 1. silnika i + 2. --oraz en'y obu silników
	}

	void stop(){
		GPIO_ResetBits(GPIOA, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_7);
		//wszystko wylaczone
	}

	void speedA(){
		//pwm
	}

	void speedB(){
		//pwm
	}
int main(void) {
	intic();
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14 | GPIO_Pin_13
			| GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_2 | GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

	stop();
	int a=0;
	char b=0;
	while (1) {
		static int count = 0;
		static int i;

		//for (i = 0; i < 10000000; ++i);
		printf("%d\r\n", ++count);
		do{
		 b=usartGetChar();
		 printf("%d\n", b);
		 if(b==1){
			 przod();
			 GPIO_SetBits(GPIOD, GPIO_Pin_15);
			 GPIO_ResetBits(GPIOD, GPIO_Pin_14|GPIO_Pin_13|GPIO_Pin_12);
		 }
		 else if(b==2){
			 prawo();
			 GPIO_SetBits(GPIOD, GPIO_Pin_12);
			 GPIO_ResetBits(GPIOD, GPIO_Pin_14|GPIO_Pin_13|GPIO_Pin_15);
		 }
		else if(b==3){
			 tyl();
			 GPIO_SetBits(GPIOD, GPIO_Pin_13);
			 GPIO_ResetBits(GPIOD, GPIO_Pin_14|GPIO_Pin_12|GPIO_Pin_15);
		}
		else if(b==4){
		 			 lewo();
		 	 GPIO_SetBits(GPIOD, GPIO_Pin_14);
		 	 GPIO_ResetBits(GPIOD, GPIO_Pin_13|GPIO_Pin_12|GPIO_Pin_15);
		 }
		else if(b==5){
		 			 stop();
		 			GPIO_ResetBits(GPIOD, GPIO_Pin_14|GPIO_Pin_13|GPIO_Pin_12|GPIO_Pin_15);
		}
		}while(USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET);

	}
}
