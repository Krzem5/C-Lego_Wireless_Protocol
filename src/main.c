#include <lwp.h>
#include <lwp_drivers.h>
#include <stdio.h>



uint8_t dev_f_cb(lwp_device_data_t* dt){
	printf("%llx\n",dt->addr);
	return LWP_DEVICE_FOUND_ACCUMULATE_RETURN;
}



int main(int argc,const char** argv){
	lwp_device_list_t* dl=lwp_find_devices(5000,dev_f_cb);
	for (uint32_t i=0;i<dl->l;i++){
		printf("Device:\n  Address: %llx\n  Type: %.2hhx\n",(*(dl->dt+i))->addr,(*(dl->dt+i))->t);
	}
	lwp_device_t* d=*dl->dt;
	lwp_free_device_list(dl);
	lwp_wait_for_ports(d,1,50);
	lwp_driver_led_light_init(d,50);
	lwp_driver_led_light_set_color(d,50,0,255,0);
	for (uint8_t i=0;i<d->ports.l;i++){
		if ((d->ports.dt+i)->f&LWP_DEVICE_PORT_ATTACHED){
			printf("Port[%u]: %.2x %.4x\n",i,(d->ports.dt+i)->f,(d->ports.dt+i)->t);
		}
	}
	getchar();
	lwp_driver_led_light_set_color(d,50,255,0,255);
	return 0;
}
