#include <lwp.h>
#include <ble_lib.h>
#include <windows.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>



#define LEGO_MANUFACTURER_ID 0x397
#define LEGO_MANUFACTURER_DATA_LEN 6
#define LEGO_MANUFACTURER_DATA_TYPE_INDEX 1
#define LEGO_SERVICE_UUID_A BLE_LIB_CREATE_UUID_A(0x00001623,0x1212,0xefde)
#define LEGO_SERVICE_UUID_B BLE_LIB_CREATE_UUID_B(0x1623,0x785feabcd123)
#define LEGO_CHARCTERISTIC_UUID_A BLE_LIB_CREATE_UUID_A(0x00001624,0x1212,0xefde)
#define LEGO_CHARCTERISTIC_UUID_B BLE_LIB_CREATE_UUID_B(0x1623,0x785feabcd123)
#define LEGO_CHARCTERISTIC_REQUIRED_FLAGS (BLE_CHRACTERISTIC_FLAG_WRITEWITHOUTRESPONSE|BLE_CHRACTERISTIC_FLAG_NOTIFY)



struct _f_dev_cb_arg{
	lwp_device_found_t cb;
	lwp_device_list_t* l;
};



void _c_notif_cb(ble_characteristic_notification_data_t dt,void* arg){
	lwp_device_t* d=(lwp_device_t*)arg;
	uint16_t l=*(dt.bf);
	if (l&128){
		dt.bf++;
		dt.l--;
		l=(*(dt.bf)<<7)|(l&(~128));
	}
	dt.bf+=2;
	uint8_t t=*(dt.bf);
	dt.bf++;
	dt.l-=3;
	WaitForSingleObject(d->_mx,INFINITE);
	switch (t){
		case 0x04:
			{
				uint8_t id=*(dt.bf);
				uint8_t ev=*(dt.bf+1);
				if (!ev){
					(d->ports.dt+id)->f&=~LWP_DEVICE_PORT_ATTACHED;
					printf("Detteched: {Port ID: %u}\n",id);
					break;
				}
				uint16_t pt=((uint16_t)*(dt.bf+3)<<8)|(*(dt.bf+2));
				if (d->ports.l<=id){
					d->ports.dt=realloc(d->ports.dt,(id+1)*sizeof(lwp_device_port_t));
					while (d->ports.l<=id){
						(d->ports.dt+d->ports.l)->f=0;
						d->ports.l++;
					}
				}
				(d->ports.dt+id)->_drv_dt=NULL;
				(d->ports.dt+id)->_cnt=0;
				(d->ports.dt+id)->f=LWP_DEVICE_PORT_ATTACHED;
				(d->ports.dt+id)->t=pt;
				if (ev==1){
					printf("Attached: {Port ID: %u, Type: %.4x, HR: %.8x, SR: %.8x}\n",id,pt,((uint32_t)*(dt.bf+7)<<24)|((uint32_t)*(dt.bf+6)<<16)|((uint32_t)*(dt.bf+5)<<8)|*(dt.bf+4),((uint32_t)*(dt.bf+11)<<24)|((uint32_t)*(dt.bf+10)<<16)|((uint32_t)*(dt.bf+9)<<8)|*(dt.bf+8));
					(d->ports.dt+id)->_cnt++;
					uint8_t bf[3]={0x21,id,0x01};
					lwp_send_raw_data(d,bf,sizeof(bf));
				}
				else{
					printf("Virtual Attached: {Port ID: %u, Type: %.4x, PortA: %u, PortB: %u}\n",id,pt,*(dt.bf+4),*(dt.bf+5));
					(d->ports.dt+id)->f|=LWP_DEVICE_PORT_VIRTUAL;
				}
				break;
			}
		case 0x43:
			{
				uint8_t id=*(dt.bf);
				if (id>=d->ports.l||!((d->ports.dt+id)->f&LWP_DEVICE_PORT_ATTACHED)){
					printf("Recived Port Info about Non-Existent Port (%u)!\n",id);
					break;
				}
				if (*(dt.bf+1)==1){
					printf("Port Mode: {Port ID: %u, Flags: %u, Mode #: %u, Input Modes: %x, Output Modes: %x}\n",id,*(dt.bf+2),*(dt.bf+3),((uint16_t)*(dt.bf+5)<<8)|(*(dt.bf+4)),((uint16_t)*(dt.bf+7)<<8)|(*(dt.bf+6)));
					(d->ports.dt+id)->f|=(*(dt.bf+2))<<2;
					if ((d->ports.dt+id)->f&LWP_DEVICE_PORT_COMBINABLE){
						uint8_t bf[3]={0x21,id,0x02};
						lwp_send_raw_data(d,bf,sizeof(bf));
					}
					(d->ports.dt+id)->modes.f|=SET_LWP_DEVICE_PORT_MODE_COUNT(*(dt.bf+3));
					for (uint8_t i=0;i<*(dt.bf+3);i++){
						(d->ports.dt+id)->modes.ml[i].f=0;
					}
					uint16_t v=((uint16_t)*(dt.bf+5)<<8)|(*(dt.bf+4));
					uint8_t bf[4]={0x22,id,0x00};
					while (v){
						if (v&1){
							(d->ports.dt+id)->_cnt+=7;
							(d->ports.dt+id)->modes.ml[bf[2]].f|=LWP_DEVICE_PORT_MODE_INPUT;
							bf[3]=0x00;
							lwp_send_raw_data(d,bf,sizeof(bf));
							bf[3]=0x01;
							lwp_send_raw_data(d,bf,sizeof(bf));
							bf[3]=0x02;
							lwp_send_raw_data(d,bf,sizeof(bf));
							bf[3]=0x03;
							lwp_send_raw_data(d,bf,sizeof(bf));
							bf[3]=0x04;
							lwp_send_raw_data(d,bf,sizeof(bf));
							bf[3]=0x05;
							lwp_send_raw_data(d,bf,sizeof(bf));
							bf[3]=0x80;
							lwp_send_raw_data(d,bf,sizeof(bf));
						}
						v>>=1;
						bf[2]++;
					}
					v=((uint16_t)*(dt.bf+7)<<8)|(*(dt.bf+6));
					bf[2]=0;
					while (v){
						if (v&1){
							(d->ports.dt+id)->_cnt+=7;
							(d->ports.dt+id)->modes.ml[bf[2]].f|=LWP_DEVICE_PORT_MODE_OUTPUT;
							bf[3]=0x00;
							lwp_send_raw_data(d,bf,sizeof(bf));
							bf[3]=0x01;
							lwp_send_raw_data(d,bf,sizeof(bf));
							bf[3]=0x02;
							lwp_send_raw_data(d,bf,sizeof(bf));
							bf[3]=0x03;
							lwp_send_raw_data(d,bf,sizeof(bf));
							bf[3]=0x04;
							lwp_send_raw_data(d,bf,sizeof(bf));
							bf[3]=0x05;
							lwp_send_raw_data(d,bf,sizeof(bf));
							bf[3]=0x80;
							lwp_send_raw_data(d,bf,sizeof(bf));
						}
						v>>=1;
						bf[2]++;
					}
				}
				else{
					uint8_t cc=0;
					dt.bf+=2;
					dt.l-=2;
					for (uint8_t i=0;i<8;i++){
						if (!dt.l){
							break;
						}
						uint16_t v=((uint16_t)*(dt.bf+1)<<8)|(*(dt.bf));
						if (!v){
							break;
						}
						printf("Port Mode Combinations: {Port ID: %u, Combination: %x}\n",id,v);
						(d->ports.dt+id)->modes.c[cc]=v;
						cc++;
						dt.bf+=2;
						dt.l-=2;
					}
					(d->ports.dt+id)->modes.f|=SET_LWP_DEVICE_PORT_COMBINATION_COUNT(cc);
				}
				(d->ports.dt+id)->_cnt--;
				break;
			}
		case 0x44:
			{
				uint8_t id=*(dt.bf);
				if (id>=d->ports.l||!((d->ports.dt+id)->f&LWP_DEVICE_PORT_ATTACHED)){
					printf("Recived Extended Port Info about Non-Existent Port (%u)!\n",id);
					break;
				}
				uint8_t m=*(dt.bf+1);
				uint8_t t=*(dt.bf+2);
				dt.bf+=3;
				dt.l-=3;
				switch (t){
					case 0x00:
						{
							uint8_t i=0;
							while (dt.l){
								*((d->ports.dt+id)->modes.ml[m].nm+i)=*(dt.bf+i);
								i++;
								dt.l--;
							}
							(d->ports.dt+id)->modes.ml[m].nm[i]=0;
							printf("Port Mode Name: {Port ID: %u, Mode: %u, Name: %s}\n",id,m,(d->ports.dt+id)->modes.ml[m].nm);
							break;
						}
					case 0x01:
						{
							(d->ports.dt+id)->modes.ml[m].rr.mn=*((float*)dt.bf);
							(d->ports.dt+id)->modes.ml[m].rr.mx=*((float*)(dt.bf+4));
							printf("Port Mode Raw Range: {Port ID: %u, Mode: %u, Min: %f, Max: %f}\n",id,m,(d->ports.dt+id)->modes.ml[m].rr.mn,(d->ports.dt+id)->modes.ml[m].rr.mx);
							break;
						}
					case 0x02:
						{
							(d->ports.dt+id)->modes.ml[m].pr.mn=*((float*)dt.bf);
							(d->ports.dt+id)->modes.ml[m].pr.mx=*((float*)(dt.bf+4));
							printf("Port Mode %% Range: {Port ID: %u, Mode: %u, Min: %f, Max: %f}\n",id,m,(d->ports.dt+id)->modes.ml[m].pr.mn,(d->ports.dt+id)->modes.ml[m].pr.mx);
							break;
						}
					case 0x03:
						{
							(d->ports.dt+id)->modes.ml[m].sr.mn=*((float*)dt.bf);
							(d->ports.dt+id)->modes.ml[m].sr.mx=*((float*)(dt.bf+4));
							printf("Port Mode SI Range: {Port ID: %u, Mode: %u, Min: %f, Max: %f}\n",id,m,(d->ports.dt+id)->modes.ml[m].sr.mn,(d->ports.dt+id)->modes.ml[m].sr.mx);
							break;
						}
					case 0x04:
						{
							uint8_t i=0;
							while (dt.l){
								*((d->ports.dt+id)->modes.ml[m].sm+i)=*(dt.bf+i);
								i++;
								dt.l--;
							}
							(d->ports.dt+id)->modes.ml[m].sm[i]=0;
							printf("Port Mode Symbol: {Port ID: %u, Mode: %u, Symbol: %s}\n",id,m,(d->ports.dt+id)->modes.ml[m].sm);
							break;
						}
					case 0x05:
						{
							uint8_t if_=*(dt.bf);
							uint8_t of=*(dt.bf+1);
							if (if_&4){
								(d->ports.dt+id)->modes.ml[m].f|=LWP_DEVICE_PORT_MODE_INPUT_MAPPING_DISCRETE;
							}
							if (if_&8){
								(d->ports.dt+id)->modes.ml[m].f|=LWP_DEVICE_PORT_MODE_INPUT_MAPPING_RELATIVE;
							}
							if (if_&16){
								(d->ports.dt+id)->modes.ml[m].f|=LWP_DEVICE_PORT_MODE_INPUT_MAPPING_ABSOLUTE;
							}
							if (if_&64){
								(d->ports.dt+id)->modes.ml[m].f|=LWP_DEVICE_PORT_MODE_INPUT_MAPPING_FUNCTIONAL_2_0;
							}
							if (if_&128){
								(d->ports.dt+id)->modes.ml[m].f|=LWP_DEVICE_PORT_MODE_INPUT_MAPPING_NULL;
							}
							if (of&4){
								(d->ports.dt+id)->modes.ml[m].f|=LWP_DEVICE_PORT_MODE_OUTPUT_MAPPING_DISCRETE;
							}
							if (of&8){
								(d->ports.dt+id)->modes.ml[m].f|=LWP_DEVICE_PORT_MODE_OUTPUT_MAPPING_RELATIVE;
							}
							if (of&16){
								(d->ports.dt+id)->modes.ml[m].f|=LWP_DEVICE_PORT_MODE_OUTPUT_MAPPING_ABSOLUTE;
							}
							if (of&64){
								(d->ports.dt+id)->modes.ml[m].f|=LWP_DEVICE_PORT_MODE_OUTPUT_MAPPING_FUNCTIONAL_2_0;
							}
							if (of&128){
								(d->ports.dt+id)->modes.ml[m].f|=LWP_DEVICE_PORT_MODE_OUTPUT_MAPPING_NULL;
							}
							printf("Port Mode Flags: {Port ID: %u, Mode: %u, Input Flags: %.2x, Output Flags: %.2x}\n",id,m,if_,of);
							break;
						}
					case 0x80:
						{
							uint8_t dt_c=*(dt.bf);
							uint8_t dt_t=*(dt.bf+1);
							uint8_t fg_c=*(dt.bf+2);
							uint8_t cd=*(dt.bf+3);
							if (!dt_t){
								(d->ports.dt+id)->modes.ml[m].f|=LWP_DEVICE_PORT_MODE_DATASET_TYPE_8BYTE;
							}
							else if (dt_t==1){
								(d->ports.dt+id)->modes.ml[m].f|=LWP_DEVICE_PORT_MODE_DATASET_TYPE_16BYTE;
							}
							else if (dt_t==2){
								(d->ports.dt+id)->modes.ml[m].f|=LWP_DEVICE_PORT_MODE_DATASET_TYPE_32BYTE;
							}
							else{
								(d->ports.dt+id)->modes.ml[m].f|=LWP_DEVICE_PORT_MODE_DATASET_TYPE_FLOAT;
							}
							printf("Port Mode Value Format: {Port ID: %u, Mode: %u, Dataset #: %u, Dataset Type: %u, Figure #: %u, Decimals: %u}\n",id,m,dt_c,dt_t,fg_c,cd);
							break;
						}
					default:
						printf("Unknown Extended Port Info Type '%u'!\n",t);
				}
				(d->ports.dt+id)->_cnt--;
				break;
			}
		default:
			printf("Unknown Message: [%u, %u] ",l,t);
			for (uint32_t i=0;i<dt.l;i++){
				printf("%.2hhx",*(dt.bf+i));
			}
			putchar('\n');
			break;
	}
	ReleaseMutex(d->_mx);
}



