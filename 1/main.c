#include <stdio.h>
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_exti.h"
#include "misc.h"

int a=0;
int i2=0;
int f=0;
int moc1=420;
int moc2=420;
int akt_V=0;

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
//		i++;
//		if(i>400000)
//		{
//			return 5;
//		}
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
		GPIO_SetBits(GPIOA, GPIO_Pin_4 | GPIO_Pin_7/* | GPIO_Pin_3| GPIO_Pin_2*/ );
		GPIO_ResetBits(GPIOA, GPIO_Pin_5 | GPIO_Pin_6);
		//wlaczone + 1. silnika i - 2. --oraz en'y obu silników
	}

	void tyl(){
		GPIO_SetBits(GPIOA, GPIO_Pin_5 | GPIO_Pin_6 );
		GPIO_ResetBits(GPIOA, GPIO_Pin_4 | GPIO_Pin_7);
		//wlaczone - 1. silnika i + 2. --oraz en'y obu silników
	}

	void stop(){
		GPIO_ResetBits(GPIOA, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
		//wszystko wylaczone
	}
	void InitializeTimer()
	{
	    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	    TIM_TimeBaseInitTypeDef timerInitStructure;

	    timerInitStructure.TIM_Prescaler = 200;
	    timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	    timerInitStructure.TIM_Period = 4200;
	    timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	    timerInitStructure.TIM_RepetitionCounter = 0;
	    TIM_TimeBaseInit(TIM2, &timerInitStructure);
	    TIM_Cmd(TIM2, ENABLE);
	}
	void InitializePWMChannel()
	{
	    TIM_OCInitTypeDef outputChannelInit;
	    outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
	    outputChannelInit.TIM_Pulse = 0;
	    outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
	    outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;

	    TIM_OC4Init(TIM2, &outputChannelInit);
	    TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);

	}
	void InitializePWMChannel2()
	{
	    TIM_OCInitTypeDef outputChannelInit = {0,};
	    outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
	    outputChannelInit.TIM_Pulse = 0;
	    outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
	    outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;

	    TIM_OC3Init(TIM2, &outputChannelInit);
	    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);


	}
	void InitializeLEDs()
	{
	    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM2);
	    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM2);

	    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3| GPIO_Pin_2;
	    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	    GPIO_Init(GPIOA, &GPIO_InitStructure);
	}
	void inir(){
			InitializeTimer();
			InitializePWMChannel();
			InitializePWMChannel2();
			InitializeLEDs();

	}
	void f_Z(int b){
		if(b==11){
			moc1=0;
			f_V(akt_V);
		}
		else if(b==12){
			moc1=84;
			f_V(akt_V);
		}
		else if(b==13){
			moc1=168;
			f_V(akt_V);
		}
		else if(b==14){
			moc1=252;
			f_V(akt_V);
		}
		else if(b==15){
			moc1=336;
			f_V(akt_V);
		}
		else if(b==16){
			moc1=moc2=420;
			f_V(akt_V);
		}
		else if(b==17){
			moc2=336;
			f_V(akt_V);
		}
		else if(b==18){
			moc2=252;
			f_V(akt_V);
		}
		else if(b==19){
			moc2=168;
			f_V(akt_V);
		}
		else if(b==20){
			moc2=84;
			f_V(akt_V);
		}
		else if(b==21){
			moc2=0;
			f_V(akt_V);
		}
	}
	void f_V(int b){
		akt_V=b;
		if(b==10){
			TIM2->CCR4 = moc1*1;
			TIM2->CCR3 = moc2*1;
		}
		else if(b==22){
			TIM2->CCR4 = moc1*2;
			TIM2->CCR3 = moc2*2;
		}
		else if(b==30){
			TIM2->CCR4 = moc1*3;
			TIM2->CCR3 = moc2*3;
		}
		else if(b==40){
			TIM2->CCR4 = moc1*4;
			TIM2->CCR3 = moc2*4;
		}
		else if(b==50){
			TIM2->CCR4 = moc1*5;
			TIM2->CCR3 = moc2*5;
		}
		else if(b==60){
			TIM2->CCR4 = moc1*6;
			TIM2->CCR3 = moc2*6;
		}
		else if(b==70){
			TIM2->CCR4 = moc1*7;
			TIM2->CCR3 = moc2*7;
		}
		else if(b==80){
			TIM2->CCR4 = moc1*8;
			TIM2->CCR3 = moc2*8;
		}
		else if(b==90){
			TIM2->CCR4 = moc1*9;
			TIM2->CCR3 = moc2*9;
		}
		else if(b==100){
			TIM2->CCR4 = moc1*10;
			TIM2->CCR3 = moc2*10;
		}
	}
int main(void) {


	inir();
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
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 /*| GPIO_Pin_2 | GPIO_Pin_3*/;
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
//			TIM2->CCR4 = 4200;
//			TIM2->CCR3 = 4200;
		//for (i = 0; i < 10000000; ++i);
		//printf("%d\r\n", ++count);
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
		else if(b>10&&b<22){
			f_Z(b);
		}
		else{
			f_V(b);
		}



		}while(USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET);

	}
}
