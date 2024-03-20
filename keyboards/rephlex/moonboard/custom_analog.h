#include <stdint.h>
#include <stdbool.h>
#include <hal.h>
#include <ch.h>

void adc_init(void);
void adc_start(void);
adcsample_t adcSample[1];
bool conversion_done;
