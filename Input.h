#include "stdint.h"


#define SINE_WAVE 0
#define SQUARE_WAVE 1
#define TRIANGLE_WAVE 2
#define SAWTOOTH_WAVE 3
#define STAIRS_WAVE 4
#define RECTIFIED_SINE_WAVE 5
#define SEGMENTED_SINE_WAVE 6
#define SINE_MODULATION_WAVE 7



static char* WAVE_TYPE[8] = {"Sine Wave",
														 "Square Wave",
														 "Triangle Wave",
														 "Sawtooth Wave",
														 "Stairs Wave",
														 "Rectified Sine Wave",
														 "Vertical Segmented Sine Wave",
														 "Sqaure Sine Modulation Wave"};

														 

void wave_switch_init(void);												 
void params_adc_init(void);
void tim3_adc_init(void);														
														 
double calculateFrequency(uint8_t freq_raw);
														 
double calculateAmplitude(uint8_t amp_raw);
														 
			
														 