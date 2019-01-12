// Ensures this Header File is only included Once while compiling
#pragma once


// Writing Pin would expand to this
// For example
// Pin pin;
// and
// uint8_t pin would be the same statements
using Pin = byte;



// This is the Object to be used for Motor Controller
// Sample Usage
// SingleMotorController controller(5,6, HIGH);
// Go Reverse At PWM 30
// controller.setSpeed(-30);
struct SingleMotorController
{
  private:
    // The Direction Pin. Used to Control Direction
    Pin m_direction;
    // The PWM Pin. Used to Control Speed
    Pin m_pwm;

    // Provide the Value for Which this given Motor Goes Straight
    // Default Value is HIGH
    uint8_t m_direction_straight;

  public:
    // p_pwm :- The PWM Pin
    // p_direction :- The Direction Pin
    // p_direction_straight :- The Value to Enter for Bot to Go Straight
    // Can be LOW or HIGH
    // Note:- Ensure that PWM Pin is actually Marked as PWM on Board
    // Note that the 0 value given to Pins by default
    // Is actually incorrect and needs to be changed
    SingleMotorController(const Pin p_direction = 0, const Pin p_pwm = 0, const uint8_t p_direction_straight = HIGH) :
      m_direction{ p_direction }, m_pwm{ p_pwm }, m_direction_straight{ p_direction_straight }
    {
      if (!isPinValueProper())
        return;
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
    inline void setSpeed(const int16_t p_speed_val) const
    {
      // If the Pin Values are not proper,
      // Do Nothing
      if (!isPinValueProper())
        return;

      if (p_speed_val < 0)
        digitalWrite(m_direction, reverseDirection());
      else if (p_speed_val > 0)
        digitalWrite(m_direction, straightDirection());

      // Constrain Speed within given range
      const int16_t constrain_speed = constrain(abs(p_speed_val), 0, 255);

      // Send the Speed Value to PWM Pin
      analogWrite(m_pwm, constrain_speed);
    }
};

struct MotionController
{
  private:
    // Returns Values for Bot Direction Of Motion
    enum class Direction
    {
      LEFT,
      FORWARD,
      RIGHT
    };

    // Front Left Motor
    SingleMotorController m_fl_motor;
    // Front Right Motor
    SingleMotorController m_fr_motor;
    // Back Left Motor
    SingleMotorController m_bl_motor;
    // Back Right Motor
    SingleMotorController m_br_motor;

    // Set the Max Allowable PWM
    int16_t m_max_allowable_pwm = 255;

    // Set the Straight Line PWM
    int16_t m_straight_line_pwm = 200;

    // Set the Bot Motion Direction
    Direction m_move_direction = Direction::FORWARD;

  public:
    void moveRawTo(const int16_t p_pwmx, const int16_t p_pwmy, const int16_t p_pwmrot)
    {
      // This is a very interesting formula
      // Try Performing Calculations on Paper
      // Note that
      // +x -> Right
      // +y -> Forward
      // +z -> Counter Clock Wise

      long flpwm = -p_pwmx + p_pwmy + p_pwmrot;
      long frpwm = -p_pwmx - p_pwmy - p_pwmrot;
      long blpwm = -p_pwmx + p_pwmy - p_pwmrot;
      long brpwm = +p_pwmx + p_pwmy - p_pwmrot;
//      Serial.print(" flpwm ");
//      Serial.print(flpwm);
//      Serial.print(" frpwm ");
//      Serial.print(frpwm);
//      Serial.print(" blpwm ");
//      Serial.print(blpwm);
//      Serial.print(" brpwm ");
//      Serial.print(brpwm);
      // Find the Maximum Speed Out of All
      int max_pwm = abs(flpwm);
      if (max_pwm < abs(frpwm))
        max_pwm = abs(frpwm);
      if (max_pwm < abs(blpwm))
        max_pwm = abs(blpwm);
      if (max_pwm < abs(brpwm))
        max_pwm = abs(brpwm);
      // In the End, the Maximum Value of All
      // Would be present in max_pwm variable

      if (max_pwm > m_max_allowable_pwm)
      {
        // Now Ensure that the Speed Values are in
        // their given proper constraints

        flpwm = (flpwm * m_max_allowable_pwm) / max_pwm;
        frpwm = (frpwm * m_max_allowable_pwm) / max_pwm;
        blpwm = (blpwm * m_max_allowable_pwm) / max_pwm;
        brpwm = (brpwm * m_max_allowable_pwm) / max_pwm;


      }
//      Serial.print(" maxPwm ");
//      Serial.print(max_pwm);
//      Serial.print(" m_max_allowable_pwm ");
//      Serial.print(m_max_allowable_pwm);
//      Serial.print(" flpwm ");
//      Serial.print(flpwm);
//      Serial.print(" frpwm ");
//      Serial.print(frpwm);
//      Serial.print(" blpwm ");
//      Serial.print(blpwm);
//      Serial.print(" brpwm ");
//      Serial.print(brpwm);
      moveMotorsDirect(flpwm, frpwm, blpwm, brpwm);
    }


    void moveMotorsDirect(int16_t flpwm, int16_t frpwm, int16_t blpwm, int16_t brpwm)
    {
      m_fl_motor.setSpeed(flpwm);
      m_fr_motor.setSpeed(frpwm);
      m_bl_motor.setSpeed(blpwm);
      m_br_motor.setSpeed(brpwm);
    }

    void halt()
    {
      // Set All Speed to 0
      // TO Halt Bot
      return moveRawTo(0, 0, 0);
    }
    // Just moves the Platform Straight
    // No Corrections
    void moveStraight()
    {
      // As there is No Correction to be made
      return moveStraightWithCorrection(0);
    }

    // This Moves the Platform Straight
    // And takes care of Little Corrections
    void moveStraightWithCorrection(const int16_t p_pwm_correction)
    {
      switch (m_move_direction)
      {
        case Direction::FORWARD:
          // While Moving Forward
          // +ve y:- Forward Line
          moveRawTo(0, m_straight_line_pwm, p_pwm_correction);
          break;
        case Direction::LEFT:
          // While Moving Left
          // -ve x:- Left
          moveRawTo(-m_straight_line_pwm, 0, p_pwm_correction);
          break;
        case Direction::RIGHT:
          // While Moving Right
          // +ve x:- Right
          moveRawTo(m_straight_line_pwm, 0, p_pwm_correction);
          break;

      }
    }

    void setStraightAsRight()
    {
      m_move_direction = Direction::RIGHT;
    }
    void setStraightAsLeft()
    {
      m_move_direction = Direction::LEFT;
    }
    void setStraightAsForward()
    {
      m_move_direction = Direction::FORWARD;
    }
    void setStraightLinePWM(const int16_t p_straight_line_pwm)
    {
      m_straight_line_pwm = constrain(abs(p_straight_line_pwm), 0, m_max_allowable_pwm);
    }
    void setMaxPWMAllowed(const int16_t p_max_allowable_pwm)
    {
      m_max_allowable_pwm = constrain(abs(p_max_allowable_pwm), 0, 255);
    }

    void setFrontLeftMotor(const SingleMotorController& p_fl_motor)
    {
      m_fl_motor = p_fl_motor;
    }
    void setFrontRightMotor(const SingleMotorController& p_fr_motor)
    {
      m_fr_motor = p_fr_motor;
    }
    void setBackLeftMotor(const SingleMotorController& p_bl_motor)
    {
      m_bl_motor = p_bl_motor;
    }
    void setBackRightMotor(const SingleMotorController& p_br_motor)
    {
      m_br_motor = p_br_motor;
    }
};



