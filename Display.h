#include "stm32f4xx.h"                  // Device header
#include <stdint.h>

static int cursor;

void display_init(void);

void display_clear(void);

void display_command(uint8_t data);

void display_write_inner(uint8_t data);

void display_write(char* sequence);

void display_ln(void);

