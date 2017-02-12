#ifndef PSV2CTRL_CTRL_H
#define PSV2CTRL_CTRL_H

#include <stdint.h>

#include "packet.h"

void
InitializeCtrl(void);

void
FillCtrlPacket(CtrlPacket *packet);

#endif