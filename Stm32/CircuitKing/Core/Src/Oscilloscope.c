#include "Oscilloscope.h"
#include "usbd_cdc_if.h"
#define CONVERSION_ONGOING 0
#define CONVERSION_DONE 1
volatile uint8_t conversion_done = CONVERSION_ONGOING; 
uint16_t adc_buffer[ADC_BUF_LEN+10] = {0};
void do_oscilloscope(){
	conversion_done = CONVERSION_ONGOING;
	HAL_ADC_Start_DMA(&hadc1,(uint32_t*)adc_buffer,ADC_BUF_LEN);
	while(conversion_done==CONVERSION_ONGOING){};
	CDC_Transmit_FS(((uint8_t*)adc_buffer),ADC_BUF_LEN*2);
}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
  //HAL_GPIO_TogglePin(LED_PIN_GPIO_Port,LED_PIN_Pin);
	conversion_done = CONVERSION_DONE;
	//HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
	
}