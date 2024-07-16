#include "usb_driver.h"
#include "Communication_Protocol_Size.h"
#include "string.h"
uint8_t usb_driver_data_buffer[USB_DRIVER_BUFFER_SIZE] = {0};
size_t data_received = 0;
size_t expected_data_length = 0;
volatile uint8_t usb_driver_data_ready = USB_DRIVER_DATA_NOT_READY;
volatile size_t usb_driver_data_length_in_buffer = 0;

void usb_driver_clear_buffer(){
	usb_driver_data_length_in_buffer = data_received;
	data_received = 0;
	expected_data_length = 0;
}

void signal_data_ready(){
	usb_driver_data_ready = USB_DRIVER_DATA_READY;
}

void usb_driver_clear_data_ready(){
	usb_driver_data_ready = USB_DRIVER_DATA_NOT_READY;
}

void usb_driver_data_recieved_callback(uint8_t* buffer, size_t data_len){
	if(data_received == 0){//means start of a new frame
		//check the control code.
		uint8_t control_code = buffer[0];
		//set the expected size
		switch(control_code){
			case SINE_WAVE_COMMAND:
				expected_data_length = SINE_WAVE_COMMAND_SIZE;
				break;
			case ARBITRARY_WAVE_COMMAND:
				expected_data_length = ARBITRARY_WAVE_COMMAND_SIZE;
				break;
			case PULSE_DURATION_COMMAND:
				expected_data_length = PULSE_DURATION_COMMAND_SIZE;
				break;
			case OSCILLOSCOPE_COMMAND:
				expected_data_length = OSCILLOSCOPE_COMMAND_SIZE;
				break;
			case IV_COMMAND:
				expected_data_length = IV_COMMAND_SIZE;
				break;
			case GPIO_COMMAND:
				expected_data_length = GPIO_COMMAND_SIZE;
				break;
		}
	}
	
	memcpy((usb_driver_data_buffer+data_received),buffer,data_len); //copy the data to data accumilation buffer
	
	data_received += data_len;//update the amount of data received
	
	if(data_received >= expected_data_length){
		signal_data_ready(); //once the total amount of data is received we signal that we are done.
		usb_driver_clear_buffer();//clear for the next data reciveal
		//however if data is recieved before the data is used this data will be overwritten -- needs to be adressed
	}
}