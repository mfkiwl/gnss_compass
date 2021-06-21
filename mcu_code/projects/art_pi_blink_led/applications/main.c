/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-09-02     RT-Thread    first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include "drv_common.h"

#define LED_PIN GET_PIN(I, 8)

int main(void)
{
    rt_uint32_t count = 1;

    printf("System Clock information");
    printf("SYSCLK_Frequency = %d\r\n", HAL_RCC_GetSysClockFreq());
    printf("HCLK_Frequency   = %d\r\n", HAL_RCC_GetHCLKFreq());
    printf("PCLK1_Frequency  = %d\r\n", HAL_RCC_GetPCLK1Freq());
    printf("PCLK2_Frequency  = %d\r\n", HAL_RCC_GetPCLK2Freq());
    
    rt_pin_mode(LED_PIN, PIN_MODE_OUTPUT);
    uint8_t buf[64] = "123123214123123";
    
    rt_device_t dev;
    
    /* test uart 1 */
    dev = rt_device_find("uart1"); /* TX:PA9  RX:PA10 */
    if(dev)
    {
        printf("found uart1\r\n");
        rt_device_open(dev, RT_DEVICE_OFLAG_RDWR);
        rt_device_write(dev, 0, (const void*)buf, 10);
    }
    
    dev = rt_device_find("uart6"); /* TX:PC6  RX:PC7 */
    if(dev)
    {
        printf("found uart6\r\n");
        rt_device_open(dev, RT_DEVICE_OFLAG_RDWR);
        rt_device_write(dev, 0, (const void*)buf, 10);
    }
    
    
    while(count++)
    {
        rt_thread_mdelay(500);
        rt_pin_write(LED_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(LED_PIN, PIN_LOW);
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


