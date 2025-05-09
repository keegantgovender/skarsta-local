#pragma once

   #include <Arduino.h>
   #include "Motor.h"
   #include "config/configuration.h" // Updated path

   class MotorRelay : public Motor {
   private:
       const uint8_t pin_power;
       const uint8_t pin_direction;

       void _off() override;

       void _dir_cw() override;

       void _dir_ccw() override;

   public:
       MotorRelay(uint8_t pin1, uint8_t pin2, uint8_t pin_power, uint8_t pin_direction, uint8_t stop_diff,
                  uint8_t min_change, bool reverse);
   };