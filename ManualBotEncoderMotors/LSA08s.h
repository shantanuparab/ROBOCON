//#include <Arduino.h>
//#include <Stream.h>

// Use this Class to Work with LSA08
struct SingleLSA08
{
 private:
   using Pin = byte;

   // The Stream Used for UART Communication
   Stream* m_comm;

   // Pin for Serial Enable Communications
   Pin m_com_enable;
   // Pin for Junction Pulse
   Pin m_jpulse;

 private:
   // Serial Enable Pin Works on Active LOW Logic
   // Set it to High to stop asking for data
   inline void stopLSA08Read() const
   {
      digitalWrite(m_com_enable, HIGH);
   }
   // Serial Enable Pin Works on Active LOW Logic
   // Set it to LOW to start asking for data
   inline void startLSA08Read() const
   {
      digitalWrite(m_com_enable, LOW);
   }

   // Read a UART Value
   // Assume Available
   inline int readUARTValue() const
   {
      return m_comm->read();
   }

   inline size_t writeUARTData(byte const p_data) const
   {
      // Send data
      return m_comm->write(p_data);
   }

   // Returns Number of Bits of Data Available
   inline int noOfUARTReadBitsAvailable() const
   {
      return m_comm->available();
   }
   // Check if the Given Line Sensor Contains proper values
   // Not those assigned by Constructor
   inline bool isUARTImproperlySetup() const
   {
      return (m_comm == nullptr || m_com_enable == 0 || m_jpulse == 0);
   }

 public:
   // Set LSA08 Address
   byte Address;

 public:
   // Has 4 parameters
   // p_commm to specify stream
   // So you may pass Serial1, Serial2, Serial3 etc
   // To Specify which UART Pins to deal via
   // p_com_enable:- Pass the Serial Communication Enable Pin
   // p_jpulse:- Pass the Junction Pulse Pin.
   // Please Ensure JPulse is Junction PIN
   // Note that these default values are
   // JUNK
   // And Corrected Values must be provided in Code
   SingleLSA08(Stream* p_comm = nullptr, Pin const p_com_enable = 0, Pin const p_jpulse = 0, byte const p_address = 0) :
       m_comm{p_comm}, m_com_enable{p_com_enable}, m_jpulse{p_jpulse}, Address{p_address}
   {

      // If the UART Pin Values are Not Properly Setup
      // Do Nothing
      // Else it may be catastrophic
      if (isUARTImproperlySetup())
         return;

      // As Data needs to be sent to Serial Enable Pin
      // Set As Output
      pinMode(m_com_enable, OUTPUT);

      // As Junction Pin sends data back,
      // Set as Input
      // Note that Junction Pin Must be at
      // Interrupt
      pinMode(m_jpulse, INPUT_PULLUP);

      // Ensure that no UART Communication
      // Has been Enabled
      // Initially
      stopLSA08Read();
   }

 public:
   // Ensure the Junction is actually set as Interrupt
   // If this is called
   // Ensure that Junction Pin is connected to Interrupt Pin
   // Please Verify Interrupt Pins for Your board
   void SetJunctionPinAsInterrupt(void (*p_isr)(void), byte const p_mode = RISING) const
   {
      attachInterrupt(digitalPinToInterrupt(m_jpulse), p_isr, p_mode);
   }
   // Return value received from Line Sensor
   // On Error, return 255
   byte read() const
   {
      // Ask for Data Actively
      startLSA08Read();
      byte dummy = 255 /*Default Value when nothing available*/;
      // If Value available, read it
      if (noOfUARTReadBitsAvailable() > 0)
      {
         // Store Read UART Value
         dummy = readUARTValue();
         // Verify if the UART Values are within the
         // Required Range
         // If Not send Not Detected
      }
      stopLSA08Read();
      return dummy;
   }

