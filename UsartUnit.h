#include "stm32f4xx.h"                  // Device header
#include "stdint.h"

// why this numbers? check keypad
#define USART_STAR 10
#define USART_SIGN_NUMBER 12

void usart_init(void);


void usart_write(char* data);

uint16_t usart_read(void);


uint16_t usart_encode(uint16_t data);

uint16_t usart_decode(uint16_t data);

