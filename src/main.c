#include <psp2/kernel/threadmgr.h>

#include "net.h"
#include "ctrl.h"
#include "display.h"
#include "usb.h"

static SceUID net_ready_mtx = 0;

int
com_thread(SceSize args, void *argp){
    InitializeCtrl();
    InitializeNet();
    InitializeUSB();

    sceKernelUnlockMutex(net_ready_mtx, 1);

    while(1){
        
    }
}

int
display_thread(SceSize args, void *argp){
    InitializeDisplay();

    StartDrawing();
    ClearScreen();
    Flip();

    unsigned int timeout = 1000;
    while(sceKernelLockMutex(net_ready_mtx, 1, &timeout));

    while(1) {
        StartDrawing();
        WriteString(2, 20, 0xFFFFFFFF, 1.0, "Monitoring Network...");
        Flip();
    }
}

int main(){

    SceUID com_tid = sceKernelCreateThread("communication", com_thread, 200, 0x1000, 0, 0, NULL);
    SceUID disp_tid = sceKernelCreateThread("display", display_thread, 201, 0x1000, 0, 0, NULL);

    net_ready_mtx = sceKernelCreateMutex("net_ready_mtx", 0, 0, 0);

    unsigned int timeout = 1000;
    sceKernelLockMutex(net_ready_mtx, 1, &timeout);

    sceKernelStartThread(com_tid, 0, NULL);
    sceKernelStartThread(disp_tid, 0, NULL);

    while(1);
}