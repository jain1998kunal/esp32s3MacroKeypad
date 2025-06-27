
#include <Arduino.h>
#include "ButtonHandler.h"
#include "Dispatcher.h"
#include "USBHIDHandler.h"
#include "RGBLedHandler.h"
#include "display.h"
// #include "fileHandler.h"

DisplayHandler displayHandler;
ButtonHandler buttonHandler;
Dispatcher dispatcher;
USBHIDHandler usbHandler;
RGBLedHandler rgbLed;

/*
 * All this manual CS control is done in this code is coz for some reason i can not get the 7 instances of adafruit_ssd1306 class
 * to work simultaneously. only last instance which was initialized will work with the actual display. Sometimes there is guru
 * meditiation error, sometimes no error but display doesn't work.
 */
// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

void setup()
{
  Serial.begin(115200);

  // if (!LittleFS.begin(FORMAT_LITTLEFS_IF_FAILED))
  // {
  //   Serial.println("LittleFS Mount Failed");
  //   return;
  // }
  // Serial.println("SPIFFS-like write file to new path and delete it w/folders");

  pinMode(DISPLAY_PWR, OUTPUT);
  digitalWrite(DISPLAY_PWR, HIGH);
  // Initialize subsystems
  buttonHandler.begin();
  usbHandler.begin();
  rgbLed.begin();
  displayHandler.begin();

  // Connect button handler to dispatcher
  buttonHandler.setClickCallback([&](uint8_t index)
                                 { dispatcher.dispatch(index); });

  // USB HID path
  dispatcher.registerListener([&](uint8_t index)
                              { usbHandler.executeCommandForButton(index); });

  // RGB LED path
  dispatcher.registerListener([&](uint8_t index)
                              { rgbLed.toggle(); });

  for (int i = 0; i < 7; i++){
        delay(10);
        displayHandler.show(i);
    }
}

void loop()
{
  buttonHandler.update();
  usbHandler.update(); // If needed
}

