#pragma once

   #include <Arduino.h>
   #include "Motor.h"
   #include "config/configuration.h" // Updated path

   class MotorBridge : public Motor {
   private:
       const uint8_t pin_r_en;
       const uint8_t pin_l_en;
       const uint8_t pin_r_pwm;
       const uint8_t pin_l_pwm;

       void _off() override;

       void _dir_cw() override;

       void _dir_ccw() override;

   public:
       MotorBridge(uint8_t pin1, uint8_t pin2, uint8_t pin_r_en, uint8_t pin_l_en, uint8_t pin_r_pwm, uint8_t pin_l_pwm,
                   uint8_t stop_diff, uint8_t min_change, bool reverse);
   };