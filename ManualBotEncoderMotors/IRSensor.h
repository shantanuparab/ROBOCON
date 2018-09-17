#pragma once

struct IRSensor
{
 private:
   using Pin = byte;

   const Pin  m_input_pin;
   const byte m_active_val;

   bool m_enabled;

 public:
   // By default is set to active low
   // And keep sensor Disabled
   IRSensor(const Pin p_input_pin, const byte p_active_val = LOW, const bool p_enabled = false) :
       m_input_pin{p_input_pin}, m_active_val{p_active_val}, m_enabled{p_enabled}
   {
      pinMode(m_input_pin, INPUT);
   }
   void enable()
   {
      m_enabled = true;
   }
   void disable()
   {
      m_enabled = false;
   }
   // Check if IR Sensor is Enabled
   bool isEnabled() const
   {
      return m_enabled;
   }
   // Returns true if
   // The Given IR Sensor is Enabled
   // And The Digital Pin And Active Value is same
   bool isDetected() const
   {
      return isEnabled() && (digitalRead(m_input_pin) == m_active_val);
   }
};

struct IRSensors
{
 private:
   IRSensor m_ir_left;
   IRSensor m_ir_right;
   IRSensor m_ir_forward;

   using Pin = byte;

 public:
   IRSensors(const Pin p_ir_left_input,
             const Pin p_ir_right_input,
             const Pin p_ir_forward_input,
             // By Default Our IRs Operate on Active Low logic
             const byte p_ir_left_active    = LOW,
             const byte p_ir_right_active   = LOW,
             const byte p_ir_forward_active = LOW,
             // Set As True if Forward Active
				 // False if Sideways Active
             const bool p_is_forward_active = false) :
       m_ir_left{p_ir_left_input, p_ir_left_active},
       m_ir_right{p_ir_right_input, p_ir_right_active}, m_ir_forward{p_ir_forward_input, p_ir_forward_active}
   {
      if (p_ir_forward_active)
         setForwardActive();
      else
         setSidewaysActive();
   }

   bool isForwardIRDetected() const
   {
      return m_ir_forward.isDetected();
   }
   bool isRightIRDetected() const
   {
      return m_ir_right.isDetected();
   }
   bool isLeftIRDetected() const
   {
      return m_ir_left.isDetected();
   }

   void setForwardActive()
   {
      // Set Forward As Active
      // Disable Left and Right
      m_ir_left.disable();
      m_ir_right.disable();
      m_ir_forward.enable();
   }
   void setSidewaysActive()
   {
      // Set Forward As Active
      // Disable Left and Right
      m_ir_left.enable();
      m_ir_right.enable();
      m_ir_forward.disable();
   }
};