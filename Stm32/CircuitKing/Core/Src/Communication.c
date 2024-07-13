#include "Communication.h"
void communication(){
	if(usb_driver_data_ready==USB_DRIVER_DATA_READY){
		uint8_t code = usb_driver_data_buffer[0];
		switch (code){
			case OSCILLOSCOPE_COMMAND:
				do_oscilloscope();
				break;
		}
	}
	usb_driver_clear_data_ready();
}
