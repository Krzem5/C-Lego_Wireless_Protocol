#include <ble_lib.h>
#include <windows.h>
#include <Windows.Foundation.h>
#include <Windows.Devices.Bluetooth.h>
#include <Windows.Devices.Bluetooth.Advertisement.h>
#include <wrl/wrappers/corewrappers.h>
#include <wrl.h>
#include <robuffer.h>
#include <inspectable.h>
#include <pplawait.h>
#include <cstdio>
#include <cstdlib>



std::vector<Windows::Devices::Bluetooth::BluetoothLEDevice^> _d_ref;
std::vector<Windows::Devices::Bluetooth::GenericAttributeProfile::GattDeviceService^> _gs_ref;
std::vector<Windows::Devices::Bluetooth::GenericAttributeProfile::GattCharacteristic^> _c_ref;



char _hex(uint8_t v){
	return v+(v<10?48:87);
}



uint8_t _dehex(char c){
	return c-(c>96?87:48);
}



void _init_lib_cpp(void){
	CoInitializeSecurity(nullptr,-1,nullptr,nullptr,RPC_C_AUTHN_LEVEL_DEFAULT,RPC_C_IMP_LEVEL_IDENTIFY,NULL,EOAC_NONE,nullptr);
}



void* _enum_dev_cpp(uint32_t tm,ble_device_found_t cb,void* arg){
	Microsoft::WRL::Wrappers::RoInitializeWrapper initialize(RO_INIT_MULTITHREADED);
	Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementWatcher^ ble_w=ref new Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementWatcher();
	uint64_t* _al=(uint64_t*)(void*)0;
	uint64_t** al=&_al;
	uint64_t _all=0;
	uint64_t* all=&_all;
	void* _r=(void*)0;
	void** r=&_r;
	HANDLE mx=CreateMutex(NULL,FALSE,NULL);
	ble_w->ScanningMode=Windows::Devices::Bluetooth::Advertisement::BluetoothLEScanningMode::Active;
	ble_w->Received+=ref new Windows::Foundation::TypedEventHandler<Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementWatcher^,Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementReceivedEventArgs^>([=](Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementWatcher^ w,Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementReceivedEventArgs^ ea){
		if (!ea->BluetoothAddress){
			return;
		}
		WaitForSingleObject(mx,INFINITE);
		if (ble_w->Status!=Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementWatcherStatus::Started){
			return;
		}
		for (uint64_t i=0;i<*all;i++){
			if (*((*al)+i)==ea->BluetoothAddress){
				ReleaseMutex(mx);
				return;
			}
		}
		(*all)++;
		*al=(uint64_t*)std::realloc(*al,(*all)*sizeof(uint64_t));
		*((*al)+(*all)-1)=ea->BluetoothAddress;
		ble_device_t* dv=(ble_device_t*)std::malloc(sizeof(ble_device_t));
		dv->addr=ea->BluetoothAddress;
		dv->addr_s[0]=_hex((uint8_t)(ea->BluetoothAddress>>44));
		dv->addr_s[1]=_hex((uint8_t)((ea->BluetoothAddress>>40)&0xf));
		dv->addr_s[2]=':';
		dv->addr_s[3]=_hex((uint8_t)((ea->BluetoothAddress>>36)&0xf));
		dv->addr_s[4]=_hex((uint8_t)((ea->BluetoothAddress>>32)&0xf));
		dv->addr_s[5]=':';
		dv->addr_s[6]=_hex((uint8_t)((ea->BluetoothAddress>>28)&0xf));
		dv->addr_s[7]=_hex((uint8_t)((ea->BluetoothAddress>>24)&0xf));
		dv->addr_s[8]=':';
		dv->addr_s[9]=_hex((uint8_t)((ea->BluetoothAddress>>20)&0xf));
		dv->addr_s[10]=_hex((uint8_t)((ea->BluetoothAddress>>16)&0xf));
		dv->addr_s[11]=':';
		dv->addr_s[12]=_hex((uint8_t)((ea->BluetoothAddress>>12)&0xf));
		dv->addr_s[13]=_hex((uint8_t)((ea->BluetoothAddress>>8)&0xf));
		dv->addr_s[14]=':';
		dv->addr_s[15]=_hex((uint8_t)((ea->BluetoothAddress>>4)&0xf));
		dv->addr_s[16]=_hex((uint8_t)(ea->BluetoothAddress&0xf));
		dv->addr_s[17]=0;
		dv->manufacturer_data.l=ea->Advertisement->ManufacturerData->Size;
		dv->manufacturer_data.data=(ea->Advertisement->ManufacturerData->Size?(ble_device_manufacturer_data_t*)std::malloc(ea->Advertisement->ManufacturerData->Size*sizeof(ble_device_manufacturer_data_t)):(ble_device_manufacturer_data_t*)(void*)0);
		dv->services.l=ea->Advertisement->ServiceUuids->Size;
		dv->services.uuids=(ea->Advertisement->ServiceUuids->Size?(ble_guid_t*)std::malloc(ea->Advertisement->ServiceUuids->Size*sizeof(ble_guid_t)):(ble_guid_t*)(void*)0);
		ble_device_manufacturer_data_t* mdp=dv->manufacturer_data.data;
		ble_guid_t* glp=dv->services.uuids;
		for (uint32_t i=0;i<ea->Advertisement->ManufacturerData->Size;i++){
			mdp->l=ea->Advertisement->ManufacturerData->GetAt(i)->Data->Length;
			mdp->id=ea->Advertisement->ManufacturerData->GetAt(i)->CompanyId;
			Microsoft::WRL::ComPtr<Windows::Storage::Streams::IBufferByteAccess> dt;
			reinterpret_cast<IInspectable*>(ea->Advertisement->ManufacturerData->GetAt(i)->Data)->QueryInterface(IID_PPV_ARGS(&dt));
			dt->Buffer((byte**)&(mdp->dt));
			mdp++;
		}
		for (uint32_t i=0;i<ea->Advertisement->ServiceUuids->Size;i++){
			Platform::String^ s=ea->Advertisement->ServiceUuids->GetAt(i).ToString();
			const char16* dt=s->Data();
			if (s->Length()!=38){
				printf("WRONG GUID LENGTH: %lu\n",s->Length());
			}
			else{
				uint32_t j=1;
				while (j<37){
					glp->s[j-1]=(char)(*(dt+j));
					if (j==9||j==14||j==19||j==24){
						j++;
						continue;
					}
					if (j<19){
						glp->a=(glp->a<<4)|_dehex((uint8_t)(char)(*(dt+j)));
					}
					else{
						glp->b=(glp->b<<4)|_dehex((uint8_t)(char)(*(dt+j)));
					}
					j++;
				}
				glp->s[36]=0;
				glp++;
			}
		}
		if ((*r=cb(dv,arg))){
			ble_w->Stop();
		}
		ReleaseMutex(mx);
	});
	ble_w->Start();
	while (tm){
		tm-=100;
		Sleep(100);
		if (ble_w->Status!=Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementWatcherStatus::Started){
			goto _end;
		}
	}
	ble_w->Stop();
_end:
	if (_all){
		std::free(_al);
	}
	CloseHandle(mx);
	return *r;
}



