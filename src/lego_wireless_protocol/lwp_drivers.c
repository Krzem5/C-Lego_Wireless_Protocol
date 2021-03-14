#include <lwp.h>
#include <lwp_drivers.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>



void _lwp_driver_led_light_init(lwp_device_t* d,uint8_t p){
	lwp_setup_port(d,p,1,0,0);
}



void lwp_driver_led_light_set_color(lwp_device_t* d,uint8_t p,uint8_t r,uint8_t g,uint8_t b){
	uint8_t bf[8]={LWP_COMMAND_PORT_OUTPUT_COMMAND,p,LWP_PORT_OUTPUT_FORMAT_NO_BUFFER|LWP_PORT_OUTPUT_FORMAT_NO_FEEDBACK,LWP_PORT_WRITE_DIRECT_MODE_DATA,0x01,r,g,b};
	lwp_send_raw_data(d,bf,sizeof(bf));
}



void _lwp_driver_gyro_init(lwp_device_t* d,uint8_t p){
	lwp_setup_port(d,p,0,10,1);
}



void _lwp_driver_gyro_update(lwp_device_t* d,uint8_t p,...){
	//
}
