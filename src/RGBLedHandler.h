#ifndef RGB_LED_HANDLER_H
#define RGB_LED_HANDLER_H

#include <FastLED.h>

#define NUM_LEDS 1
#define DATA_PIN 48

class RGBLedHandler {
public:
    RGBLedHandler();
    void begin();
    void toggle();

private:
    CRGB leds[NUM_LEDS];
    uint8_t state = 0;
};

#endif
