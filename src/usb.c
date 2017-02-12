#include "usb.h"

#include <psp2/usbserial.h>

static int err_code = 0;

void
InitializeUSB(void) {
    //TODO make this part work
    err_code = sceUsbSerialStart();
    
    if(!err_code)
        err_code = sceUsbSerialSetup(0);
}


int
USBIsInitialized(void){
    return err_code;
}