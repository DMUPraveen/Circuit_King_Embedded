#include "GPIO_driver.h"
#include "Communication_Protocol_Size.h"

void run_gpio_command(){
	uint8_t instruction = usb_driver_data_buffer[1]; //get the gpio instruction
	HAL_GPIO_WritePin(Measuring_LED_GPIO_Port,Measuring_LED_Pin, !!(instruction & MEASURING_LED));
	HAL_GPIO_WritePin(Connected_LED_GPIO_Port,Connected_LED_Pin, !!(instruction & CONNECTED_LED));
}