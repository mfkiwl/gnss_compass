#include "rtthread.h"
#include "rtdevice.h"
#include "drv_common.h"



#define LED_PIN GET_PIN(I, 8)

void thread_led_entry(void* parameter)
{
    rt_pin_mode(LED_PIN, PIN_MODE_OUTPUT);
    while(1)
    {
        rt_thread_mdelay(500);
        rt_pin_write(LED_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(LED_PIN, PIN_LOW);
    }
}
