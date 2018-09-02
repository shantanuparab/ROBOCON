// Ensures this Header File is only included Once while compiling
#pragma once

#if defined(ARDUINO) && ARDUINO >= 100
#   include <Arduino.h>
#elif defined(WIRING)
#   include <Wiring.h>
#else
#   include <WProgram.h>
#   include <pins_arduino.h>
#endif

// This Ensures that the digitalRead and digitalWrites are faster
// Only use on AVR Platform
// On Teensy Boards, installed by Default
#if defined(__AVR__) && defined(ARDUINO) && ARDUINO >= 100
#   include <digitalWriteFast.h>
#endif

// This is the Object to be used for Motor Controller
// Sample Usage
// MotorController controller(5,6, HIGH);
// Go Reverse At PWM 30
// controller.setSpeed(-30);
struct SingleMotorController
{
 private:
   // Writing Pin would expand to this
   // For example
   // Pin pin;
   // and
   // uint8_t pin would be the same statements
   using Pin = byte;

   // The Direction Pin. Used to Control Direction
   const Pin m_direction;
   // The PWM Pin. Used to Control Speed
   const Pin m_pwm;

   // Provide the Value for Which this given Motor Goes Straight
   // Default Value is HIGH
   const uint8_t m_direction_straight;

 public:
   // p_pwm :- The PWM Pin
   // p_direction :- The Direction Pin
   // p_direction_straight :- The Value to Enter for Bot to Go Straight
   // Can be LOW or HIGH
   // Note:- Ensure that PWM Pin is actually Marked as PWM on Board
   // If Any Pin is At 0, it means it hasn't been initialised
   SingleMotorController(const Pin p_direction, const Pin p_pwm, const uint8_t p_direction_straight = HIGH) :
       m_direction{p_direction}, m_pwm{p_pwm}, m_direction_straight{p_direction_straight}
   {
      pinMode(m_direction, OUTPUT);
      pinMode(m_pwm, OUTPUT);

      // Initially Halt the Motor
      setSpeed(0);
   }

 private:
   inline uint8_t reverseDirection() const
   {
      // Simply Returns the Opposite of Straight Direction
      return (m_direction_straight == HIGH) ? LOW : HIGH;
   }
   inline uint8_t straightDirection() const
   {
      return m_direction_straight;
   }

 public:
   // Supply Given Speed Value to PWM Pin
   // Provide Negative Speed to reverse
   void setSpeed(const int16_t p_speed_val) const
   {
      // If Speed Value is Greater than Max Speed
      // Send Max Speed
      if (p_speed_val < -255)
         return setSpeed(-255);
      else if (p_speed_val > 255)
         return setSpeed(255);
      else
      {
      }
      if (p_speed_val < 0)
         digitalWrite(m_direction, reverseDirection());
      else if (p_speed_val > 0)
         digitalWrite(m_direction, straightDirection());

      // Send the Speed Value to PWM Pin
      analogWrite(m_pwm, p_speed_val);
   }
};