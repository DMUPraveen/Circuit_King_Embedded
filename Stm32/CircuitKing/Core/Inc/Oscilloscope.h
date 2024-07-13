#ifndef OSCILLOSCOPE_H
#define OSCILLOSCOPE_H
#include "main.h"
#define ADC_BUF_LEN 1024
extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;
extern uint16_t adc_buffer[];
void do_oscilloscope();

#endif