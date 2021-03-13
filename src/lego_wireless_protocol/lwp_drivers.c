#include <lwp.h>
#include <lwp_drivers.h>
#include <stdint.h>



void lwp_driver_led_light_init(lwp_device_t* d,uint8_t p){
	uint8_t bf[8]={0x41,p,0x01,0x00,0x00,0x00,0x00,0x01};
	lwp_send_raw_data(d,bf,sizeof(bf));
}



void lwp_driver_led_light_set_color(lwp_device_t* d,uint8_t p,uint8_t r,uint8_t g,uint8_t b){
	uint8_t bf[8]={0x81,p,0x11,0x51,0x01,r,g,b};
	lwp_send_raw_data(d,bf,sizeof(bf));
}
