#include "Communication.h"
#include "Pulse_Measure.h"
#include "Espcom.h"
void communication(){
	if(usb_driver_data_ready==USB_DRIVER_DATA_READY){
		uint8_t code = usb_driver_data_buffer[0];
		switch (code){
			case OSCILLOSCOPE_COMMAND:
				do_oscilloscope();
				break;
			case PULSE_DURATION_COMMAND:
				complete_pulse_measure();
				break;
			case SINE_WAVE_COMMAND:
				run_esp_com();
				break;
			case ARBITRARY_WAVE_COMMAND:
				run_esp_com();
				break;
			case IV_COMMAND:
				run_esp_com();
				break;
				
		}
	}
	usb_driver_clear_data_ready();
}
