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

   // The MotionDirection Pin. Used to Control MotionDirection
   Pin const m_direction;
   // The PWM Pin. Used to Control Speed
   Pin const m_pwm;

 public:
   // p_pwm :- The PWM Pin
   // p_direction :- The MotionDirection Pin
   // Note:- Ensure that PWM Pin is actually Marked as PWM on Board
   // Note that the 0 value given to Pins by default
   // Is actually incorrect and needs to be changed
   SingleMotorController(Pin const p_direction, Pin const p_pwm) : m_direction{p_direction}, m_pwm{p_pwm}
   {
      // if (!isPinValueProper())
      //   return;
      pinMode(m_direction, OUTPUT);
      pinMode(m_pwm, OUTPUT);

      // Initially Halt the Motor
      setSpeed(0);
   }

 private:
   inline bool isPinValueProper() const
   {
      return m_direction != 0 && m_pwm != 0;
   }
   inline constexpr static uint8_t reverseDirection()
   {
      return LOW;
   }
   inline constexpr static uint8_t straightDirection()
   {
      return HIGH;
   }

 public:
   // Supply Given Speed Value to PWM Pin
   // Provide Negative Speed to reverse
   void setSpeed(int16_t const p_speed_val) const
   {
      digitalWrite(m_direction, (p_speed_val > 0) ? straightDirection() : reverseDirection());

      // Constrain Speed within given range
      uint8_t const constrain_speed = constrain(abs(p_speed_val), 0 /*Min PWM*/, 255 /*Max PWM*/);

      // Send the Speed Value to PWM Pin
      analogWrite(m_pwm, constrain_speed);
   }
};

// Returns Values for Bot MotionDirection Of Motion
enum class MotionDirection : byte
{
   LEFT,
   FORWARD,
   RIGHT
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
   constexpr inline T maxF(T const left, T const right) const
   {
      return (left > right) ? left /*Max*/ : right /*Max*/;
   }

 public:
   MotionController(SingleMotorController const& p_fl_motor,
                    SingleMotorController const& p_fr_motor,
                    SingleMotorController const& p_bl_motor,
                    SingleMotorController const& p_br_motor) :
       m_fl_motor{p_fl_motor},
       m_fr_motor{p_fr_motor}, m_bl_motor{p_bl_motor}, m_br_motor{p_br_motor}
   {
   }

   //void moveRawTo(int16_t const p_x, int16_t const p_y, int16_t const p_rot) const
   //{
   //   // This is a very interesting formula
   //   // Try Performing Calculations on Paper
   //   // Note that
   //   // +x -> Right
   //   // +y -> Forward
   //   // +z -> Counter Clock Wise

   //   int16_t flpwm = +p_x + p_y - p_rot;
   //   int16_t frpwm = -p_x + p_y + p_rot;
   //   int16_t blpwm = -p_x + p_y - p_rot;
   //   int16_t brpwm = +p_x + p_y + p_rot;

   //   // Find the Maximum Speed Out of All

   //   int16_t max_pwm = maxF(abs(flpwm), abs(frpwm));
   //   max_pwm         = maxF(max_pwm, abs(blpwm));
   //   max_pwm         = maxF(max_pwm, abs(brpwm));

   //   // In the End, the Maximum Value of All
   //   // Would be present in max_pwm variable

   //   if (max_pwm > MaxPWMAllowed)
   //   {
   //      // Now Ensure that the Speed Values are in
   //      // their given proper constraints

   //      flpwm = (flpwm * MaxPWMAllowed) / max_pwm;
   //      frpwm = (frpwm * MaxPWMAllowed) / max_pwm;
   //      blpwm = (blpwm * MaxPWMAllowed) / max_pwm;
   //      brpwm = (brpwm * MaxPWMAllowed) / max_pwm;
   //   }
   //   moveMotorsDirect(flpwm, frpwm, blpwm, brpwm);
   //}

   // Move Legs Direct
   inline void moveLegsDirect(int16_t const flpwm, int16_t const frpwm, int16_t const blpwm, int16_t const brpwm) const
   {
      // Change the Signs Here
      // To Change Direction of
      // Revolution
      return moveMotorsDirect(flpwm, frpwm, -blpwm, -brpwm);
   }

   // Directly supply PWM to the Given Motors
   inline void
       moveMotorsDirect(int16_t const flpwm, int16_t const frpwm, int16_t const blpwm, int16_t const brpwm) const
   {
      m_fl_motor.setSpeed(flpwm);
      m_fr_motor.setSpeed(frpwm);
      m_bl_motor.setSpeed(blpwm);
      m_br_motor.setSpeed(brpwm);
   }

   void halt() const
   {
      // Set All Speed to 0
      // TO Halt Bot
      return moveMotorsDirect(0, 0, 0, 0);
   }

   // Just moves the Platform Straight
   // No Corrections
   //inline void moveStraight() const
   //{
   //   // As there is No Correction to be made
   //   return moveStraightWithCorrection(0);
   //}

   // This Moves the Platform Straight
   // And takes care of Little Corrections
   //void moveStraightWithCorrection(int16_t const p_pwm_correction) const
   //{
   //   // Pass Correction to Rot
   //   switch (StraightDirection)
   //   {
   //      case MotionDirection::FORWARD:
   //         // While Moving Forward
   //         // +ve y:- Forward Line
   //         moveRawTo(0, m_straight_line_pwm, p_pwm_correction);
   //         break;
   //      case MotionDirection::LEFT:
   //         // While Moving Left
   //         // -ve x:- Left
   //         moveRawTo(-m_straight_line_pwm, 0, p_pwm_correction);
   //         break;
   //      case MotionDirection::RIGHT:
   //         // While Moving Right
   //         // +ve x:- Right
   //         moveRawTo(m_straight_line_pwm, 0, p_pwm_correction);
   //         break;
   //   }
   //}

   //void StraightLinePWM(uint8_t const p_straight_line_pwm)
   //{
   //   m_straight_line_pwm = constrain(p_straight_line_pwm, 0 /*MinPWM*/, MaxPWMAllowed);
   //}
   //void FrontLeftMotor(SingleMotorController const&& p_fl_motor)
   //{
   //   m_fl_motor = p_fl_motor;
   //}
   //void FrontRightMotor(SingleMotorController const&& p_fr_motor)
   //{
   //   m_fr_motor = p_fr_motor;
   //}
   //void BackLeftMotor(SingleMotorController const&& p_bl_motor)
   //{
   //   m_bl_motor = p_bl_motor;
   //}
   //void BackRightMotor(SingleMotorController const&& p_br_motor)
   //{
   //   m_br_motor = p_br_motor;
   //}
};