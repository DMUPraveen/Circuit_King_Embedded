#include "Espcom.h"
#include "Communication_Protocol_Size.h"
#include "usbd_cdc_if.h"
extern UART_HandleTypeDef huart1;
void run_esp_com(){
	uint8_t transmit_buffer[32];
	uint8_t code = usb_driver_data_buffer[0]; //get the command code
	HAL_UART_Receive(&huart1,transmit_buffer,IV_COMMAND_UART_REPLY_SIZE,1);
	HAL_UART_Transmit(&huart1,usb_driver_data_buffer,usb_driver_data_length_in_buffer,1000); //transmit the message	
	// recieve and send stuff it is necessary to be done so
	switch (code){
		case SINE_WAVE_COMMAND:
			break; //no reply
		case ARBITRARY_WAVE_COMMAND:
			break; //no reply
		case IV_COMMAND:
			HAL_UART_Receive(&huart1,transmit_buffer,IV_COMMAND_UART_REPLY_SIZE,10000);
			CDC_Transmit_FS(transmit_buffer,IV_COMMAND_UART_REPLY_SIZE);
		
	}
}