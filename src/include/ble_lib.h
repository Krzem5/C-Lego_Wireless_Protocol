#ifndef __BLE_LIB_H__
#define __BLE_LIB_H__ 1
#ifdef __cplusplus
#include <cstdint>
extern "C" {
#else
#include <stdint.h>
#endif



#define BLE_CHRACTERISTIC_FLAG_BROADCAST 1
#define BLE_CHRACTERISTIC_FLAG_READ 2
#define BLE_CHRACTERISTIC_FLAG_WRITEWITHOUTRESPONSE 4
#define BLE_CHRACTERISTIC_FLAG_WRITE 8
#define BLE_CHRACTERISTIC_FLAG_NOTIFY 16
#define BLE_CHRACTERISTIC_FLAG_INDICATE 32
#define BLE_CHRACTERISTIC_FLAG_AUTHENTICATEDSIGNEDWRITES 64
#define BLE_CHRACTERISTIC_FLAG_EXTENDEDPROPERTIES 128
#define BLE_CHRACTERISTIC_FLAG_RELIABLEWRITES 256
#define BLE_CHRACTERISTIC_FLAG_WRITABLEAUXILIARIES 512



#define BLE_LIB_CREATE_UUID_A(a,b,c) ((((uint64_t)(a))<<32)|(((uint64_t)(b))<<16)|(c))
#define BLE_LIB_CREATE_UUID_B(a,b) ((((uint64_t)(a))<<48)|(b))



typedef struct __BLE_DEVICE_MANUFACTURER_DATA{
	uint32_t l;
	uint16_t id;
	uint8_t* dt;
} ble_device_manufacturer_data_t;



typedef struct __BLE_DEVICE_MANUFACTURER_DATA_LIST{
	uint32_t l;
	ble_device_manufacturer_data_t* data;
} ble_device_manufacturer_data_list_t;



typedef struct __BLE_GUID{
	uint64_t a;
	uint64_t b;
	char s[37];
} ble_guid_t;



typedef struct __BLE_DEVICE_SERVICE_LIST{
	uint32_t l;
	ble_guid_t* uuids;
} ble_device_service_list_t;



typedef struct __BLE_DEVICE{
	uint64_t addr;
	char addr_s[18];
	ble_device_manufacturer_data_list_t manufacturer_data;
	ble_device_service_list_t services;
} ble_device_t;



typedef struct __BLE_DEVICE_CHARACTERISTICS{
	ble_guid_t uuid;
	uint16_t f;
	uint64_t _dt;
} ble_connected_device_characteristics_t;



typedef struct __BLE_DEVICE_CHARACTERISTICS_LIST{
	uint32_t l;
	ble_connected_device_characteristics_t* data;
} ble_connected_device_characteristics_list_t;



typedef struct __BLE_CONNECTED_DEVICE_SERVICE{
	ble_guid_t uuid;
	ble_connected_device_characteristics_list_t characteristics;
	uint64_t _dt;
} ble_connected_device_service_t;



typedef struct __BLE_CONNECTED_DEVICE_SERVICE_LIST{
	uint32_t l;
	ble_connected_device_service_t* data;
} ble_connected_device_service_list_t;



typedef struct __BLE_CONNECTED_DEVICE{
	ble_device_t* dv;
	ble_connected_device_service_list_t services;
	uint64_t _dt;
} ble_connected_device_t;



typedef struct __BLE_CHARACTERISTIC_NOTIFICATION_DATA{
	uint64_t tm;
	uint32_t l;
	uint8_t* bf;
} ble_characteristic_notification_data_t;



typedef void* (*ble_device_found_t)(ble_device_t* dv,void* arg);
typedef void (*ble_characteristic_notification_t)(ble_characteristic_notification_data_t dt,void* arg);



void ble_lib_init(void);



void* ble_lib_enum_devices(uint32_t tm,ble_device_found_t cb,void* arg);



ble_connected_device_t* ble_lib_connect_device(ble_device_t* dv);



void ble_lib_load_characteristics(ble_connected_device_service_t* s);



void ble_lib_register_characteristic_notification(ble_connected_device_characteristics_t* c,ble_characteristic_notification_t cb,void* arg);



void ble_lib_write_characteristic(ble_connected_device_characteristics_t* c,uint8_t* dt,uint32_t dtl);



void ble_lib_disconnect_device(ble_connected_device_t* cdv);



void ble_lib_free_device(ble_device_t* dv);



#ifdef __cplusplus
}
#endif
#endif
