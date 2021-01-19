#include "stm32f4xx.h"                  // Device header
#include "Display.h"
#include "Main.h"




#define DISPLAY_DI 0
#define DISPLAY_RW 1
#define DISPLAY_ENABLE 2
#define DISPLAY_DATA 3



void display_init(void){
	GPIOC->MODER |= GPIO_MODER_MODE0_0| GPIO_MODER_MODE1_0 | GPIO_MODER_MODE2_0;
	GPIOC->MODER |= GPIO_MODER_MODE3_0 | GPIO_MODER_MODE4_0; 
	GPIOC->MODER |= GPIO_MODER_MODE5_0| GPIO_MODER_MODE6_0| GPIO_MODER_MODE7_0;
	GPIOC->MODER |= GPIO_MODER_MODE8_0| GPIO_MODER_MODE9_0| GPIO_MODER_MODE10_0;
	
	 // initialization sequence /
	display_command(0x30);
	delay(30);
	display_command(0x30);
	delay(30);
	display_command(0x30);
	delay(30);
	display_command(0x38); // function set: 8-bit, 2-line, 5x7 font /
	display_clear();
	display_command(0x06); // move cursor right after each char /
	display_command(0x0C); // turn on display, cursor off /
	display_command(0x80);
}


void display_clear(void){
	display_command(0x01); // clear screen, move cursor to home /
	delay(10);
	cursor = 0;
}

void display_write_inner(uint8_t data) {
	CLEAR_BIT(GPIOC->ODR,mask(DISPLAY_RW));
	delay(1);
	SET_BIT(GPIOC->ODR,mask(DISPLAY_ENABLE));
	delay(1);
	GPIOC->ODR &= ~(0xFF << DISPLAY_DATA);
	GPIOC->ODR |= data << DISPLAY_DATA;
	delay(1);
	CLEAR_BIT(GPIOC->ODR,mask(DISPLAY_ENABLE));
	delay(1);
}


void display_command(uint8_t data) {
	CLEAR_BIT(GPIOC->ODR,mask(DISPLAY_DI));
	display_write_inner(data);
}

void display_write(char* sequence) {
	SET_BIT(GPIOC->ODR,mask(DISPLAY_DI));
	int i = 0;
	while(1) {
		char c = sequence[i++];
		if(c == '\0')
			break;
	  display_write_inner(c);
	  cursor++;
	}
		
}


void display_ln(void) {
	display_command(0xC0);
}
