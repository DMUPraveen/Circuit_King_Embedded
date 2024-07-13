#include "Communication.h"
#include "Pulse_Measure.h"
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
				
		}
	}
	usb_driver_clear_data_ready();
}
