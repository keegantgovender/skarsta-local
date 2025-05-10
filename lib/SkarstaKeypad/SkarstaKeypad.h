#pragma once

#include <Arduino.h>
#include "service/Service.h" // Explicit path
#include "Motor.h"
#include "Display.h"
#include "Keypad.h"
#include "Calibrator.h"

#ifdef __EEPROM__
#include <EEPROM.h>
#endif

#define SAVE_BUTTON_TIMEOUT 1500
#define RST_BUTTON_TIMEOUT 1500

class SkarstaKeypad : public Service {
private:
    Keypad keypad;
    Motor* motor = nullptr;
    Display* display = nullptr;
    Calibrator* calibrator = nullptr;
    char lastKey = NO_KEY;
    bool downPressed = false;
    bool upPressed = false;
    bool preset0Pressed = false;
    bool preset1Pressed = false;
    bool preset2Pressed = false;
    bool resetPressed = false;
    unsigned long resetPressStart = 0;
    unsigned long preset0PressStart = 0;
    unsigned long preset1PressStart = 0;
    unsigned long preset2PressStart = 0;

protected:
    bool stop_motor();

public:
    SkarstaKeypad(Motor* _motor, Display* _display, Calibrator* _calibrator,
                  const byte* rowPins, const byte* colPins);

    bool begin() override;

    void goto_preset(uint8_t i, unsigned int pos);

    void cycle() override;
};