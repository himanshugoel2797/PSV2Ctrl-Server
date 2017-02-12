#ifndef PSV2CTRL_NET_H
#define PSV2CTRL_NET_H

#include <stdint.h>

#include "packet.h"

void
InitializeNet(void);

void
PollConnection(void);

void
SendPacket(void);

void
CloseConnection(void);

#endif