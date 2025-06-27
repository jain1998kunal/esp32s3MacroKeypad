#ifndef BUTTON_HANDLER_H
#define BUTTON_HANDLER_H

#include <Arduino.h>
#include <OneButton.h>

#define BUTTON1 5
#define BUTTON2 3
#define BUTTON3 1
#define BUTTON4 2
#define BUTTON5 4
#define BUTTON6 6
#define BUTTON7 7

#define NUM_BUTTONS 7

const uint8_t BUTTON_PINS[NUM_BUTTONS] = {
    BUTTON1, BUTTON2, BUTTON3, BUTTON4, BUTTON5, BUTTON6, BUTTON7
};

class ButtonHandler {
public:
    using CallbackType = std::function<void(uint8_t)>;

    ButtonHandler();
    void begin();
    void update();
    void setClickCallback(CallbackType cb);

private:
    OneButton* buttons[NUM_BUTTONS];
    static CallbackType clickCallback;

    static void onClick0();
    static void onClick1();
    static void onClick2();
    static void onClick3();
    static void onClick4();
    static void onClick5();
    static void onClick6();

    static void handleClick(uint8_t index);
};

#endif
