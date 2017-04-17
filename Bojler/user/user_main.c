/*
	The hello world demo
*/

//#include <mem.h>
//#include <ets_sys.h>
//#include <osapi.h>
#include "user_interface.h"
#include <os_type.h>
#include <gpio.h>
#include "driver/uart.h"
#include "httpclient.h"
#include "parson.h"

#define DELAY 5000 /* milliseconds */
LOCAL os_timer_t hello_timer;
extern int ets_uart_printf(const char *fmt, ...);

/******************************************************************************
 * FunctionName : user_rf_cal_sector_set
 * Description  : SDK just reversed 4 sectors, used for rf init data and paramters.
 *                We add this function to force users to set rf cal sector, since
 *                we don't know which sector is free in user's application.
 *                sector map for last several sectors : ABBBCDDD
 *                A : rf cal
 *                B : at parameters
 *                C : rf init data
 *                D : sdk parameters
 * Parameters   : none
 * Returns      : rf cal sector
*******************************************************************************/
uint32 ICACHE_FLASH_ATTR user_rf_cal_sector_set(void)
{
    enum flash_size_map size_map = system_get_flash_size_map();
    uint32 rf_cal_sec = 0;

    switch (size_map) {
        case FLASH_SIZE_4M_MAP_256_256:
            rf_cal_sec = 128 - 8;
            break;

        case FLASH_SIZE_8M_MAP_512_512:
            rf_cal_sec = 256 - 5;
            break;

        case FLASH_SIZE_16M_MAP_512_512:
        case FLASH_SIZE_16M_MAP_1024_1024:
            rf_cal_sec = 512 - 5;
            break;

        case FLASH_SIZE_32M_MAP_512_512:
        case FLASH_SIZE_32M_MAP_1024_1024:
            rf_cal_sec = 1024 - 5;
            break;

        default:
            rf_cal_sec = 0;
            break;
    }

    return rf_cal_sec;
}

void ICACHE_FLASH_ATTR user_rf_pre_init(void)
{

}

void ICACHE_FLASH_ATTR wifi_handle_event_cb(System_Event_t *evt)
{
	ets_uart_printf("event %x\n", evt->event);
	int res;
    switch (evt->event) {
         case EVENT_STAMODE_GOT_IP:
        	 ets_uart_printf("dobio ip adresu!\n");
       		JSON_Value *val = NULL;
       		JSON_Object *obj = NULL;
       		val = json_value_init_object();
       		obj = json_value_get_object(val);
       		if(json_object_set_string(obj, "rec", "nova lozinka") != JSONSuccess)
       			return;
       		if(json_object_set_number(obj, "broj", 12345) != JSONSuccess)
       			return;
       		char * bafer = json_serialize_to_string(val);
       		if(bafer == NULL){
       			ets_uart_printf("bafer = NULL!\n");
       		}
       		else
       		{
       			ets_uart_printf("poslato:\n%s\n", bafer);
       			http_post("http://btomic.000webhostapp.com/test_json_primi_remote.php", bafer, "", http_callback_example);
//         	 http_post("http://btomic.000webhostapp.com/test_json_primi_remote.php", "{\"broj\":\"333\",\"rec\":\"bilosta\"}", "", http_callback_example);
////        	 http_get("http://btomic.000webhostapp.com/vremena.php?sat1=148", "", http_callback_example);
//
       		}
              break;
         default:
             break;
 }
}


void ICACHE_FLASH_ATTR init_done() {
	ets_uart_printf("zavrsio init!\n");
}

void ICACHE_FLASH_ATTR user_init(void)
{
	wifi_set_event_handler_cb(wifi_handle_event_cb);
	system_init_done_cb(init_done);
}
