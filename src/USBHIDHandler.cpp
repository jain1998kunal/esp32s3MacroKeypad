#include "USBHIDHandler.h"
#include "Arduino.h"
#include <LittleFS.h>

static USBCDC USBSerial;
static USBHID HID;
static USBHIDKeyboard Keyboard;
static USBHIDConsumerControl ConsumerControl;
bool USBHIDHandler::customLayout = false;

void USBHIDHandler::usbEventCallback(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_base == ARDUINO_USB_EVENTS)
    {
        arduino_usb_event_data_t *data = (arduino_usb_event_data_t *)event_data;
        switch (event_id)
        {
        case ARDUINO_USB_STARTED_EVENT:
            // Serial.println("USB PLUGGED");
            break;
        case ARDUINO_USB_STOPPED_EVENT:
            // Serial.println("USB UNPLUGGED");
            break;
        case ARDUINO_USB_SUSPEND_EVENT:
            // Serial.printf("USB SUSPENDED: remote_wakeup_en: %u\n", data->suspend.remote_wakeup_en);
            break;
        case ARDUINO_USB_RESUME_EVENT:
            // Serial.println("USB RESUMED");
            break;

        default:
            break;
        }
    }
    else if (event_base == ARDUINO_USB_CDC_EVENTS)
    {
        arduino_usb_cdc_event_data_t *data = (arduino_usb_cdc_event_data_t *)event_data;
        switch (event_id)
        {
        case ARDUINO_USB_CDC_CONNECTED_EVENT:
            customLayout = true;
            // Serial.println("CDC CONNECTED");
            break;
        case ARDUINO_USB_CDC_DISCONNECTED_EVENT:
            customLayout = false;
            // Serial.println("CDC DISCONNECTED");
            break;
        case ARDUINO_USB_CDC_LINE_STATE_EVENT:
            // Serial.printf("CDC LINE STATE: dtr: %u, rts: %u\n", data->line_state.dtr, data->line_state.rts);
            break;
        case ARDUINO_USB_CDC_LINE_CODING_EVENT:
            // Serial.printf(
            //     "CDC LINE CODING: bit_rate: %lu, data_bits: %u, stop_bits: %u, parity: %u\n", data->line_coding.bit_rate, data->line_coding.data_bits,
            //     data->line_coding.stop_bits, data->line_coding.parity);
            break;
        case ARDUINO_USB_CDC_RX_EVENT:
            // Serial.printf("CDC RX [%u]:", data->rx.len);
            {
                uint8_t buf[data->rx.len];
                size_t len = USBSerial.read(buf, data->rx.len);
                // Serial.write(buf, len);
            }
            // Serial.println();
            break;
        case ARDUINO_USB_CDC_RX_OVERFLOW_EVENT:
            // Serial.printf("CDC RX Overflow of %d bytes", data->rx_overflow.dropped_bytes);
            break;

        default:
            break;
        }
        // } else if (event_base == ARDUINO_FIRMWARE_MSC_EVENTS) {
        //   arduino_firmware_msc_event_data_t *data = (arduino_firmware_msc_event_data_t *)event_data;
        //   switch (event_id) {
        //     case ARDUINO_FIRMWARE_MSC_START_EVENT: //Serial.println("MSC Update Start"); break;
        //     case ARDUINO_FIRMWARE_MSC_WRITE_EVENT:
        //       //Serial.printf("MSC Update Write %u bytes at offset %u\n", data->write.size, data->write.offset);
        //       Serial.print(".");
        //       break;
        //     case ARDUINO_FIRMWARE_MSC_END_EVENT:   Serial.printf("\nMSC Update End: %u bytes\n", data->end.size); break;
        //     case ARDUINO_FIRMWARE_MSC_ERROR_EVENT: Serial.printf("MSC Update ERROR! Progress: %u bytes\n", data->error.size); break;
        //     case ARDUINO_FIRMWARE_MSC_POWER_EVENT:
        //       Serial.printf("MSC Update Power: power: %u, start: %u, eject: %u\n", data->power.power_condition, data->power.start, data->power.load_eject);
        //       break;

        //     default: break;
        //   }
    }
    else if (event_base == ARDUINO_USB_HID_EVENTS)
    {
        arduino_usb_hid_event_data_t *data = (arduino_usb_hid_event_data_t *)event_data;
        switch (event_id)
        {
        case ARDUINO_USB_HID_SET_PROTOCOL_EVENT:
            // Serial.printf("HID SET PROTOCOL: %s\n", data->set_protocol.protocol ? "REPORT" : "BOOT");
            break;
        case ARDUINO_USB_HID_SET_IDLE_EVENT:
            // Serial.printf("HID SET IDLE: %u\n", data->set_idle.idle_rate);
            break;

        default:
            break;
        }
    }
    else if (event_base == ARDUINO_USB_HID_KEYBOARD_EVENTS)
    {
        arduino_usb_hid_keyboard_event_data_t *data = (arduino_usb_hid_keyboard_event_data_t *)event_data;
        switch (event_id)
        {
        case ARDUINO_USB_HID_KEYBOARD_LED_EVENT:
            // Serial.printf("HID KEYBOARD LED: NumLock:%u, CapsLock:%u, ScrollLock:%u\n", data->numlock, data->capslock, data->scrolllock);
            break;

        default:
            break;
        }
    }
    // else if (event_base == ARDUINO_USB_HID_VENDOR_EVENTS) {
    //   arduino_usb_hid_vendor_event_data_t *data = (arduino_usb_hid_vendor_event_data_t *)event_data;
    //   switch (event_id) {
    //     case ARDUINO_USB_HID_VENDOR_GET_FEATURE_EVENT:
    //       Serial.printf("HID VENDOR GET FEATURE: len:%u\n", data->len);
    //       for (uint16_t i = 0; i < data->len; i++) {
    //         Serial.write(data->buffer[i] ? data->buffer[i] : '.');
    //       }
    //       //Serial.println();
    //       break;
    //     case ARDUINO_USB_HID_VENDOR_SET_FEATURE_EVENT:
    //       Serial.printf("HID VENDOR SET FEATURE: len:%u\n", data->len);
    //       for (uint16_t i = 0; i < data->len; i++) {
    //         Serial.write(data->buffer[i] ? data->buffer[i] : '.');
    //       }
    //       //Serial.println();
    //       break;
    //     case ARDUINO_USB_HID_VENDOR_OUTPUT_EVENT:
    //       Serial.printf("HID VENDOR OUTPUT: len:%u\n", data->len);
    //       for (uint16_t i = 0; i < data->len; i++) {
    //         Serial.write(Vendor.read());
    //       }
    //       //Serial.println();
    //       break;

    //     default: break;
    //   }
    // }
}

