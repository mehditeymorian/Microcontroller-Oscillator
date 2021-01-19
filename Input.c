#include "stm32f4xx.h"                  // Device header
#include "Input.h"


void wave_switch_init(void){
	
	
	GPIOB->MODER &= ~GPIO_MODER_MODE12_Msk;
	
	GPIOB->PUPDR |= GPIO_PUPDR_PUPD5_1 | GPIO_PUPDR_PUPD6_1 | GPIO_PUPDR_PUPD7_1;
	GPIOB->PUPDR |= GPIO_PUPDR_PUPD8_1 | GPIO_PUPDR_PUPD9_1 | GPIO_PUPDR_PUPD10_1;
	GPIOB->PUPDR |= GPIO_PUPDR_PUPD12_1 | GPIO_PUPDR_PUPD13_1;
	
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; // sysclk
	

	SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI5_PB; 
	SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI6_PB; 
	SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI7_PB; 
	SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI8_PB; 
	SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI9_PB; 
	SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI10_PB; 
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI12_PB; 
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PB; 
	
	EXTI->IMR |= 0x1FF << 5;
	EXTI->RTSR |= 0x1FF << 5; 
	__enable_irq(); 
	
	NVIC_SetPriority(EXTI9_5_IRQn,0);
	NVIC_ClearPendingIRQ(EXTI9_5_IRQn); 
	NVIC_EnableIRQ(EXTI9_5_IRQn);
	
	NVIC_SetPriority(EXTI15_10_IRQn,0);
	NVIC_ClearPendingIRQ(EXTI15_10_IRQn); 
	NVIC_EnableIRQ(EXTI15_10_IRQn);
}


void params_adc_init(void) {
	GPIOA->MODER |= GPIO_MODER_MODE0; // set PA0 to analog
	GPIOA->MODER |= GPIO_MODER_MODE1; // set PA1 to analog
	
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD0;
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD1;
	
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; // enable clock for adc1
	
	ADC1->CR1 &= ~ADC_CR1_RES_Msk; // 12bit resolution

	ADC1->SQR3 |= 0 << ADC_SQR3_SQ1_Pos;
	ADC1->SQR3 |= 1 << ADC_SQR3_SQ2_Pos;


	ADC1->SQR1 &= ~ADC_SQR1_L_Msk;
	ADC1->SQR1 |= 1 << ADC_SQR1_L_Pos; // 2 conversion
	
	ADC1->SMPR2 |= 7 << ADC_SMPR2_SMP0_Pos;
	ADC1->SMPR2 |= 7 << ADC_SMPR2_SMP1_Pos;
	

	ADC1->CR1 |= ADC_CR1_SCAN; // scan mode
	ADC1->CR1 |= ADC_CR1_EOCIE;
	ADC1->CR2 |= ADC_CR2_EOCS;
	
	NVIC_EnableIRQ(ADC_IRQn);
	
	ADC1->CR2 |= ADC_CR2_ADON;
	
	
	ADC1->CR2 |= ADC_CR2_SWSTART;
		
}

void tim3_adc_init(void) {
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	TIM3->CR1 |= TIM_CR1_CKD_1;
	TIM3->PSC = 1000-1;
	TIM3->ARR = 750 - 1;
	TIM3->CNT = 0;
	
	
	TIM3->DIER |= TIM_DIER_UIE;
	NVIC_EnableIRQ(TIM3_IRQn);
	
	TIM3->CR1 |= TIM_CR1_CEN;
}

double calculateFrequency(uint8_t freq_raw){
	if(freq_raw == 0) return 1.0;
	else if (freq_raw <= 10) return freq_raw;
	else if (freq_raw <= 20) return (freq_raw-10) * 10.0;
	else if (freq_raw <= 38) return ((freq_raw-20) * 50.0) + 100;
	else if (freq_raw <= 78) return ((freq_raw-38) * 100.0) + 1000;
	else if (freq_raw <= 98) return ((freq_raw-78) * 250.0) + 5000;
	else if (freq_raw == 99) return 15000.0;
	else return 20000.0; // 100
}
														 
double calculateAmplitude(uint8_t amp_raw) {
	if			(amp_raw < 15) return 4.0;
	else if	(amp_raw < 30) return 5.0;
	else if	(amp_raw < 45) return 6.0;
	else if	(amp_raw < 60) return 7.0;
	else if	(amp_raw < 75) return 8.0;
	else if	(amp_raw < 90) return 9.0;
	else return 10.0;
}




