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
#define LWP_DEVICE_PORT_DRIVER 64

#define GET_LWP_DEVICE_PORT_MODE_COUNT(f) (((f)&0xf)+1)
#define GET_LWP_DEVICE_PORT_COMBINATION_COUNT(f) ((f)>>4)
#define SET_LWP_DEVICE_PORT_MODE_COUNT(c) ((c)-1)
#define SET_LWP_DEVICE_PORT_COMBINATION_COUNT(c) ((c)<<4)

#define LWP_COMMAND_HUB_PROPERTIES 0x01
#define LWP_COMMAND_HUB_ACTIONS 0x02
#define LWP_COMMAND_HUB_ALERTS 0x03
#define LWP_COMMAND_HUB_ATTACHED_IO 0x04
#define LWP_COMMAND_GENERIC_ERROR_MESSAGES 0x05
#define LWP_COMMAND_HW_NETWORK_COMMANDS 0x08
#define LWP_COMMAND_FW_UPDATE_GO_INTO_BOOT_MODE 0x10
#define LWP_COMMAND_FW_UPDATE_LOCK_MEMORY 0x11
#define LWP_COMMAND_FW_UPDATE_LOCK_STATUS_REQUEST 0x12
#define LWP_COMMAND_FW_LOCK_STATUS 0x13
#define LWP_COMMAND_PORT_INFORMATION_REQUEST 0x21
#define LWP_COMMAND_PORT_MODE_INFORMATION_REQUEST 0x22
#define LWP_COMMAND_PORT_INPUT_FORMAT_SETUP_SINGLE 0x41
#define LWP_COMMAND_PORT_INPUT_FORMAT_SETUP_COMBINED 0x42
#define LWP_COMMAND_PORT_INFORMATION 0x43
#define LWP_COMMAND_PORT_MODE_INFORMATION 0x44
#define LWP_COMMAND_PORT_VALUE_SINGLE 0x45
#define LWP_COMMAND_PORT_VALUE_COMBINED 0x46
#define LWP_COMMAND_PORT_INPUT_FORMAT_SINGLE 0x47
#define LWP_COMMAND_PORT_INPUT_FORMAT_COMBINED 0x48
#define LWP_COMMAND_VIRTUAL_PORT_SETUP 0x61
#define LWP_COMMAND_PORT_OUTPUT_COMMAND 0x81
#define LWP_COMMAND_PORT_OUTPUT_COMMAND_FEEDBACK 0x82

#define LWP_PORT_INFORMATION_TYPE_VALUE 0x00
#define LWP_PORT_INFORMATION_TYPE_MODE 0x01
#define LWP_PORT_INFORMATION_TYPE_COMBINATIONS 0x02

#define LWP_PORT_MODE_INFORMATION_TYPE_NAME 0x00
#define LWP_PORT_MODE_INFORMATION_TYPE_RAW 0x01
#define LWP_PORT_MODE_INFORMATION_TYPE_PCT 0x02
#define LWP_PORT_MODE_INFORMATION_TYPE_SI 0x03
#define LWP_PORT_MODE_INFORMATION_TYPE_SYMBOL 0x04
#define LWP_PORT_MODE_INFORMATION_TYPE_MAPPING 0x05
#define LWP_PORT_MODE_INFORMATION_TYPE_VALUE_FORMAT 0x80

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
#define GET_LWP_DEVICE_PORT_MODE_DATASET_TYPE(f) ((f)>>12)

#define LWP_WAIT_FOR_ALL_PORTS 0

#define LWP_PORT_WRITE_DIRECT_MODE_DATA 0x51

#define LWP_PORT_OUTPUT_FORMAT_BUFFER 0
#define LWP_PORT_OUTPUT_FORMAT_NO_BUFFER 16
#define LWP_PORT_OUTPUT_FORMAT_FEEDBACK 1
#define LWP_PORT_OUTPUT_FORMAT_NO_FEEDBACK 0

#define LWP_DEVICE_FREE_SHUTDOWN 0
#define LWP_DEVICE_FREE_DISCONNECT 1



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
	uint8_t _dtl;
	lwp_device_port_mode_range_t rr;
	lwp_device_port_mode_range_t pr;
	lwp_device_port_mode_range_t sr;
} lwp_device_port_mode_t;



typedef struct __LWP_DEVICE_PORT{
	void* _drv_dt;
	uint16_t t;
	uint8_t f;
	uint8_t _cnt;
	uint16_t _drv_id;
	uint16_t sm;
	uint8_t mf;
	uint16_t mc[8];
	lwp_device_port_mode_t ml[16];
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



typedef void (*lwp_driver_init_func)(lwp_device_t* d,uint8_t p);
typedef void (*lwp_driver_update_func)(lwp_device_t* d,uint8_t p,uint32_t f,float tm,void* bf);
typedef void (*lwp_driver_deinit_func)(lwp_device_t* d,uint8_t p);



typedef struct __LWP_DRIVER{
	uint16_t id;
	const char* nm;
	lwp_driver_init_func init;
	lwp_driver_update_func update;
	lwp_driver_deinit_func deinit;
} lwp_driver_t;



typedef uint8_t (*lwp_device_found_t)(lwp_device_data_t* dt);



lwp_device_list_t* lwp_find_devices(uint32_t tm,lwp_device_found_t cb);



void lwp_free_device_list(lwp_device_list_t* dl);



void lwp_send_raw_data(lwp_device_t* d,uint8_t* dt,uint16_t dtl);



void lwp_wait_for_ports(lwp_device_t* d,uint8_t c,...);



lwp_device_port_t* lwp_get_port(lwp_device_t* d,uint8_t p);



const char* lwp_get_port_string(lwp_device_t* d,uint8_t p);



void lwp_setup_port(lwp_device_t* d,uint8_t p,uint8_t m,uint32_t di,uint8_t ne);



void lwp_setup_port_multiple(lwp_device_t* d,uint8_t p,uint8_t c,...);



void lwp_free_device(lwp_device_t* d,uint8_t t);



#endif
