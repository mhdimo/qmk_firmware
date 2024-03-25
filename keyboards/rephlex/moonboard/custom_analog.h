#include <stdint.h>
#include <stdbool.h>
#include <hal.h>
#include <ch.h>

void adc_init(void);
void adc_start_conversion(void);
adcsample_t adc_samples[6];
