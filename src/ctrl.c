#include <psp2/ctrl.h>
#include <psp2/motion.h>
#include <psp2/touch.h>

#include "ctrl.h"

void
InitializeCtrl(void) {

    //Setup controls
    sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG_WIDE);

    //Setup motion sensor
    sceMotionReset();
    sceMotionStartSampling();

    //Setup touch controls
    sceTouchEnableTouchForce(SCE_TOUCH_PORT_FRONT);
    sceTouchEnableTouchForce(SCE_TOUCH_PORT_BACK);
}

static void
GetKeyData(CtrlPacket *packet) {
    SceCtrlData ctrl_data;
    sceCtrlPeekBufferPositive(0, &ctrl_data, 1);

    packet->buttons = ctrl_data.buttons;

    packet->left_stick.x = ctrl_data.lx;
    packet->left_stick.y = ctrl_data.ly;
    
    packet->right_stick.x = ctrl_data.rx;
    packet->right_stick.y = ctrl_data.ry;
}

static void
GetAccel(CtrlPacket *packet) {
    SceMotionState mot_state;
    sceMotionGetState(&mot_state);

    packet->orientation = mot_state.deviceQuat;
}

static void
EncodeTouchData(TouchData *data, const SceTouchData *src){
    data->x = 0xFFFF;
    data->y = 0xFFFF;

    if(src->reportNum > 0){
        data->x = src->report[0].x;
        data->y = src->report[0].y;
        data->force = src->report[0].force;
        data->id = src->report[0].id;
    }
}

static void
GetTouchData(CtrlPacket *packet) {
    SceTouchData touch_data_fr, touch_data_bk;
    sceTouchPeek(SCE_TOUCH_PORT_FRONT, &touch_data_fr, 1);
    sceTouchPeek(SCE_TOUCH_PORT_BACK, &touch_data_bk, 1);

    EncodeTouchData(&packet->front_touch, &touch_data_fr);
    EncodeTouchData(&packet->back_touch, &touch_data_bk);
}

void
FillCtrlPacket(CtrlPacket *packet) {
    GetKeyData(packet);
    GetAccel(packet);
    GetTouchData(packet);
}