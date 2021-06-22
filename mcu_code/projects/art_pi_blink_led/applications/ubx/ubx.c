#include "ubx.h"


typedef enum
{
    UBX_MESSAGE_STAGE_HEADER0,   
    UBX_MESSAGE_STAGE_HEADER1,   
    UBX_MESSAGE_STAGE_CLASS,    //1 byte
    UBX_MESSAGE_STAGE_ID,       //1 byte
    UBX_MESSAGE_STAGE_LENLOW,
    UBX_MESSAGE_STAGE_LENHIGH,
    UBX_MESSAGE_STAGE_PAYLOAD,
    UBX_MESSAGE_STAGE_CKA,
    UBX_MESSAGE_STAGE_CKB,
}ubx_message_stage_t;



static void ubx_cal_chksum(uint8_t *buf, uint32_t len, uint8_t *cka, uint8_t *ckb)
{
    uint32_t i;
    for (i=0; i<len; i++)
    {
        cka[0] += buf[i];
        ckb[0] += cka[0];
    }
}

void ubx_protocol_parse(ubx_t *u, uint8_t ch)
{
    switch (u->stage)
    {
        case UBX_MESSAGE_STAGE_HEADER0:
            if(ch == 0xB5)
            {
                u->stage = UBX_MESSAGE_STAGE_HEADER1;
            }
            break;
        case UBX_MESSAGE_STAGE_HEADER1:
            if(ch == 0x62)
                u->stage = UBX_MESSAGE_STAGE_CLASS;
            else
                u->stage = UBX_MESSAGE_STAGE_HEADER0;
            break;
        case UBX_MESSAGE_STAGE_CLASS:
            u->class = ch;
            u->stage = UBX_MESSAGE_STAGE_ID;
            u->cka_cal = 0;
            u->ckb_cal = 0;
            ubx_cal_chksum(&u->class, 1, &u->cka_cal, &u->ckb_cal);
            break;
        case UBX_MESSAGE_STAGE_ID:
            u->id = ch;
            u->stage = UBX_MESSAGE_STAGE_LENLOW;
            ubx_cal_chksum(&u->id, 1, &u->cka_cal, &u->ckb_cal);
            break;
        case UBX_MESSAGE_STAGE_LENLOW:
            u->len = ch;
            ubx_cal_chksum(&ch, 1, &u->cka_cal, &u->ckb_cal);
            u->stage = UBX_MESSAGE_STAGE_LENHIGH;
            break;
        case UBX_MESSAGE_STAGE_LENHIGH:
            u->len |= (ch<<8);
            ubx_cal_chksum(&ch, 1, &u->cka_cal, &u->ckb_cal);
            u->stage = UBX_MESSAGE_STAGE_PAYLOAD;
            u->pos = 0;
            break;
        case UBX_MESSAGE_STAGE_PAYLOAD:
            u->buf[u->pos++] = ch;
            
            if(u->pos >= u->len)
            {
                ubx_cal_chksum(u->buf, u->len, &u->cka_cal, &u->ckb_cal);
                u->stage = UBX_MESSAGE_STAGE_CKA;
            }
            break;
        case UBX_MESSAGE_STAGE_CKA:
            u->cka_recv = ch;
            u->stage = UBX_MESSAGE_STAGE_CKB;
            break;
        case UBX_MESSAGE_STAGE_CKB:
            u->ckb_recv = ch;
            
            if((u->cka_cal == u->cka_recv) && (u->ckb_cal == u->ckb_recv) && (u->cb))
            {
                u->cb(u->class, u->id, u->buf, u->len);
            }
            
            u->stage = UBX_MESSAGE_STAGE_HEADER0;
            break;
        default:
            break;
    }
}