void* _f_dev_cb(ble_device_t* dv,void* _arg){
	struct _f_dev_cb_arg* arg=(struct _f_dev_cb_arg*)_arg;
	lwp_device_data_t dv_dt={
		dv->addr,
		LWP_DEVICE_TYPE_UNKNOWN
	};
	for (uint32_t i=0;i<dv->manufacturer_data.l;i++){
		if ((dv->manufacturer_data.data+i)->id==LEGO_MANUFACTURER_ID&&(dv->manufacturer_data.data+i)->l){
			for (uint32_t j=0;j<dv->services.l;j++){
				if ((dv->services.uuids+j)->a==LEGO_SERVICE_UUID_A&&(dv->services.uuids+j)->b==LEGO_SERVICE_UUID_B){
					dv_dt.dt=*((dv->manufacturer_data.data+i)->dt+LEGO_MANUFACTURER_DATA_TYPE_INDEX);
				}
			}
			if (dv_dt.dt!=LWP_DEVICE_TYPE_UNKNOWN){
				uint8_t r=arg->cb(&dv_dt);
				if (r!=LWP_DEVICE_FOUND_IGNORE){
					arg->l->l++;
					arg->l->dt=realloc(arg->l->dt,arg->l->l*sizeof(lwp_device_t*));
					*(arg->l->dt+arg->l->l-1)=malloc(sizeof(lwp_device_t));
					lwp_device_t* d=*(arg->l->dt+arg->l->l-1);
					d->addr=dv_dt.addr;
					d->t=dv_dt.dt;
					d->ports.l=0;
					d->ports.dt=malloc(1);
					ble_connected_device_t* cdv=ble_lib_connect_device(dv);
					d->_dt=cdv;
					d->_mx=CreateMutex(NULL,FALSE,NULL);
					for (uint32_t k=0;k<cdv->services.l;k++){
						if ((cdv->services.data+k)->uuid.a==LEGO_SERVICE_UUID_A&&(cdv->services.data+k)->uuid.b==LEGO_SERVICE_UUID_B){
							ble_lib_load_characteristics(cdv->services.data+k);
							for (uint32_t m=0;m<(cdv->services.data+k)->characteristics.l;m++){
								if (((cdv->services.data+k)->characteristics.data+m)->uuid.a==LEGO_CHARCTERISTIC_UUID_A&&((cdv->services.data+k)->characteristics.data+m)->uuid.b==LEGO_CHARCTERISTIC_UUID_B&&((cdv->services.data+k)->characteristics.data+m)->f&LEGO_CHARCTERISTIC_REQUIRED_FLAGS){
									ble_lib_register_characteristic_notification((cdv->services.data+k)->characteristics.data+m,_c_notif_cb,d);
									d->_dc=(cdv->services.data+k)->characteristics.data+m;
									return (void*)(uint64_t)(r==LWP_DEVICE_FOUND_ACCUMULATE?0:1);
								}
							}
						}
					}
					printf("Unable to Locate Service/Characteristic!\n");
					free(*(arg->l->dt+arg->l->l-1));
					ble_lib_disconnect_device(cdv);
					arg->l->l--;
				}
				ble_lib_free_device(dv);
				return (void*)0;
			}
		}
	}
	ble_lib_free_device(dv);
	return (void*)0;
}



