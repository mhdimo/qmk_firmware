#include <stdint.h>
#include <stdbool.h>
#include <hal.h>
#include <ch.h>

void adc_init(void);
void adc_start(void);
adcsample_t sampleBuffer[1];
bool is_adc_conversion_done(void);
