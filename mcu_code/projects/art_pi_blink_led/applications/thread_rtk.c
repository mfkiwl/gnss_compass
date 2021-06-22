#include "rtthread.h"
#include "rtdevice.h"
#include "drv_common.h"
#include <stdio.h>

static rt_mq_t mq;

typedef struct
{
    uint32_t instance; /* which M8T? */
    uint8_t class;
    uint8_t id;
    uint16_t len;
    uint8_t buf[512];
}ubx_data_t;

void thread_rtk_put(uint8_t instance, uint8_t class, uint8_t id, uint8_t *buf, uint32_t len)
{
    ubx_data_t data;
    
    rt_memcpy(data.buf, buf, len);
    data.class = class;
    data.id = id;
    data.instance = instance;
    data.len = len;
    rt_mq_send(mq, &data, sizeof(ubx_data_t));
}

void thread_rtk_entry(void* parameter)
{
    
    mq = rt_mq_create("mq", sizeof(ubx_data_t), 10, RT_IPC_FLAG_FIFO);
    
    ubx_data_t data;
    
    while(1)
    {
        if(rt_mq_recv(mq, &data, sizeof(ubx_data_t), RT_WAITING_FOREVER) == RT_EOK)
        {
            printf("M8T[%d], class:%d  id:0x%X  len:%d\r\n", data.instance, data.class, data.id, data.len);
        }

    }
}
