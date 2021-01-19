#include "stm32f4xx.h"                  // Device header
#include "stdio.h"
#include "Main.h"
#include "Display.h"
#include "Input.h"
#include "Wave.h"
#include "UsartUnit.h"
#include "math.h"




volatile static uint8_t turn = 0;
volatile static uint8_t initState = 1;
volatile static uint8_t inputChange = 0;

volatile static uint8_t pwm_count = 0;
volatile static uint8_t pwm_start = 0;

int main(void)
{
	pwm_clock_init();
	gpio_init();
	wave_switch_init();
	display_init();
	usart_init();
	params_adc_init();
	tim3_adc_init();
	pwm_init();
	tim2_init();
	
	
	
	while(1){
		if(inputChange == 1) {
			inputChange = 0;
			on_params_update();
		}
	}
}



void gpio_init(void){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	GPIOB->MODER |= GPIO_MODER_MODE0_0;

}

void TIM2_IRQHandler(void) {
	NVIC_ClearPendingIRQ(TIM2_IRQn);
	TIM2->SR &= ~TIM_SR_UIF_Msk; // reset interrupt request
	TIM2->CNT = 0; // clear counter
	
	uint32_t val;
	switch(wave_type){
		case SINE_WAVE: 					 val = SINE_LUT[index]; break;
		case SQUARE_WAVE: 				 val = SQUARE_LUT[index]; break;
		case TRIANGLE_WAVE: 			 val = TRIANGLE_LUT[index]; break;
		case SAWTOOTH_WAVE: 			 val = SAWTOOTH_LUT[index]; break;
		case STAIRS_WAVE: 				 val = STAIRS_LUT[index]; break;
		case RECTIFIED_SINE_WAVE:  val = RECTIFIED_SINE_LUT[index]; break;
		case SEGMENTED_SINE_WAVE:  val = SEGMENTED_SINE_LUT[index]; break;
		case SINE_MODULATION_WAVE: val = SINE_MODULATION_LUT[index]; break;
		default: val = 0;
	}
	
	// update duty cycle of pwm based on amplitude
	val = (uint32_t)(wave_scale*2*(val/RES) + 2048 - wave_scale);
	
	TIM5->CCR3 = val;
	index = (index+1) % NS;
	
	GPIOB->ODR ^= mask(0);
}

void TIM3_IRQHandler(void){
	NVIC_ClearPendingIRQ(TIM3_IRQn);
	ADC1->CR2 |= ADC_CR2_SWSTART;

	if(pwm_start == 0){
		if(pwm_count < 60) pwm_count++;
		else {
			TIM5->CR1 |= TIM_CR1_CEN;
			TIM2->CR1 |= TIM_CR1_CEN;
			pwm_start = 1;
		}
	}
}

void ADC_IRQHandler(void){
	NVIC_ClearPendingIRQ(ADC_IRQn);
	TIM3->CR1 &= ~TIM_CR1_CEN;
	
	uint32_t temp = ADC1->DR;
	if(turn == 0){ // checking frequency input
		if(temp != freq_raw){
			freq_raw = temp;
			uint8_t freq_percentage = round((freq_raw/4095.0) * 100);
			frequency = calculateFrequency(freq_percentage);

			// update psc to change pwm frequency
			uint16_t val = CLK/(frequency*RES);
			TIM5->PSC = val;
			TIM2->PSC = val;
			
			
			if(initState == 1) initState = 0;
			else inputChange = 1;
			// log values
//			log_num("freq raw",freq_raw);
//			log_num("freq percentage",freq_percentage);
//			log_num("PSC",val);
		}
		turn = 1;
	} 
	else if (turn == 1){ // checking amplitude input
		if (temp != amp_raw){
			amp_raw = temp;
			uint8_t amp_percentage = round((amp_raw/4095.0) * 100);
			amplitude = calculateAmplitude(amp_percentage);
			wave_scale = (uint32_t) (((RES*amplitude)/MAX_AMPLITUDE) - 2048);
			inputChange = 1;
			// log the values
//			log_num("amp raw",amp_raw);
//			log_num("amp_percentage",amp_percentage);
		}
		turn = 0;
	}


	TIM3->CNT = 0;
	TIM3->CR1 |= TIM_CR1_CEN;
	
}

void on_params_update(void) {
	display_clear();
	display_write(WAVE_TYPE[wave_type]);
	display_ln();
	display_write("amp: ");
	display_write_number(amplitude);
	display_write(" freq: ");
	display_write_number(frequency);
//	char parameters[32];
//	sprintf(parameters,"amp=%.2f freq=%.0f\r\n", amplitude, frequency);
//	display_write(parameters);
}


void EXTI9_5_IRQHandler(void){
	NVIC_ClearPendingIRQ(EXTI9_5_IRQn);
	handle_wave_type();
}

void EXTI15_10_IRQHandler(void) {
	NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
	handle_wave_type();
}

void handle_wave_type(void) {
	int last_type = wave_type;
	if(GPIOB->IDR & mask(5)) {
		wave_type = 0;
		EXTI->PR|= mask(5);
	}
	else if (GPIOB->IDR & mask(6)){ 
		wave_type = 1;
		EXTI->PR|= mask(6);
	}
	else if (GPIOB->IDR & mask(7)){ 
		wave_type = 2;
		EXTI->PR|= mask(7);
	}
	else if (GPIOB->IDR & mask(8)){ 
		wave_type = 3;
		EXTI->PR|= mask(8);
	}
	else if (GPIOB->IDR & mask(9)){ 
		wave_type = 4;
		EXTI->PR|= mask(9);
	}
	else if(GPIOB->IDR & mask(10)) {
		wave_type = 5;
		EXTI->PR|= mask(10);
	}
	else if(GPIOB->IDR & mask(12)) {
		wave_type = 6;
		EXTI->PR|= mask(12);
	}
	else if(GPIOB->IDR & mask(13)) {
		wave_type = 7;
		EXTI->PR|= mask(13);
	}
	
	if(last_type != wave_type){
		index =0;
		inputChange = 1;
	}
		
	

}

void delay(long ms){
	int i;
  int j;
  for(i = 0 ; i < ms; i++)
		for (j = 0; j < 2000; j++) 
			__NOP();  
	  
}

void log_num(char title[], uint32_t number) {
	
	char text2[50];
	sprintf(text2,"%s: %d\r\n",title, number);
	usart_write(text2);
}

void log_numd(char title[], double number) {
	char text2[50];
	sprintf(text2,"%s: %.3f\r\n",title, number);
	usart_write(text2);
}

void display_write_number(double number) {
	int temp = number;
	int length = 0;
	while(temp != 0) {
		temp = temp / 10;
		length++;
	}
	temp = number;
	int revers_number = 0;
	int digit = 0;
	for(int i = 0; i < length; i++)
	{
		revers_number *= 10;
		revers_number += temp % 10;
		temp = temp / 10;
	}
	for(int i = 0; i < length; i++)
	{
		char c = (char)((revers_number % 10) + '0');
		revers_number /= 10;
		display_write_inner(c);
	}
}



