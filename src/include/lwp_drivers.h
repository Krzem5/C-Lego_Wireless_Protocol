#ifndef __LWP_DRIVERS_H__
#define __LWP_DRIVERS_H__ 1
#include <lwp.h>
#include <stdint.h>



void lwp_driver_led_light_init(lwp_device_t* d,uint8_t p);



void lwp_driver_led_light_set_color(lwp_device_t* d,uint8_t p,uint8_t r,uint8_t g,uint8_t b);



#endif
