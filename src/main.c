#include <psp2/kernel/threadmgr.h>
#include <vita2d.h>

#include "net.h"
#include "ctrl.h"
#include "display.h"
#include "usb.h"

#define TEXT_COLOR RGBA8(0xff,0xff,0xff,0xff)
#define TEXT_SIZE 1.0f
#define TEXT_HEIGHT 50

enum {
    ConnectionType_None,
    ConnectionType_Network,
    ConnectionType_USB
};

static _Atomic int connection_type = 0;

int
com_thread(SceSize args, void *argp){

    while(connection_type == ConnectionType_None){
        if(PollNetConnection() == 1){
            connection_type = ConnectionType_Network;
        }
    }

    while(1){
        CtrlPacket packet;
        FillCtrlPacket(&packet);

        if(connection_type == ConnectionType_Network){
            SendNetPacket(&packet, sizeof(CtrlPacket));
        }
    }
}

int
display_thread(SceSize args, void *argp){
    InitializeDisplay();

    int cntr = 0;
    int clear_color = 0;

    //Wait for a connection
    while(connection_type == ConnectionType_None){
        StartDrawing();

        SetClearColor(clear_color);
        ClearScreen();
        clear_color++;

        int y = 20;

        if(USBIsInitialized())
            WriteString(2, y, TEXT_COLOR, TEXT_SIZE, "USB Initialization Error: %x", USBIsInitialized());
        else
            WriteString(2, y, TEXT_COLOR, TEXT_SIZE, "Monitoring USB...");

        y += TEXT_HEIGHT;

        if(NetIsInitialized())
            WriteString(2, y, TEXT_COLOR, TEXT_SIZE, "Network Initialization Error: %x", NetIsInitialized());
        else {
            char ip_addr[16];
            char hostname[256];

            GetCurrentIP(ip_addr);
            GetDHCPName(hostname);

            WriteString(2, y, TEXT_COLOR, TEXT_SIZE, "Monitoring Network at IP %s:%d or hostname \"%s\"...", ip_addr, GetListeningPort(), hostname);
        }

        y += TEXT_HEIGHT;

        if(cntr < 4){
            
            for(int i = 0, x = 2; i < cntr; i++, x += 16)
                WriteString(x, y, TEXT_COLOR, TEXT_SIZE, ".");

            cntr++;
        }else
            cntr = 0;

        Flip();
        sceKernelDelayThread(500000);
    }

    while(1){
        StartDrawing();
        SetClearColor(clear_color);
        ClearScreen();
        clear_color++;

        if(connection_type == ConnectionType_Network)
            WriteString(2, 20, TEXT_COLOR, TEXT_SIZE, "Network Connected!");
        else if(connection_type == ConnectionType_USB)
            WriteString(2, 20, TEXT_COLOR, TEXT_SIZE, "USB Connected!");

        Flip();
    }
}

int main(){

    InitializeCtrl();
    InitializeNet();
    InitializeUSB();

    SceUID com_tid = sceKernelCreateThread("communication", com_thread, 128, 0x1000, 0, 0, NULL);
    SceUID disp_tid = sceKernelCreateThread("display", display_thread, 128, 0x1000, 0, 0, NULL);

    sceKernelStartThread(com_tid, 0, NULL);
    sceKernelStartThread(disp_tid, 0, NULL);

    while(1);
}