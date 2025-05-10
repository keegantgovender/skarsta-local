#include <vector>
#include <Arduino.h>
#include "configuration.h" // Simplified path
#include "Service.h"
#include "Motor.h"
#include "Display.h"
#include "Calibrator.h"
#include "SkarstaKeypad/SkarstaKeypad.h"
#include "Watchdog.h"

#ifdef __H_BRIDGE_MOTOR__
#include "MotorBridge.h"
#else
#include "MotorRelay.h"
#endif
#if defined(__SERIAL_COM__) || defined(__SERIAL_COM_HW__)
#include "SerialCom.h"
#endif

#ifdef __EEPROM__
#include <EEPROM.h>
#endif

#ifdef __EEPROM__

bool eeprom_valid() {
    for (uint16_t i = ADDRESS_MODE + 4 * sizeof(unsigned int); i < EEPROM.length(); ++i) {
        if (EEPROM[i] != 0) {
            return false;
        }
    }
    return true;
}

void eeprom_reset() {
    LOG("reset eeprom");
    for (unsigned int i = 0; i < EEPROM.length(); ++i)
        updateEEPROM(i, 0);
}

#endif

#ifdef __H_BRIDGE_MOTOR__
MotorBridge motor(SENSOR_PIN0, SENSOR_PIN1, R_EN, L_EN, R_PWM, L_PWM, STOP_POS_DIFF, MINIMUM_POS_CHANGE,
                  REVERSE_POLARITY);
#else
MotorRelay motor(SENSOR_PIN0, SENSOR_PIN1, POWER_RELAY, DIRECTION_RELAY, STOP_POS_DIFF, MINIMUM_POS_CHANGE,
                 REVERSE_POLARITY);
#endif
Display display(DISPLAY_PIN_CLK, DISPLAY_PIN_DIO, FADE_TIMEOUT);
Watchdog watchdog(&motor, WATCHDOG_TIMEOUT, WATCHDOG_DEADLOCK_CHANGE, WATCHDOG_OTHER_CHANGE, WATCHDOG_OTHER_SLEEP);
Calibrator calibrator(&motor);

// Define row and column pins for the 4x4 keypad
const byte rowPins[4] = {ROW1_PIN, ROW2_PIN, ROW3_PIN, ROW4_PIN};
const byte colPins[4] = {COL1_PIN, COL2_PIN, COL3_PIN, COL4_PIN};
SkarstaKeypad keypad(&motor, &display, &calibrator, rowPins, colPins);

#if defined(__SERIAL_COM__) || defined(__SERIAL_COM_HW__)
SerialCom serialCom(&motor, &calibrator, SERIAL_COM_BAUD, SERIAL_COM_RX, SERIAL_COM_TX);
#endif

std::vector<Service *> services = {
        &calibrator,
        &watchdog,
        &keypad,
        &motor,
        &display,
};

#ifdef __WATCHDOG__
SafetyTrigger stallTrigger(&motor, &display, STOPPED, WATCHDOG_TOLERANCE);
SafetyTrigger runawayTrigger(&motor, &display, STARTED, WATCHDOG_TOLERANCE);
#endif

void setup() {
    LOG_INIT(Serial.begin(9600), &Serial);
    LOG("starting");
#ifdef __EEPROM__
    if (!eeprom_valid())
        eeprom_reset();
#endif
    watchdog.add_trigger(&calibrator);
#ifdef __WATCHDOG__
    watchdog.add_trigger(&stallTrigger);
    watchdog.add_trigger(&runawayTrigger);
#endif
    bool failed = false;
    for (const auto &service : services) {
        bool result = service->begin();
        if (!result) {
            LOG("Service failed to initialize");
        }
        failed |= !result;
    }

    if (failed) {
        LOG("Initialization failed, disabling services with cause: ");
        LOG(INIT);
        for (const auto &service : services)
            service->disable(INIT);
    } else {
        LOG("All services initialized successfully");
    }
    LOG("started");
}

void loop() {
    for (const auto &service : services)
        service->cycle();
}