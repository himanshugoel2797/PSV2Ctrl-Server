#ifndef PSV2CTRL_NET_H
#define PSV2CTRL_NET_H

#include <stdint.h>

#include "packet.h"

void
InitializeNet(void);

int
NetIsInitialized(void);

void
GetCurrentIP(char ip[16]);

int
GetListeningPort(void);

void
GetDHCPName(char dst[256]);

int
PollNetConnection(void);

void
SendNetPacket(void *data, int sz);

void
CloseConnection(void);

#endif