 public:
   // Send Data to UART
   void sendCommandToLineSensor(byte const command, byte const data) const
   {
      byte const checksum = Address + command + data;

      writeUARTData(Address);
      writeUARTData(command);
      writeUARTData(data);
      writeUARTData(checksum);
   }
   void clearJunctionCount()
   {
      sendCommandToLineSensor('X', 0x00);
   }
   // Check if the LSA08 is properly connected
   // Note that this isn't easy
   // We need to send and receieve UART Data
   // But We need to modify a register while checking connection
   // Let us assume the register that we shall modify is
   // The Register that sets the Output Mode
   bool TestConnection() const
   {
      // Set UART Mode to 2
      // This Constrains Output between 0-70
      // This is just for testing purposes and the best idea that we could muster
      setUARTDataOutputMode(2);

      // Wait for sometime
      delay(600 /*0.6 Seconds*/);
      // Now check if Data is Available
      Serial.print(F("Count of UART Bits at OUT\t:"));
      Serial.println(noOfUARTReadBitsAvailable());
      if (noOfUARTReadBitsAvailable() < 2)
         return false;
      // This reaches here only if there is data to be read

      // On Correct Value Send,
      // LSA08 Returns OK
      // As such check_1 must contain 'O'
      auto const check_1 = readUARTValue();

      Serial.print(F("Check 1 Returned"));
      Serial.println((char)check_1);

      // In Case first Letter is Not O
      // It means nothing detected
      if (check_1 != 'O' && check_1 != 'o')
         return false;

      // On Correct Value Send,
      // LSA08 Returns OK
      // As such check_2 must contain 'K'
      auto const check_2 = readUARTValue();

      Serial.print(F("Check 2 Returned"));
      Serial.println((char)check_2);

      // In Case first Letter is Not O
      // It means nothing detected
      if (check_2 != 'K' && check_2 != 'k')
         return false;

      // In Case we reached here
      // It means everything is all right
      // This means that the LSA08 Connection is working
      return true;
   }
   void setUARTDataOutputMode(byte const mode) const
   {
      sendCommandToLineSensor('D', mode);
   }
   void startCaliberation()
   {
      sendCommandToLineSensor('C', 0x00);
   }
   void setLCDContrast(byte const p_contrast)
   {
      sendCommandToLineSensor('S', p_contrast);
   }
   void setLCDBacklight(byte const p_backlight)
   {
      sendCommandToLineSensor('B', p_backlight);
   }
   void setJunctionWidth(byte const p_junction_width)
   {
      sendCommandToLineSensor('J', p_junction_width);
   }
   void setLineThreshold(byte const p_threshold)
   {
      sendCommandToLineSensor('T', p_threshold);
   }
   void setLineTypeMode(byte const p_line_mode)
   {
      sendCommandToLineSensor('L', p_line_mode);
   }
};

enum class LSA08Selected : uint8_t
{
   FORWARD,
   SIDEWAYS
};

struct LSA08s
{
 private:
   SingleLSA08 m_forward;
   SingleLSA08 m_sideways;

 public:
   // Set the Selected LSA08
   LSA08Selected Selected = LSA08Selected::FORWARD;

   // This forces the Line Sensor to test the connection
   // If any Line Sensor is not connected
   // Displays Error Message
   void TestConnection() const
   {
      while (!m_forward.TestConnection())
      {
         Serial.println(F("\n\nUnable to Connect to Forward Line Sensor"));
         Serial.println(F("Please Verify if Pins etc. Provided are Correct and Line Sensor is ON"));
         delay(2000 /*2 Seconds*/);
      }
      while (!m_sideways.TestConnection())
      {
         Serial.println(F("\n\nUnable to Connect to Sideways Line Sensor"));
         Serial.println(F("Please Verify if Pins etc. Provided are Correct and Line Sensor is ON"));
         delay(2000 /*2 Seconds*/);
      }
   }

   // Set the Forward Sensor
   void LSA08Forward(SingleLSA08 const& p_forward)
   {
      m_forward = p_forward;
   }
   // Set the Sideways Sensor
   void LSA08Sideways(SingleLSA08 const& p_sideways)
   {
      m_sideways = p_sideways;
   }
   // Returns the Value Read from the Line Sensor from the Appropriate
   // LSA08 Connected
   byte read() const
   {
      switch (Selected)
      {
         case LSA08Selected::FORWARD:
            return m_forward.read();
         case LSA08Selected::SIDEWAYS:
            return m_sideways.read();
      }
      // This statement is impossible to reach
      // As FORWARD and SIDEWAYS are the
      // Only Two Line Sensors
      // Kept to remove GCC Compiler Warnings
      // 255 indicates no line detected
      return 255;
   }

   // Ensure that the Junction Pin Provided
   // Is Connected to Interrupt
   void SetJunctionPinAsInterrupt(LSA08Selected const p_selected, void (*p_isr)(), byte const p_mode = RISING)
   {
      switch (p_selected)
      {
         case LSA08Selected::FORWARD:
            return m_forward.SetJunctionPinAsInterrupt(p_isr, p_mode);
         case LSA08Selected::SIDEWAYS:
            return m_sideways.SetJunctionPinAsInterrupt(p_isr, p_mode);
      }
   }
};
