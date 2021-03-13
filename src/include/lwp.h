#ifndef __LWP_H__
#define __LWP_H__ 1
#include <stdint.h>
#include <ble_lib.h>
#include <windows.h>



#define LWP_DEVICE_FOUND_IGNORE 0
#define LWP_DEVICE_FOUND_ACCUMULATE 1
#define LWP_DEVICE_FOUND_ACCUMULATE_RETURN 2

#define LWP_DEVICE_TYPE_WEDO_HUB 0
#define LWP_DEVICE_TYPE_DUPLO_TRAIN 0x20
#define LWP_DEVICE_TYPE_MOVE_HUB 0x40
#define LWP_DEVICE_TYPE_2PORT_HUB 0x41
#define LWP_DEVICE_TYPE_2PORT_HANDSET 0x42
#define LWP_DEVICE_TYPE_MEDIUM_HUB 0x80
#define LWP_DEVICE_TYPE_UNKNOWN 0xff

#define LWP_DEVICE_PORT_ATTACHED 1
#define LWP_DEVICE_PORT_VIRTUAL 2
#define LWP_DEVICE_PORT_OUTPUT 4
#define LWP_DEVICE_PORT_INPUT 8
#define LWP_DEVICE_PORT_COMBINABLE 16
#define LWP_DEVICE_PORT_SYNCHRONIZABLE 32

#define LWP_DEVICE_PORT_MODE_INPUT 1
#define LWP_DEVICE_PORT_MODE_INPUT_MAPPING_DISCRETE 2
#define LWP_DEVICE_PORT_MODE_INPUT_MAPPING_RELATIVE 4
#define LWP_DEVICE_PORT_MODE_INPUT_MAPPING_ABSOLUTE 8
#define LWP_DEVICE_PORT_MODE_INPUT_MAPPING_FUNCTIONAL_2_0 16
#define LWP_DEVICE_PORT_MODE_INPUT_MAPPING_NULL 32
#define LWP_DEVICE_PORT_MODE_OUTPUT 64
#define LWP_DEVICE_PORT_MODE_OUTPUT_MAPPING_DISCRETE 128
#define LWP_DEVICE_PORT_MODE_OUTPUT_MAPPING_RELATIVE 256
#define LWP_DEVICE_PORT_MODE_OUTPUT_MAPPING_ABSOLUTE 512
#define LWP_DEVICE_PORT_MODE_OUTPUT_MAPPING_FUNCTIONAL_2_0 1024
#define LWP_DEVICE_PORT_MODE_OUTPUT_MAPPING_NULL 2048
#define LWP_DEVICE_PORT_MODE_DATASET_TYPE_8BYTE 0
#define LWP_DEVICE_PORT_MODE_DATASET_TYPE_16BYTE 4096
#define LWP_DEVICE_PORT_MODE_DATASET_TYPE_32BYTE 8192
#define LWP_DEVICE_PORT_MODE_DATASET_TYPE_FLOAT 12288
#define GET_LWP_DEVICE_PORT_MODE_DATASET_TYPE(f) (((f)>>12)&3)



typedef struct __LWP_DEVICE_DATA{
	uint64_t addr;
	uint8_t dt;
} lwp_device_data_t;



typedef struct __LWP_DEVICE_PORT_MODE_RANGE{
	float mn;
	float mx;
} lwp_device_port_mode_range_t;



typedef struct __LWP_DEVICE_PORT_MODE{
	uint16_t f;
	char nm[12];
	char sm[6];
	lwp_device_port_mode_range_t rr;
	lwp_device_port_mode_range_t pr;
	lwp_device_port_mode_range_t sr;
} lwp_device_port_mode_t;



typedef struct __LWP_DEVICE_PORT_MODES{
	uint8_t l;
	uint8_t cc;
	uint16_t c[8];
	lwp_device_port_mode_t ml[16];
} lwp_device_port_mode_list_t;



typedef struct __LWP_DEVICE_PORT{
	uint8_t f;
	uint16_t t;
	lwp_device_port_mode_list_t modes;
} lwp_device_port_t;



typedef struct __LWP_DEVICE_PORT_LIST{
	uint8_t l;
	lwp_device_port_t* dt;
} lwp_device_port_list_t;



typedef struct __LWP_DEVICE{
	uint64_t addr;
	uint8_t t;
	lwp_device_port_list_t ports;
	ble_connected_device_t* _dt;
	ble_connected_device_characteristics_t* _dc;
	HANDLE _mx;
} lwp_device_t;



typedef struct __LWP_DEVICE_LIST{
	uint32_t l;
	lwp_device_t** dt;
} lwp_device_list_t;



typedef uint8_t (*lwp_device_found_t)(lwp_device_data_t* dt);



lwp_device_list_t* lwp_find_devices(uint32_t tm,lwp_device_found_t cb);



void lwp_free_device_list(lwp_device_list_t* dl);



#endif
