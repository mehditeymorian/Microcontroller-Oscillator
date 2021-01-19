#include "stm32f4xx.h"                  // Device header
#include "stdint.h"
#include "Wave.h"
#include "Input.h"
#include "UsartUnit.h"


void pwm_clock_init(void){
	

	// pwr clock
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	PWR->CR &= ~PWR_CR_VOS_Msk;
	PWR->CR |= PWR_CR_VOS_1; // scale mode 2
	
	
	// flash
	FLASH->ACR &= ~FLASH_ACR_LATENCY;
	FLASH->ACR |= FLASH_ACR_LATENCY_2WS;
	
	// HSI CONFIGURATION
	RCC->CR |= RCC_CR_HSION;
	while( !(RCC->CR & RCC_CR_HSIRDY) ){}
	
	
	// PLL CONFIGURATION
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLSRC; // PLL SRC= HSI
	
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLM_Msk;
	RCC->PLLCFGR |= 16 << RCC_PLLCFGR_PLLM_Pos;
	
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLN_Msk;
	RCC->PLLCFGR |= 336 << RCC_PLLCFGR_PLLN_Pos;
	
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLP_Msk;
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLP_0; // div4
	
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLQ_Msk;
	RCC->PLLCFGR |= 4 << RCC_PLLCFGR_PLLQ_Pos;
	
	RCC->CR |= RCC_CR_PLLON;
	while( !(RCC->CR & RCC_CR_PLLRDY) ){}

		
		
	// flash
	FLASH->ACR &= ~FLASH_ACR_LATENCY;
	FLASH->ACR |= FLASH_ACR_LATENCY_2WS;
	
	RCC->CFGR &= ~RCC_CFGR_HPRE; // AHB DIV 1
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
	
	RCC->CFGR &= ~RCC_CFGR_PPRE1_Msk; // APB1 DIV 2
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;
	
	RCC->CFGR &= ~RCC_CFGR_PPRE2_Msk; // APB2 DIV 1
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;
	
	// CPU, AHB, APB buses clocks
	RCC->CFGR &= ~RCC_CFGR_SW_Msk;// PLL CLK SRC
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while( !(RCC->CFGR & RCC_CFGR_SWS_PLL) ){}

	SystemCoreClockUpdate();
}


void pwm_init(void){
	// TIM5 CH3 PA2

	GPIOA->MODER &= ~GPIO_MODER_MODE2_Msk;
	GPIOA->MODER |= GPIO_MODER_MODE2_1;
	GPIOA->AFR[0] |= GPIO_AFRL_AFRL2_1;
	
	
	RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
	TIM5->PSC = 1024-1; // prescaler
	TIM5->ARR = RES-1; // whole cycle
	TIM5->CNT = 0;
	TIM5->CCER |= TIM_CCER_CC3E;
	TIM5->CCMR2 |= 6 << TIM_CCMR2_OC3M_Pos;
	TIM5->CCR3 = 512; // duty cycle
	
	
	
	
}

void tim2_init(void) {

	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	TIM2->CR1 |= TIM_CR1_CKD_1;
	TIM2->PSC = 1024-1;
	TIM2->ARR = RES - 1;
	TIM2->CNT = 0;
	
	
	TIM2->DIER |= TIM_DIER_UIE;
	NVIC_EnableIRQ(TIM2_IRQn);
	
	
}