lwp_device_list_t* lwp_find_devices(uint32_t tm,lwp_device_found_t cb){
	ble_lib_init();
	struct _f_dev_cb_arg arg={
		cb,
		malloc(sizeof(lwp_device_list_t))
	};
	arg.l->l=0;
	arg.l->dt=malloc(1);
	ble_lib_enum_devices(tm,_f_dev_cb,&arg);
	return arg.l;
}



void lwp_send_raw_data(lwp_device_t* d,uint8_t* dt,uint16_t dtl){
	uint16_t l=dtl+(dtl>=127?3:2);
	uint8_t* n_bf=malloc(l*sizeof(uint8_t));
	*n_bf=(uint8_t)(l&0x7f);
	uint16_t i=1;
	if (l>127){
		*(n_bf+1)=(uint8_t)(l>>7);
		i++;
	}
	*(n_bf+i)=0;
	i++;
	uint8_t off=(uint8_t)i;
	while (dtl){
		*(n_bf+i)=*(dt+i-off);
		i++;
		dtl--;
	}
	ble_lib_write_characteristic(d->_dc,n_bf,l);
	free(n_bf);
}



void lwp_wait_for_ports(lwp_device_t* d,uint8_t c,...){
	if (!c){
		for (uint8_t i=0;i<d->ports.l;i++){
			if ((d->ports.dt+i)->f&LWP_DEVICE_PORT_ATTACHED){
				while ((d->ports.dt+i)->_cnt){
					Sleep(10);
				}
			}
		}
		return;
	}
	va_list va;
	va_start(va,c);
	while (c){
		uint8_t p=va_arg(va,uint8_t);
		while (!((d->ports.dt+p)->f&LWP_DEVICE_PORT_ATTACHED)||(d->ports.dt+p)->_cnt){
			Sleep(10);
		}
		c--;
	}
	va_end(va);
}



lwp_device_port_t* lwp_get_port(lwp_device_t* d,uint8_t p){
	if (p<=d->ports.l){
		return NULL;
	}
	return d->ports.dt+p;
}



void lwp_free_device_list(lwp_device_list_t* dl){
	free(dl->dt);
	free(dl);
}