void USBHIDHandler::begin()
{
    USB.onEvent(usbEventCallback);
    USBSerial.onEvent(usbEventCallback);
    HID.onEvent(usbEventCallback);
    Keyboard.onEvent(usbEventCallback);

    USBSerial.begin();
    Keyboard.begin();
    ConsumerControl.begin();
    USB.begin();
}

void USBHIDHandler::update()
{
    // Write custom code

    // int buttonState = digitalRead(buttonPin);
    // if (HID.ready() && buttonState != previousButtonState)
    // {
    //     previousButtonState = buttonState;

    //     if (buttonState == LOW)
    //     {
    //         USBSerial.println("Button Pressed");
    // Keyboard.pressRaw(HID_KEY_CAPS_LOCK);
    //         ConsumerControl.press(CONSUMER_CONTROL_VOLUME_INCREMENT);
    //     }
    //     else
    //     {
    //         USBSerial.println("Button Released");
    //         Keyboard.releaseRaw(HID_KEY_CAPS_LOCK);
    //         ConsumerControl.release();
    //     }
    //     delay(200);
    // }
}
/*
1 for  standard alphanumeric keypress
2 for modifier keys
3 for consumer control keys
4 for standard release
5 for standard release all
6 for consumer control release
*/

void USBHIDHandler::executeCommandForButton(uint8_t buttonIndex)
{
    File file = LittleFS.open("/commands.txt", "r");
    if (!file)
    {
        Serial.println("Failed to open commands file");
        return;
    }

    // Read file line-by-line until reaching the one for buttonIndex
    int currentLine = 0;
    String line;
    while (file.available())
    {
        line = file.readStringUntil('\n');
        if (currentLine == buttonIndex)
            break;
        currentLine++;
    }
    file.close();

    line.trim();
    if (line.length() == 0)
    {
        Serial.println("No command on this line");
        return;
    }

    // Split line by commas
    int fromIndex = 0;
    while (fromIndex < line.length())
    {
        int commaIndex = line.indexOf(',', fromIndex);
        String segment;
        if (commaIndex == -1)
        {
            segment = line.substring(fromIndex);
            fromIndex = line.length();
        }
        else
        {
            segment = line.substring(fromIndex, commaIndex);
            fromIndex = commaIndex + 1;
        }

        segment.trim();
        if (segment.length() == 0)
            continue;

        int sepIndex = segment.indexOf(' ');
        if (sepIndex == -1)
            continue;

        String cmdTypeStr = segment.substring(0, sepIndex);
        String argStr = segment.substring(sepIndex+1);
        // String keyCodeStr = segment.substring(sepIndex + 1);

        uint8_t cmdType = cmdTypeStr.toInt();

        if(cmdType == 7)
        {
            for (char ch : argStr)
            {
                Keyboard.write(ch);
            }
            continue;
        }
        uint8_t keyCode = (uint8_t)strtol(argStr.c_str(), NULL, 16);

        switch (cmdType)
        {
        case 1:
            Keyboard.pressRaw(keyCode);
            break;
        case 2:
            Keyboard.pressRaw(keyCode); // Modifier
            break;
        case 3:
            ConsumerControl.press(keyCode); // Requires ConsumerControl
            break;
        case 4:
            Keyboard.release(keyCode);
            break;
        case 5:
            Keyboard.releaseAll();
            break;
        case 6:
            ConsumerControl.release(); // if library supports this
            break;
        default:
            USBSerial.println("Unknown command type");
            break;
        }
    }
}

