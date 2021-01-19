#include "stdint.h"

#define mask(x) (1UL << x)



void gpio_init(void);
														
void EXTI9_5_IRQHandler(void);

void EXTI15_10_IRQHandler(void);

void handle_wave_type(void);
													 
void delay(long ms);

void ADC_IRQHandler(void);

void on_params_update(void);

void TIM2_IRQHandler(void);

void TIM3_IRQHandler(void);

void log_num(char title[], uint32_t number);

void log_numd(char title[], double number);

void display_write_number(double number);

