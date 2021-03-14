#ifndef __LWP_DRIVERS_H__
#define __LWP_DRIVERS_H__ 1
#include <lwp.h>
#include <stdint.h>



void _lwp_driver_led_light_init(lwp_device_t* d,uint8_t p);



void lwp_driver_led_light_set_color(lwp_device_t* d,uint8_t p,uint8_t r,uint8_t g,uint8_t b);



void _lwp_driver_gyro_init(lwp_device_t* d,uint8_t p);



void _lwp_driver_gyro_update(lwp_device_t* d,uint8_t p,...);



static lwp_driver_t LWP_DRIVERS[]={
	{
		0x0017,
		"Led Light",
		_lwp_driver_led_light_init,
		NULL,
		NULL
	},
	{
		0x003a,
		"Gyro",
		_lwp_driver_gyro_init,
		_lwp_driver_gyro_update,
		NULL
	}
};



#endif