void USBHIDHandler::sendKey(uint8_t index)
{
    USBSerial.println("Button Pressed: " + String(index));

    if (customLayout)
    {
        USBSerial.println("custom Pressed: " + String(index));
    }
    else
    {
        switch (index)
        {
        case 0:
            Keyboard.pressRaw(HID_KEY_ALT_LEFT);
            Keyboard.pressRaw(HID_KEY_TAB);
            Keyboard.releaseAll();
            break;
        case 1:
            Keyboard.pressRaw(HID_KEY_GUI_LEFT);
            Keyboard.pressRaw(HID_KEY_ARROW_LEFT);
            Keyboard.releaseAll();
            break;
        case 2:
            ConsumerControl.press(CONSUMER_CONTROL_VOLUME_INCREMENT);
            ConsumerControl.release();
            break;
        case 3:
            ConsumerControl.press(CONSUMER_CONTROL_VOLUME_DECREMENT);
            ConsumerControl.release();
            break;
        case 4:
            Keyboard.pressRaw(HID_KEY_GUI_LEFT);
            Keyboard.pressRaw(HID_KEY_E);
            Keyboard.releaseAll();
            break;
        case 5:
            Keyboard.pressRaw(HID_KEY_GUI_LEFT);
            Keyboard.pressRaw(HID_KEY_ARROW_RIGHT);
            Keyboard.releaseAll();
            break;
        case 6:
            USBSerial.println("case Button Pressed: " + String(index));
            break;
        default:
            USBSerial.println("default Button Pressed: " + String(index));
            break;
        }
    }
}
