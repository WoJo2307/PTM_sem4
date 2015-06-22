#include <stdio.h>
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_exti.h"
#include "misc.h"

int a=0;

void TIM2_IRQHandler(void) {
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
		//GPIO_ToggleBits(GPIOA, GPIO_Pin_2);

		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
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
void TM_TIMER_Init(void) {
	TIM_TimeBaseInitTypeDef TIM_BaseStruct;

	/* Enable clock for TIM4 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	//Time base configuration
		TIM_BaseStruct.TIM_Period = 4200;
		TIM_BaseStruct.TIM_Prescaler = 100;
		TIM_BaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_BaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
	/* Initialize TIM4 */
    TIM_TimeBaseInit(TIM2, &TIM_BaseStruct);
	/* Start count on TIM4 */
    TIM_Cmd(TIM2, ENABLE);
}
void TM_PWM_Init(void) {
   NVIC_InitTypeDef NVIC_InitStructure;
   	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
   	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
   	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
   	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   	NVIC_Init(&NVIC_InitStructure);
   	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
   	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

   	TIM_OCInitTypeDef TIM_OCInitStructure;
   	//PWM1 Mode configuration:
   	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
   	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
   	TIM_OCInitStructure.TIM_Pulse = 0;
   	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM2);

   	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
   	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
   /* Common settings */

   /* PWM mode 2 = Clear on compare match */
   /* PWM mode 1 = Set on compare match */
//   TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM2;
//   TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
//   TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
//   TIM_OCStruct.TIM_Pulse = 10; /* 25% duty cycle */
//   TIM_OC1Init(TIM2, &TIM_OCStruct);
//   TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);

//   TIM_OCStruct.TIM_Pulse = 4199; /* 50% duty cycle */
//   TIM_OC2Init(TIM2, &TIM_OCStruct);
//   TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);

//   TIM_OCStruct.TIM_Pulse = 6299; /* 75% duty cycle */
//   TIM_OC3Init(TIM2, &TIM_OCStruct);
//   TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
//
//   TIM_OCStruct.TIM_Pulse = 8399; /* 100% duty cycle */
//   TIM_OC4Init(TIM2, &TIM_OCStruct);
//   TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
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
	// czekaj na odebranie danych
	while (USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET) {}
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
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
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
	//init_pin();
	//TM_TIMER_Init();
	//TM_PWM_Init();

//		counter = TIM2->CNT;
		//printf("%d\n",counter);
//		if(TIM_GetFlagStatus(TIM2, TIM_FLAG_Update)) {
//					if(a==0){
//						GPIO_SetBits(GPIOA, GPIO_Pin_2);
//						a=1;
//					}
//					else {
//						GPIO_ResetBits(GPIOA, GPIO_Pin_2);
//						a=0;
//					}
//			    		TIM_ClearFlag(TIM2, TIM_FLAG_Update);
//			    	}
//	}


		/*float cos[6] = { 0, 0.2, 0.4,0.6,0.8,1.0 };
			int i = 0;
			int b = 0;

		unsigned int counter = TIM2->CNT;
		while(1){
			counter = TIM2->CNT;
			printf("%d\n",counter);
			printf("%d\n",b);
			printf("%d\n",i);
			if (TIM2->CNT >= 2100) {
						i++;
					}
					if(i>b)
					{	TIM2->CCR1 = 4200 * cos[b];
						b++;
						if (b == 5)
							b = 0;
						i = 0;
					}*/
	printf("Hello World!\r\n");
	int a=0;
	char b=0;
	while (1) {
		static int count = 0;
		static int i;

		for (i = 0; i < 10000000; ++i)
			;
		//GPIO_ToggleBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
		printf("%d\r\n", ++count);
		GPIO_SetBits(GPIOA, GPIO_Pin_4 | GPIO_Pin_6|GPIO_Pin_5 | GPIO_Pin_7|GPIO_Pin_2|GPIO_Pin_3);
		do{
		 b=usartGetChar();
		 printf("%d\n", b);
		 if(b==1){
			 przod();
		 }
		 if(b==2)
			 prawo();
			 //GPIO_ToggleBits(GPIOD, GPIO_Pin_13);
		 if(b==3)
			 tyl();
			 //GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
		 if(b==4)
		 			 lewo();
		 if(b==5)
		 			 stop();
		}while(USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET);

	}
}
