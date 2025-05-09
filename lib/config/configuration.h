#pragma once

// Pin definitions for sensors (rotary encoder)
#define SENSOR_PIN0 2
#define SENSOR_PIN1 3

// Pin definitions for TM1637 display
#define DISPLAY_PIN_CLK     4
#define DISPLAY_PIN_DIO     5
#define FADE_TIMEOUT        60000

// 4x4 Matrix Keypad Pins (4 rows, 4 columns)
#define ROW1_PIN 6
#define ROW2_PIN 11
#define ROW3_PIN 12
#define ROW4_PIN 13
#define COL1_PIN A0  // D14
#define COL2_PIN A1  // D15
#define COL3_PIN A2  // D16
#define COL4_PIN A3  // D17

/**
 * __WATCHDOG__         if watchdog is enabled
 * WATCHDOG_TIMEOUT     represents timeout in millis for watchdog to perform its checks
 * WATCHDOG_TOLERANCE   represents number of failed checks in a row needed to trigger an error,
 *                      so the reaction time will be (WATCHDOG_TIMEOUT * WATCHDOG_TOLERANCE) ms
 *
 * WATCHDOG_DEADLOCK_CHANGE     represents rotation steps threshold for detecting if motor is stuck and cannot move
 *                              so if rotation change is greater than WATCHDOG_DEADLOCK_CHANGE for WATCHDOG_TOLERANCE times, Error 1 will be triggered
 * WATCHDOG_OTHER_CHANGE        represents rotation steps threshold for detecting if motor was started mainly due to programming errors,
 *                              so if rotation change is greater than WATCHDOG_OTHER_CHANGE for WATCHDOG_TOLERANCE times, Error 2 will be triggered
 * WATCHDOG_OTHER_SLEEP         represents timeout in seconds after which Error 2 check is not performed, 0 value means no timeout is applied
 */
#ifdef __USENSOR__
    #define WATCHDOG_TIMEOUT        300
#else
    #define WATCHDOG_TIMEOUT        100
#endif
#define WATCHDOG_DEADLOCK_CHANGE    6
#define WATCHDOG_OTHER_CHANGE       50  // Increased to prevent false positives
#ifdef __H_BRIDGE_MOTOR__
    #define WATCHDOG_TOLERANCE      10  // Increased to require more failed checks
#else
    #define WATCHDOG_TOLERANCE      5   // Increased to require more failed checks
#endif
#ifndef WATCHDOG_OTHER_SLEEP
    #define WATCHDOG_OTHER_SLEEP    1000  // Delay Error 2 check for 1 second
#endif

// Motor configuration
#define STOP_POS_DIFF       1   // Offset distance for preset end stop
#define MINIMUM_POS_CHANGE  8   // Minimum distance for presets to be invoked

// H-bridge pin definitions
#define R_EN    7
#define L_EN    8
#define R_PWM   9
#define L_PWM   10

#define REVERSE_POLARITY   false
#define DIRECTION_RELAY     6
#define POWER_RELAY         7

// Serial configuration (if used)
#define SERIAL_COM_RX   12      // Ignored if HW serial is used
#define SERIAL_COM_TX   11      // Ignored if HW serial is used
#define SERIAL_COM_BAUD 9600

// Define experimental features
#define __EEPROM__
#define __WATCHDOG__
// #define __EXPERIMENTAL__