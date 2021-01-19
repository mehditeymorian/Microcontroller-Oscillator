#include "UsartUnit.h"

// USART2 -> AF07
// PA9  -> USART1_TX
// PA10 -> USART1_RX


void usart_init(void) {

	RCC->APB2ENR |= RCC_APB2ENR_USART1EN; // enable usart1 clock
	
	// TX 9
	GPIOA->MODER &= ~GPIO_MODER_MODE9_Msk; // reset
	GPIOA->MODER |= GPIO_MODER_MODE9_1; // pinA9 AF
	GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR9; //reset
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR9_1; // very fast speed
	GPIOA->AFR[1] &= ~GPIO_AFRH_AFRH1;
	GPIOA->AFR[1] |= 0x7 << 4; // pinA9 AF to AF7
	
	// RX 10
	GPIOA->MODER &= ~GPIO_MODER_MODE10_Msk; // reset
	GPIOA->MODER |= GPIO_MODER_MODE10_1; // pinA10 AF
	GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR10; //reset
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR10_1; // very fast speed
	GPIOA->AFR[1] &= ~GPIO_AFRH_AFRH2;
	GPIOA->AFR[1] |= 0x7 << 8; // pinA10 AF to AF7

	// interupt
	NVIC_SetPriority(USART1_IRQn, 0);
	NVIC_ClearPendingIRQ(USART1_IRQn);
	NVIC_EnableIRQ(USART1_IRQn);
	USART1->CR1 |= USART_CR1_RXNEIE; // enable interrupt for rxne
	
	USART1->CR1 |= USART_CR1_UE; // enable usart
	USART1->BRR |= 104 << USART_BRR_DIV_Mantissa_Pos; // set baud rate 84Mhz 50,000baud 
	USART1->BRR |= 16 << USART_BRR_DIV_Fraction_Pos; // 
	USART1->CR1 |= USART_CR1_TE | USART_CR1_RE; // transmission enable, receive enable
}


void usart_write(char* data) {
	int i = 0;
	while(1){
		char c = data[i++];
		if(c == '\0') break;
		USART1->DR = c;
		while(!READ_BIT(USART1->SR,USART_SR_TC)) {}
	}
}

uint16_t usart_read(void) {
	while(!READ_BIT(USART1->SR, USART_SR_RXNE)) {}
	return (uint16_t)(USART1->DR & (uint16_t)0x01FF);
}


