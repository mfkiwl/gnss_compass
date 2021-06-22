

#include <rtthread.h>
#include <rtdevice.h>
#include "drv_common.h"
#include "ubx.h"



void thread_led_entry(void* parameter);
void thread_rtk_entry(void* parameter);

void thread_rtk_put(uint8_t instance, uint8_t class, uint8_t id, uint8_t *buf, uint32_t len);

static rt_device_t dev_uart[2];
static ubx_t ubx[2];


static void ubx_cb0(uint8_t class, uint8_t id, uint8_t *buf, uint32_t len)
{
    thread_rtk_put(0, class, id, buf, len);
}

static void ubx_cb1(uint8_t class, uint8_t id, uint8_t *buf, uint32_t len)
{
    thread_rtk_put(1, class, id, buf, len);
}

    
static rt_err_t serial_rx_ind(rt_device_t dev, rt_size_t size)
{
    uint32_t len;
    int i;
    uint8_t buf[512];

    len = rt_device_read(dev, 0, buf, sizeof(buf));
        
    if(dev_uart[0] == dev)
    {
        for(i=0; i<len; i++)
        {
            ubx_protocol_parse(&ubx[0], buf[i]);
        }
    }
    else
    {
        for(i=0; i<len; i++)
        {
            ubx_protocol_parse(&ubx[1], buf[i]);
        }
    }
    return RT_EOK;
}

int main(void)
{

    printf("System Clock information");
    printf("SYSCLK_Frequency = %d\r\n", HAL_RCC_GetSysClockFreq());
    printf("HCLK_Frequency   = %d\r\n", HAL_RCC_GetHCLKFreq());
    printf("PCLK1_Frequency  = %d\r\n", HAL_RCC_GetPCLK1Freq());
    printf("PCLK2_Frequency  = %d\r\n", HAL_RCC_GetPCLK2Freq());
    

//    uint8_t buf[64] = "123123214123123";
    
    ubx[0].cb = ubx_cb0;
    ubx[1].cb = ubx_cb1;
    
    /* open uart */
    dev_uart[0] = rt_device_find("uart1"); /* TX:PA9  RX:PA10 */
    rt_device_open(dev_uart[0], RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_DMA_RX);
    rt_device_set_rx_indicate(dev_uart[0], serial_rx_ind);
    
    dev_uart[1] = rt_device_find("uart6"); /* TX:PC6  RX:PC7 */
    rt_device_open(dev_uart[1], RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_DMA_RX);
    rt_device_set_rx_indicate(dev_uart[1], serial_rx_ind);
    
    /* led thread */
    rt_thread_t tid;
    tid = rt_thread_create("led", thread_led_entry, RT_NULL, 512, 11, 20);
    rt_thread_startup(tid);
    
    /* rtk thread */
    tid = rt_thread_create("rtk", thread_rtk_entry, RT_NULL, 1024*1024, 10, 20);
    rt_thread_startup(tid);
    
    while(1)
    {
        rt_thread_mdelay(500);
    }
    return RT_EOK;
}

#include "stm32h7xx.h"
static int vtor_config(void)
{
    /* Vector Table Relocation in Internal QSPI_FLASH */
    SCB->VTOR = QSPI_BASE;
    return 0;
}
INIT_BOARD_EXPORT(vtor_config);


