#include "ButtonHandler.h"

ButtonHandler::CallbackType ButtonHandler::clickCallback = nullptr;

ButtonHandler::ButtonHandler() {
    for (int i = 0; i < NUM_BUTTONS; ++i) {
        buttons[i] = new OneButton(BUTTON_PINS[i], true);
    }
}

void ButtonHandler::begin() {
    buttons[0]->attachClick(onClick0);
    buttons[1]->attachClick(onClick1);
    buttons[2]->attachClick(onClick2);
    buttons[3]->attachClick(onClick3);
    buttons[4]->attachClick(onClick4);
    buttons[5]->attachClick(onClick5);
    buttons[6]->attachClick(onClick6);
}

void ButtonHandler::update() {
    for (int i = 0; i < NUM_BUTTONS; ++i) {
        buttons[i]->tick();
    }
}

void ButtonHandler::setClickCallback(CallbackType cb) {
    clickCallback = cb;
}

void ButtonHandler::handleClick(uint8_t index) {
    // Serial.print("Button ");
    // Serial.print(index);
    // Serial.println(" clicked!");
    if (clickCallback) {
        clickCallback(index);
    }
}

// Static trampoline functions
void ButtonHandler::onClick0() { handleClick(0); }
void ButtonHandler::onClick1() { handleClick(1); }
void ButtonHandler::onClick2() { handleClick(2); }
void ButtonHandler::onClick3() { handleClick(3); }
void ButtonHandler::onClick4() { handleClick(4); }
void ButtonHandler::onClick5() { handleClick(5); }
void ButtonHandler::onClick6() { handleClick(6); }