concurrency::task<ble_connected_device_t*> _connect_dev_cpp(ble_device_t* dv){
	ble_connected_device_t* o=(ble_connected_device_t*)std::malloc(sizeof(ble_connected_device_t));
	o->dv=dv;
	o->_dt=_d_ref.size()-1;
	Windows::Devices::Bluetooth::BluetoothLEDevice^ d=co_await Windows::Devices::Bluetooth::BluetoothLEDevice::FromBluetoothAddressAsync(dv->addr);
	_d_ref.push_back(d);
	Windows::Devices::Bluetooth::GenericAttributeProfile::GattDeviceServicesResult^ gsl=co_await d->GetGattServicesAsync();
	o->services.l=gsl->Services->Size;
	o->services.data=(ble_connected_device_service_t*)std::malloc(gsl->Services->Size*sizeof(ble_connected_device_service_t));
	ble_connected_device_service_t* sp=o->services.data;
	for (uint32_t i=0;i<o->services.l;i++){
		Windows::Devices::Bluetooth::GenericAttributeProfile::GattDeviceService^ gs=gsl->Services->GetAt(i);
		_gs_ref.push_back(gs);
		Platform::String^ s=gs->Uuid.ToString();
		const char16* dt=s->Data();
		if (s->Length()!=38){
			printf("WRONG GUID LENGTH: %lu\n",s->Length());
		}
		else{
			uint32_t j=1;
			while (j<37){
				sp->uuid.s[j-1]=(char)(*(dt+j));
				if (j==9||j==14||j==19||j==24){
					j++;
					continue;
				}
				if (j<19){
					sp->uuid.a=(sp->uuid.a<<4)|_dehex((uint8_t)(char)(*(dt+j)));
				}
				else{
					sp->uuid.b=(sp->uuid.b<<4)|_dehex((uint8_t)(char)(*(dt+j)));
				}
				j++;
			}
			sp->uuid.s[36]=0;
			sp->_dt=_gs_ref.size()-1;
			sp++;
		}
	}
	return o;
}



