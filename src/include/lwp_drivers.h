#ifndef __LWP_DRIVERS_H__
#define __LWP_DRIVERS_H__ 1
#include <lwp.h>
#include <stdint.h>



void _lwp_driver_base_motor_setup(lwp_device_t* d,uint8_t p);



void _lwp_driver_led_light_init(lwp_device_t* d,uint8_t p);



void lwp_driver_led_light_set_color(lwp_device_t* d,uint8_t p,uint8_t r,uint8_t g,uint8_t b);



void _lwp_driver_gyro_init(lwp_device_t* d,uint8_t p);



void lwp_driver_gyro_setup(lwp_device_t* d,uint8_t p,uint32_t ul);



void _lwp_driver_gyro_update(lwp_device_t* d,uint8_t p,uint32_t f,float tm,void* bf);



static lwp_driver_t LWP_DRIVERS[]={
	{
		0x0000,
		"<Unknown>",
		NULL,
		NULL,
		NULL
	},
	{
		0x0001,
		"Motor",
		_lwp_driver_base_motor_setup,
		NULL,
		NULL
	},
	{
		0x0002,
		"Train Motor",
		NULL,
		NULL,
		NULL
	},
	{
		0x0005,
		"Button",
		NULL,
		NULL,
		NULL
	},
	{
		0x0008,
		"Led Light",
		NULL,
		NULL,
		NULL
	},
	{
		0x0014,
		"Voltage Sensor",
		NULL,
		NULL,
		NULL
	},
	{
		0x0015,
		"Current Sensor",
		NULL,
		NULL,
		NULL
	},
	{
		0x0016,
		"Piezo Tone Sound",
		NULL,
		NULL,
		NULL
	},
	{
		0x0017,
		"RGB Light",
		_lwp_driver_led_light_init,
		NULL,
		NULL
	},
	{
		0x0022,
		"External Tilt Sensor",
		NULL,
		NULL,
		NULL
	},
	{
		0x0023,
		"Motion Sensor",
		NULL,
		NULL,
		NULL
	},
	{
		0x0025,
		"Vision Sensor",
		NULL,
		NULL,
		NULL
	},
	{
		0x0026,
		"Linear Motor",
		NULL,
		NULL,
		NULL
	},
	{
		0x0027,
		"Internal Motor",
		NULL,
		NULL,
		NULL
	},
	{
		0x0028,
		"Tilt Sensor",
		NULL,
		NULL,
		NULL
	},
	{
		0x0029,
		"Train Motor",
		NULL,
		NULL,
		NULL
	},
	{
		0x002a,
		"Train Speaker",
		NULL,
		NULL,
		NULL
	},
	{
		0x002b,
		"Train Color Sensor",
		NULL,
		NULL,
		NULL
	},
	{
		0x002c,
		"Train Speedometer",
		NULL,
		NULL,
		NULL
	},
	{
		0x002e,
		"Large Linear Motor",
		NULL,
		NULL,
		NULL
	},
	{
		0x002f,
		"XL Linear Motor",
		_lwp_driver_base_motor_setup,
		NULL,
		NULL
	},
	{
		0x0030,
		"Medium Angular Motor",
		NULL,
		NULL,
		NULL
	},
	{
		0x0031,
		"Large Angular Motor",
		NULL,
		NULL,
		NULL
	},
	{
		0x0036,
		"Gesture Sensor",
		NULL,
		NULL,
		NULL
	},
	{
		0x0037,
		"Remote Control Button",
		NULL,
		NULL,
		NULL
	},
	{
		0x0038,
		"Remote Control Rssi",
		NULL,
		NULL,
		NULL
	},
	{
		0x0039,
		"Acceleormoter",
		NULL,
		NULL,
		NULL
	},
	{
		0x003a,
		"Gyro Sensor",
		_lwp_driver_gyro_init,
		_lwp_driver_gyro_update,
		NULL
	},
	{
		0x003b,
		"Tilt Sensor",
		NULL,
		NULL,
		NULL
	},
	{
		0x003c,
		"Temperature Sensor",
		NULL,
		NULL,
		NULL
	},
	{
		0x003d,
		"Color Sensor",
		NULL,
		NULL,
		NULL
	},
	{
		0x003e,
		"Distance Sensor",
		NULL,
		NULL,
		NULL
	},
	{
		0x003f,
		"Force Sensor",
		NULL,
		NULL,
		NULL
	},
	{
		0x004b,
		"Medium Angular Motor",
		NULL,
		NULL,
		NULL
	},
	{
		0x004c,
		"Large Angular Motor",
		NULL,
		NULL,
		NULL
	}
};



#endif
