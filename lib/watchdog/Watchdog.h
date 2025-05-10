#pragma once

#include <vector>
#include "Service.h"
#include "Motor.h"
#include "Display.h"
#include "configuration.h" // Simplified path

class SafetyTrigger : public Trigger {
private:
    Motor *motor = nullptr;
    Display *display = nullptr;
    uint8_t error_count = 0;

    const ErrorType type;
    const uint8_t tolerance;
public:
    SafetyTrigger(Motor *m, Display *d, ErrorType t, uint8_t tolerance);

    bool trip(ErrorType t) override;

    void reset() override;
};

class Watchdog : public TimedService {
private:
    Motor *motor = nullptr;
    const uint16_t timeout;
    const uint16_t other_sleep;
    const uint8_t deadlock_change;
    const uint8_t other_change;
    elapsedSeconds lastRotation = 0;
    std::vector<Trigger *> triggers;

public:
    Watchdog(Motor *m, uint16_t timeout, uint8_t deadlock_change, uint8_t other_change, uint16_t other_sleep);

    Watchdog &add_trigger(Trigger *t);

    void cycle() override;
};