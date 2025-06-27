#include "RGBLedHandler.h"

RGBLedHandler::RGBLedHandler() {}

void RGBLedHandler::begin() {
    FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(10);
    leds[0] = CRGB::Black;
    FastLED.show();
}

void RGBLedHandler::toggle() {
    switch (state) {
        case 0: leds[0] = CRGB::Red; break;
        case 1: leds[0] = CRGB::Green; break;
        case 2: leds[0] = CRGB::Blue; break;
        default:
            leds[0] = CRGB::Black;
            state = 0;
            FastLED.show();
            return;
    }
    state++;
    FastLED.show();
}
