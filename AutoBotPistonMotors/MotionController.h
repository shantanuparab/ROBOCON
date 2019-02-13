// Ensures this Header File is only included Once while compiling
#pragma once

// This is the Object to be used for Motor Controller
// Sample Usage
// SingleMotorController controller(5,6, HIGH);
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

 public:
   // The MotionDirection Pin. Used to Control MotionDirection
   Pin const Direction;
   // The PWM Pin. Used to Control Speed
   Pin const PWM;

 public:
   // p_pwm :- The PWM Pin
   // p_direction :- The MotionDirection Pin
   // Note:- Ensure that PWM Pin is actually Marked as PWM on Board
   // Note that the 0 value given to Pins by default
   // Is actually incorrect and needs to be changed
   SingleMotorController(Pin const p_direction, Pin const p_pwm) : Direction{p_direction}, PWM{p_pwm}
   {
      pinMode(Direction, OUTPUT);
      pinMode(PWM, OUTPUT);
      // Initially Halt the Motor
      setSpeed(0);
   }

 private:
   inline constexpr static uint8_t ReverseDirection()
   {
      return LOW;
   }
   inline constexpr static uint8_t StraightDirection()
   {
      return HIGH;
   }

 public:
   // Supply Given Speed Value to PWM Pin
   // Provide Negative Speed to reverse
   void setSpeed(int16_t const p_speed_val) const
   {
      digitalWrite(Direction, (p_speed_val > 0) ? StraightDirection() : ReverseDirection());

      // Constrain Speed within given range
      uint16_t const constrain_speed = constrain(abs(p_speed_val), 0 /*Min PWM*/, 256 /*Max PWM*/);
      uint16_t const opp_speed       = 256 - constrain_speed;
      // Send the Speed Value to PWM Pin
      analogWrite(PWM, opp_speed);
   }
};

struct MotionController
{
 private:
   // Front Left Motor
   SingleMotorController const m_fl_motor;
   // Front Right Motor
   SingleMotorController const m_fr_motor;
   // Back Left Motor
   SingleMotorController const m_bl_motor;
   // Back Right Motor
   SingleMotorController const m_br_motor;

 private:
   template <typename T>
   constexpr T MaxAbs(T const l, T const r) const
   {
      return max(abs(l), abs(r));
   }
   template <typename T>
   constexpr T MaxAbs(T const l, T const r, T const vals...) const
   {
      return MaxAbs(MaxAbs(l, r), vals);
   }

 public:
   byte MaxSpeedAllowed = 255;

 public:
   MotionController(SingleMotorController&& p_fl_motor,
                    SingleMotorController&& p_fr_motor,
                    SingleMotorController&& p_bl_motor,
                    SingleMotorController&& p_br_motor) :
       m_fl_motor{p_fl_motor},
       m_fr_motor{p_fr_motor}, m_bl_motor{p_bl_motor}, m_br_motor{p_br_motor}
   {
   }
   // Move Legs with given PWM values
   inline void MoveLegs(int16_t const flpwm, int16_t const frpwm, int16_t const blpwm, int16_t const brpwm) const
   {
      // Change the Signs Here
      // To Change Direction of
      // Revolution
      return MoveMotors(flpwm, frpwm, -blpwm, -brpwm);
   }

   // Directly supply PWM to the Given Motors
   inline void MoveMotors(int16_t flpwm, int16_t frpwm, int16_t blpwm, int16_t brpwm) const
   {
      // Find Absolute Max of All Values
      uint16_t const max_pwm_here = MaxAbs(flpwm, frpwm, blpwm, brpwm);
      if (max_pwm_here > MaxSpeedAllowed)
      {
         // Scale Down
         float const scale = (MaxSpeedAllowed / ((float)max_pwm_here));
         flpwm *= scale;
         frpwm *= scale;
         blpwm *= scale;
         brpwm *= scale;
      }
      return MoveMotorsDirect(-flpwm, -frpwm, -blpwm, -brpwm);
   }
   inline void
       MoveMotorsDirect(int16_t const flpwm, int16_t const frpwm, int16_t const blpwm, int16_t const brpwm) const
   {
      m_fl_motor.setSpeed(flpwm);
      m_fr_motor.setSpeed(frpwm);
      m_bl_motor.setSpeed(blpwm);
      m_br_motor.setSpeed(brpwm);
   }

   void Halt() const
   {
      // Set All Speed to 0
      // TO Halt Bot
      return MoveMotorsDirect(0, 0, 0, 0);
   }
};