#include "main.h"
#ifndef MY_USB_DRIVER_H
#define MY_USB_DRIVER_H

#define USB_DRIVER_BUFFER_SIZE 4096
#define USB_DRIVER_DATA_READY 1
#define USB_DRIVER_DATA_NOT_READY 2

void usb_driver_data_recieved_callback(uint8_t* buffer, size_t data_len);
void usb_driver_clear_data_ready();
extern uint8_t usb_driver_data_buffer[USB_DRIVER_BUFFER_SIZE];
extern volatile uint8_t usb_driver_data_ready;
extern volatile size_t usb_driver_data_length_in_buffer;
#endif