ble_connected_device_t* _connect_dev_wr_cpp(ble_device_t* dv){
	return _connect_dev_cpp(dv).get();
}



concurrency::task<void> _load_ch_cpp(ble_connected_device_service_t* s){
	Windows::Devices::Bluetooth::GenericAttributeProfile::GattCharacteristicsResult^ cl=co_await _gs_ref[s->_dt]->GetCharacteristicsAsync();
	s->characteristics.l=cl->Characteristics->Size;
	s->characteristics.data=(ble_connected_device_characteristics_t*)std::malloc(cl->Characteristics->Size*sizeof(ble_connected_device_characteristics_t));
	ble_connected_device_characteristics_t* cp=s->characteristics.data;
	for (uint32_t i=0;i<cl->Characteristics->Size;i++){
		Windows::Devices::Bluetooth::GenericAttributeProfile::GattCharacteristic^ c=cl->Characteristics->GetAt(i);
		Platform::String^ s=c->Uuid.ToString();
		const char16* dt=s->Data();
		if (s->Length()!=38){
			printf("WRONG GUID LENGTH: %lu\n",s->Length());
		}
		else{
			uint32_t j=1;
			while (j<37){
				cp->uuid.s[j-1]=(char)(*(dt+j));
				if (j==9||j==14||j==19||j==24){
					j++;
					continue;
				}
				if (j<19){
					cp->uuid.a=(cp->uuid.a<<4)|_dehex((uint8_t)(char)(*(dt+j)));
				}
				else{
					cp->uuid.b=(cp->uuid.b<<4)|_dehex((uint8_t)(char)(*(dt+j)));
				}
				j++;
			}
			cp->uuid.s[36]=0;
			cp->f=0;
			Windows::Devices::Bluetooth::GenericAttributeProfile::GattCharacteristicProperties gcp=c->CharacteristicProperties;
			uint32_t f=(uint32_t)gcp;
			if (f&(uint32_t)(Windows::Devices::Bluetooth::GenericAttributeProfile::GattCharacteristicProperties::Broadcast)){
				cp->f|=BLE_CHRACTERISTIC_FLAG_BROADCAST;
			}
			if (f&(uint32_t)(Windows::Devices::Bluetooth::GenericAttributeProfile::GattCharacteristicProperties::Read)){
				cp->f|=BLE_CHRACTERISTIC_FLAG_READ;
			}
			if (f&(uint32_t)(Windows::Devices::Bluetooth::GenericAttributeProfile::GattCharacteristicProperties::WriteWithoutResponse)){
				cp->f|=BLE_CHRACTERISTIC_FLAG_WRITEWITHOUTRESPONSE;
			}
			if (f&(uint32_t)(Windows::Devices::Bluetooth::GenericAttributeProfile::GattCharacteristicProperties::Write)){
				cp->f|=BLE_CHRACTERISTIC_FLAG_WRITE;
			}
			if (f&(uint32_t)(Windows::Devices::Bluetooth::GenericAttributeProfile::GattCharacteristicProperties::Notify)){
				cp->f|=BLE_CHRACTERISTIC_FLAG_NOTIFY;
			}
			if (f&(uint32_t)(Windows::Devices::Bluetooth::GenericAttributeProfile::GattCharacteristicProperties::Indicate)){
				cp->f|=BLE_CHRACTERISTIC_FLAG_INDICATE;
			}
			if (f&(uint32_t)(Windows::Devices::Bluetooth::GenericAttributeProfile::GattCharacteristicProperties::AuthenticatedSignedWrites)){
				cp->f|=BLE_CHRACTERISTIC_FLAG_AUTHENTICATEDSIGNEDWRITES;
			}
			if (f&(uint32_t)(Windows::Devices::Bluetooth::GenericAttributeProfile::GattCharacteristicProperties::ExtendedProperties)){
				cp->f|=BLE_CHRACTERISTIC_FLAG_EXTENDEDPROPERTIES;
			}
			if (f&(uint32_t)(Windows::Devices::Bluetooth::GenericAttributeProfile::GattCharacteristicProperties::ReliableWrites)){
				cp->f|=BLE_CHRACTERISTIC_FLAG_RELIABLEWRITES;
			}
			if (f&(uint32_t)(Windows::Devices::Bluetooth::GenericAttributeProfile::GattCharacteristicProperties::WritableAuxiliaries)){
				cp->f|=BLE_CHRACTERISTIC_FLAG_WRITABLEAUXILIARIES;
			}
			_c_ref.push_back(c);
			cp->_dt=_c_ref.size()-1;
			cp++;
		}
	}
}



