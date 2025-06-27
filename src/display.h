#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <FS.h>
#include <LittleFS.h>          // change to SPIFFS if you like
#include <Adafruit_SSD1306.h>  // 128×64 I²C OLED

// #define SCREEN_WIDTH 128 // OLED display width, in pixels
// #define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_CLK 33  // D0
#define OLED_MOSI 35 // D1
#define OLED_RST 37
#define OLED_DC 39
#define OLED_CS 45 // dummy cs pin
#define OLED_CS1 47
#define OLED_CS2 18
#define OLED_CS3 34
#define OLED_CS4 36
#define OLED_CS5 38
#define OLED_CS6 40
#define OLED_CS7 41
#define OLED_CS_ALL 7
#define DISPLAY_PWR 21

class DisplayHandler {
public:
    bool begin(); // call once in setup()
    void show(uint8_t index);                               // call from dispatcher

private:
    static constexpr uint8_t WIDTH  = 128;
    static constexpr uint8_t HEIGHT = 64;
    static constexpr char FILE_TEMPLATE[] PROGMEM = "/page%u";
    const uint8_t oledPins[7] = {OLED_CS1, OLED_CS2, OLED_CS3, OLED_CS4, OLED_CS5, OLED_CS6, OLED_CS7};
    const int8_t numOleds = 7;

    // helpers
    void handleCSPin(uint8_t index, bool state);
    bool drawBMP(File &f);
    bool drawTXT(File &f);
    void centerText(const String &line, int16_t y);
    void displaytext(uint8_t displayIdx, uint8_t x, uint8_t y, uint8_t size, const char data[]);

    Adafruit_SSD1306 _oled{WIDTH, HEIGHT, &SPI, OLED_DC, OLED_RST, OLED_CS};
};

#endif