#ifndef USBHIDHANDLER_H
#define USBHIDHANDLER_H

// #include <BleKeyboard.h>
#include "USB.h"
#include "USBHIDKeyboard.h"
#include "USBHIDConsumerControl.h"
#include "USBHIDVendor.h"

class USBHIDHandler {
public:
    void begin();
    void update();
    void sendKey(uint8_t index); // send keystroke based on index
    void executeCommandForButton(uint8_t buttonIndex);
    

private:
    static void usbEventCallback(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
    static bool customLayout;
    // BleKeyboard  bleKB{"Kunal's_KB", "Kunal", 100};
    // bool         bleConnected = false;
    // bool previousButtonState = HIGH;
    // const int buttonPin = 0;
};


#endif
