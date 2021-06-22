#ifndef __UBX_PROTOCOL_H__
#define __UBX_PROTOCOL_H__


#include <stdint.h>
#include <stdbool.h>



#define UBX_CLASS_NAV (0X01)            //Navigation Results: Position, Speed, Time, Acceleration, Heading, DOP, SVs used
#define UBX_CLASS_NAV_ID_PVT (0X07)     //Navigation Position Velocity Time Solution
#define UBX_CLASS_RXM (0X02)            //Receiver Manager Messages: Satellite Status, RTC Status
#define UBX_CLASS_INF (0X04)            //Information Messages: Printf-Style Messages, with IDs such as Error, Warning, Notice
#define UBX_CLASS_ACK (0X05)            //Ack/Nack Messages: as replies to CFG Input Messages
#define UBX_CLASS_CFG (0X06)            //Configuration Input Messages: Set Dynamic Model, Set DOP Mask, Set Baud Rate, etc.
#define UBX_CLASS_UPD (0X09)            //Firmware Update Messages: Memory/Flash erase/write, Reboot, Flash identification, etc.
#define UBX_CLASS_MON (0X0A)            //Monitoring Messages: Comunication Status, CPU Load, Stack Usage, Task Status
#define UBX_CLASS_AID (0x0B)            //AssistNow Aiding Messages: Ephemeris, Almanac, other A-GPS data input
#define UBX_CLASS_TIM (0x0D)            //Timing Messages: Time Pulse Output, Timemark Results
#define UBX_CLASS_MGA (0X13)            //Multi-GNSS Assistance: Assistance data for various GNSS
#define UBX_CLASS_LOG (0X21)            //Logging Messages: Log creation, deletion, info and retrieval



#define UBX_BUF_MAX_SIZE    (128)



typedef struct
{
    uint8_t     stage;
    uint8_t     class;
    uint8_t     id;
    uint16_t    len;
    uint16_t    pos;
    uint8_t     buf[UBX_BUF_MAX_SIZE];
    uint8_t     cka_recv;
    uint8_t     ckb_recv;
    uint8_t     cka_cal;
    uint8_t     ckb_cal;
    void (*cb)  (uint8_t class, uint8_t id, uint8_t *buf, uint32_t len);
}ubx_t;


typedef struct
{
    uint32_t        iTOW;          // ms       GPS time of week of the navigation epoch. See the description of iTOW for 
    uint16_t        year;          // y        Year UTC
    uint8_t         month;         // month    Month, range 1..12 UTC
    uint8_t         day;           // d        Day of month, range 1..31 UTC
    uint8_t         hour;          // h        Hour of day, range 0..23 UTC
    uint8_t         min;           // min      Minute of hour, range 0..59 UTC
    uint8_t         sec;           // s        Seconds of minute, range 0..60 UTC
    int8_t          valid;         // -        Validity Flags (see graphic below)
    uint32_t        tAcc;          // ns       Time accuracy estimate UTC
    int32_t         nano;          // ns       Fraction of second, range -1e9..1e9 UTC
    uint8_t         fixType;       // -        GNSSfix Type, range 0..5
    int8_t          flags;         // -        Fix Status Flags (see graphic below)
    uint8_t         reserved1;     // -        Reserved
    uint8_t         numSV;         // -        Number of satellites used in Nav Solution
    int32_t         lon;           // deg      Longitude (1e-7)
    int32_t         lat;           // deg      Latitude (1e-7)
    int32_t         height;        // mm       Height above Ellipsoid
    int32_t         hMSL;          // mm       Height above mean sea level
    uint32_t        hAcc;          // mm       Horizontal Accuracy Estimate
    uint32_t        vAcc;          // mm       Vertical Accuracy Estimate
    int32_t         velN;          // mm/s     NED north velocity
    int32_t         velE;          // mm/s     NED east velocity
    int32_t         velD;          // mm/s     NED down velocity
    int32_t         gSpeed;        // mm/s     Ground Speed (2-D)
    int32_t         heading;       // deg      Heading of motion 2-D (1e-5)
    uint32_t        sAcc;          // mm/s     Speed Accuracy Estimate
    uint32_t        headingAcc;    // deg      Heading Accuracy Estimate (1e-5)
    uint16_t        pDOP;          // -        Position DOP (0.01)
    uint8_t         reserved2[6];  // -        Reserved
    int32_t         headVeh;
    uint8_t         reserved3[4];  // -        Reserved 
}ubx_pvt_t;




void ubx_protocol_parse(ubx_t *u, uint8_t ch);


#endif
