#ifndef PSV2CTRL_PACKET_H
#define PSV2CTRL_PACKET_H

#include <stdint.h>
#include <psp2/types.h>

typedef struct {
    uint16_t x;
    uint16_t y;
} StickData;

typedef struct {
    uint16_t x;
    uint16_t y;
    uint8_t force;
    uint8_t id;
} TouchData;

typedef struct {
    uint32_t buttons;
    StickData left_stick;
    StickData right_stick;
    SceFQuaternion orientation;
    TouchData front_touch;
    TouchData back_touch;
} CtrlPacket;

#endif