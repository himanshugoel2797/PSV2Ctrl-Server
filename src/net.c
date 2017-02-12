#include "net.h"

#include <stdlib.h>
#include <string.h>

#include <psp2/sysmodule.h>
#include <psp2/net/netctl.h>
#include <psp2/net/net.h>

#define NET_INIT_SIZE (64 * 1024)
#define NET_PORT 56789

static int err_code = 0;
static SceNetCtlInfo net_info;
static SceNetSockaddrIn client_info;

static SceUID sock;
static int port = NET_PORT;
static int client_id = 0;

void
InitializeNet(void) {

    sceSysmoduleLoadModule(SCE_SYSMODULE_NET);
	err_code = sceNetShowNetstat();
	if (err_code == SCE_NET_ERROR_ENOTINIT) {
		SceNetInitParam initparam;
		initparam.memory = malloc(NET_INIT_SIZE);
		initparam.size = NET_INIT_SIZE;
		initparam.flags = 0;
		err_code = sceNetInit(&initparam);
	}
	
    err_code = sceNetCtlInit();
	err_code = sceNetCtlInetGetInfo(SCE_NETCTL_INFO_GET_IP_ADDRESS, &net_info);
    
	SceNetInAddr vita_addr;
	sceNetInetPton(SCE_NET_AF_INET, net_info.ip_address, &vita_addr);

    sock = sceNetSocket("net_coms", SCE_NET_AF_INET, SCE_NET_SOCK_STREAM, 0);

    uint32_t nbio = 1;
	err_code = sceNetSetsockopt(sock, SCE_NET_SOL_SOCKET, SCE_NET_SO_NBIO, &nbio, sizeof(nbio));

	SceNetSockaddrIn sock_addr;
	sock_addr.sin_family = SCE_NET_AF_INET;
	sock_addr.sin_addr.s_addr = sceNetHtonl(SCE_NET_INADDR_ANY);
	
    //Find an available port
    while(port <= 0xFFFF){
        sock_addr.sin_port = sceNetHtons(port);
	    if(sceNetBind(sock, (SceNetSockaddr *)&sock_addr, sizeof(SceNetSockaddrIn)) == SCE_NET_ERROR_EADDRINUSE)
            port++;
        else
            break;
    }
    if(port > 0xFFFF){
        err_code = -1;
        return;
    }

	sceNetListen(sock, 1);
}

int
NetIsInitialized(void){
    return err_code;
}

void
GetCurrentIP(char ip[]) {
    strcpy(ip, net_info.ip_address);
}

int
GetListeningPort(void) {
    return port;
}

void
GetDHCPName(char dst[256]) {
    strcpy(dst, net_info.dhcp_hostname);
}

int
PollNetConnection(void) {
	unsigned int addrlen = sizeof(client_info);
    int err_code = sceNetAccept(sock, (SceNetSockaddr *)&client_info, &addrlen);
    if(err_code == SCE_NET_ERROR_EWOULDBLOCK)
        return -1;
    else if(err_code >= 0){
        client_id = err_code;
        return 1;
    }

    return 0;
}

void
SendPacket(void* data, int sz) {
    sceNetSend(client_id, data, sz, 0);
}

void
CloseConnection(void) {
    sceNetSocketClose(sock);
}