void _load_ch_wr_cpp(ble_connected_device_service_t* s){
	_load_ch_cpp(s).get();
}



concurrency::task<void> _reg_cn_handle_cpp(ble_connected_device_characteristics_t* c,ble_characteristic_notification_t cb,void* arg){
	_c_ref[c->_dt]->ValueChanged+=ref new Windows::Foundation::TypedEventHandler<Windows::Devices::Bluetooth::GenericAttributeProfile::GattCharacteristic^,Windows::Devices::Bluetooth::GenericAttributeProfile::GattValueChangedEventArgs^>([=](Windows::Devices::Bluetooth::GenericAttributeProfile::GattCharacteristic^ gc,Windows::Devices::Bluetooth::GenericAttributeProfile::GattValueChangedEventArgs^ ea){
		ble_characteristic_notification_data_t dt={
			(uint64_t)(ea->Timestamp.UniversalTime/10000000-11644473600LL),
			ea->CharacteristicValue->Length
		};
		Microsoft::WRL::ComPtr<Windows::Storage::Streams::IBufferByteAccess> bf_dt;
		reinterpret_cast<IInspectable*>(ea->CharacteristicValue)->QueryInterface(IID_PPV_ARGS(&bf_dt));
		bf_dt->Buffer((byte**)&(dt.bf));
		cb(dt,arg);
	});
	co_await _c_ref[c->_dt]->WriteClientCharacteristicConfigurationDescriptorAsync(Windows::Devices::Bluetooth::GenericAttributeProfile::GattClientCharacteristicConfigurationDescriptorValue::Notify);
}



void _reg_cn_handle_wr_cpp(ble_connected_device_characteristics_t* c,ble_characteristic_notification_t cb,void* arg){
	_reg_cn_handle_cpp(c,cb,arg).get();
}



concurrency::task<void> _wr_c_cpp(ble_connected_device_characteristics_t* c,uint8_t* dt,uint32_t dtl){
	Windows::Storage::Streams::DataWriter^ wr=ref new Windows::Storage::Streams::DataWriter();
	wr->WriteBytes(ref new Platform::Array<byte>((byte*)dt,dtl));
	co_await _c_ref[c->_dt]->WriteValueAsync(wr->DetachBuffer(),Windows::Devices::Bluetooth::GenericAttributeProfile::GattWriteOption::WriteWithoutResponse);
}



void _wr_c_wr_cpp(ble_connected_device_characteristics_t* c,uint8_t* dt,uint32_t dtl){
	_wr_c_cpp(c,dt,dtl).get();
}



void _diconnect_dev_cpp(ble_connected_device_t* cdv){
	std::free(cdv);
}



void _free_dev_cpp(ble_device_t* dv){
	if (dv->manufacturer_data.l){
		std::free(dv->manufacturer_data.data);
	}
	if (dv->services.l){
		std::free(dv->services.uuids);
	}
	std::free(dv);
}



extern "C" void ble_lib_init(void){
	_init_lib_cpp();
}



extern "C" void* ble_lib_enum_devices(uint32_t tm,ble_device_found_t cb,void* arg){
	return _enum_dev_cpp(tm,cb,arg);
}



extern "C" ble_connected_device_t* ble_lib_connect_device(ble_device_t* dv){
	return _connect_dev_wr_cpp(dv);
}



extern "C" void ble_lib_load_characteristics(ble_connected_device_service_t* s){
	_load_ch_wr_cpp(s);
}



extern "C" void ble_lib_register_characteristic_notification(ble_connected_device_characteristics_t* c,ble_characteristic_notification_t cb,void* arg){
	_reg_cn_handle_wr_cpp(c,cb,arg);
}



extern "C" void ble_lib_write_characteristic(ble_connected_device_characteristics_t* c,uint8_t* dt,uint32_t dtl){
	_wr_c_wr_cpp(c,dt,dtl);
}



extern "C" void ble_lib_disconnect_device(ble_connected_device_t* cdv){
	_diconnect_dev_cpp(cdv);
}



extern "C" void ble_lib_free_device(ble_device_t* dv){
	_free_dev_cpp(dv);
}
