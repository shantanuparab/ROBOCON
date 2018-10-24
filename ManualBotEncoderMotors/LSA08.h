#include <Arduino.h>
#include <Stream.h>

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

    // Set LSA08 Address
    byte m_address;

  private:
    // Serial Enable Pin Works on Active LOW Logic
    // Set it to High to stop asking for data
    void stopUARTCommunication()
    {
      digitalWrite(m_com_enable, HIGH);
    }
    // Serial Enable Pin Works on Active LOW Logic
    // Set it to LOW to start asking for data
    void startUARTCommunication()
    {
      digitalWrite(m_com_enable, LOW);
    }

    // Read a UART Value
    // Assume Available
    int readUARTValue()
    {
      return m_comm->read();
    }

    int writeUARTData(const byte p_data)
    {
      // Verify if the UART Portions have been properly set up
      if (!isUARTProperlySetup())
        // 0 usually represents Nothing Written
        return 0;

      // Send data
      return m_comm->write(p_data);
    }

    // Returns Number of Bits of Data Available
    int noOfUARTReadBitsAvailable()
    {
      // Verify if the UART Portions have been properly set up
      if (!isUARTProperlySetup())
        // 0 usually represents Nothing Available
        return 0;

      return m_comm->available();
    }
    // Check if the Given Line Sensor Contains proper values
    // Not those assigned by Constructor
    bool isUARTProperlySetup() const
    {
      return (m_comm != nullptr && m_com_enable != 0 && m_jpulse != 0);
    }

  public:
    // Has 4 parameters
    // p_commm to specify stream
    // So you may pass Serial1, Serial2, Serial3 etc
    // To Specify which UART Pins to deal via
    // p_com_enable:- Pass the Serial Communication Enable Pin
    // p_jpulse:- Pass the Junction Pulse Pin
    // Note that these default values are
    // JUNK
    // And Corrected Values must be provided in Code
    SingleLSA08(Stream*	 p_comm			= nullptr,
                const Pin  p_com_enable = 0,
                const Pin  p_jpulse		= 0,
                const byte p_address		= 0x01) :
      m_comm{p_comm},
      m_com_enable{p_com_enable}, m_jpulse{p_jpulse}, m_address{p_address}
    {

      // If the UART Pin Values are Not Properly Setup
      // Do Nothing
      // Else it may be catastrophic
      if (!isUARTProperlySetup())
        return;

      // As Data needs to be sent to Serial Enable Pin
      // Set As Output
      pinMode(m_com_enable, OUTPUT);

      // As Junction Pin sends data back,
      // Set as Input
      pinMode(m_jpulse, INPUT);

      stopUARTCommunication();
      // UART Mode 2 returns value between 0 & 70
      setUARTDataOutputMode(2);

      Serial.print("Hey ");
      Serial.println(noOfUARTReadBitsAvailable());

      // Clear the Junction Count
      clearJunctionCount();


      m_comm->flush();
    }

  public:
    // Ensure the Junction is actually set as Interrupt
    // If this is called
    // Ensure that Junction Pin is connected to Interrupt Pin
    // Please Verify Interrupt Pins for Your board
    void setJunctionPinAsInterrupt(void (*p_isr)(void), const int p_change = RISING) const
    {
      attachInterrupt(digitalPinToInterrupt(m_jpulse), p_isr, p_change);
    }
    // If Junction Detected
    // Return true
    bool isJunctionDetected() const
    {
      return (digitalRead(m_jpulse) == HIGH);
    }
    byte getAddress() const
    {
      return m_address;
    }

    // Return value received from Line Sensor
    // On Error, return 255
    byte readLineSensor()
    {
      // Verify if the UART Portions have been properly set up
      if (!isUARTProperlySetup())
        // As 255 represents Nothing detected
        return 255;

      // Ask for Data Actively
      startUARTCommunication();

      // If Value available, read it
      if (noOfUARTReadBitsAvailable() > 0)
      {
        // Store Read UART Value
        const byte dummy = readUARTValue();
        stopUARTCommunication();

        Serial.print("UART Value");
        Serial.println(dummy);


        // Verify if the UART Values are within the
        // Required Range
        // If Not send Not Detected
        if (dummy >= 0 && dummy <= 70)
        {
          // Return the Read Value
          return dummy;
        }
        else
        {
          // Return 255 as it represents Nothing Detected
          return 255;
        }
      }
      stopUARTCommunication();
      // Returns 255 when nothing detected
      return 255;
    }

  public:
    // Send Data to UART
    void sendCommandToLineSensor(const byte command, const byte data)
    {
      // Verify if the UART Portions have been properly set up
      if (!isUARTProperlySetup())
        return;

      const byte checksum = m_address + command + data;

      writeUARTData(m_address);
      writeUARTData(command);
      writeUARTData(data);
      writeUARTData(checksum);
    }
    void clearJunctionCount()
    {
      sendCommandToLineSensor('X', 0x00);
    }
    int getJunctionCount()
    {
      // Verify if the UART Portions have been properly set up
      if (!isUARTProperlySetup())
        // Send 0 Junction Counts sent by Default
        return 0;

      // Ask LSA08 to supply data
      sendCommandToLineSensor('X', 0x01);

      // if has data available, check and return
      if (noOfUARTReadBitsAvailable() > 0)
        return readUARTValue();

      // Send 0 Junction Counts sent by Default
      return 0;
    }
    // Check if the LSA08 is properly connected
    // Note that this isn't easy
    // We need to send and receieve UART Data
    // But We need to modify a register while checking connection
    // Let us assume the register that we shall modify is
    // The Register that sets the Output Mode
    // Now we know that by default the LSA08 Data Output Mode
    // is 2
    // So we shall set it to that
    bool isConnected()
    {
      // Verify if the UART Portions have been properly set up
      if (!isUARTProperlySetup())
        return false;

      //m_comm->flush();
      // Set UART Mode to 2
      // This is just for testing purposes and the best idea that we could muster
      setUARTDataOutputMode(2);

      //Serial.println("Sent Data to UART Monitor");

      // Wait for sometime
      //delay(200 /*0.2 Seconds*/);

      //Serial.print(noOfUARTReadBitsAvailable());

      // Now check if Data is Available
      if (noOfUARTReadBitsAvailable() <= 0)
        return false;
      // This reaches here only if there is data to be read

      // Print No Of Bits Available
      Serial.print("No Of UART Bits Available to Read\t:");
      Serial.print(noOfUARTReadBitsAvailable());
      Serial.println();

      // On Correct Value Send,
      // LSA08 Returns OK
      // As such check_1 must contain 'O'
      const int check_1 = readUARTValue();

      // In Case first Letter is Not O
      // It means nothing detected
      if (check_1 != 'O' && check_1 != 'o')
        return false;

      // Now check if Data is Available
      if (noOfUARTReadBitsAvailable() <= 0)
        return false;

      // On Correct Value Send,
      // LSA08 Returns OK
      // As such check_2 must contain 'K'
      const int check_2 = readUARTValue();

      // In Case first Letter is Not O
      // It means nothing detected
      if (check_2 != 'K' && check_2 != 'k')
        return false;

      // In Case we reached here
      // It means everything is all right
      // This means that the LSA08 Connection is working
      return true;
    }
    void setUARTDataOutputMode(const byte mode)
    {
      // Verify if the UART Portions have been properly set up
      if (!isUARTProperlySetup())
        return;

      // There are a maximum of 3 Modes available
      if (mode > 3)
        return;
      sendCommandToLineSensor('D', mode);
    }
    void startCaliberation()
    {
      sendCommandToLineSensor('C', 0x00);
    }
    void setLCDContrast(const byte p_contrast)
    {
      sendCommandToLineSensor('S', p_contrast);
    }
    void setLCDBacklight(const byte p_backlight)
    {
      if (p_backlight <= 10)
        sendCommandToLineSensor('B', p_backlight);
    }
    void setJunctionWidth(const byte p_junction_width)
    {
      if (p_junction_width >= 3 && p_junction_width <= 8)
        sendCommandToLineSensor('J', p_junction_width);
    }
    void setLineThreshold(const byte p_threshold) {
      if (p_threshold <= 7)
        sendCommandToLineSensor('T', p_threshold);
    }
    void setLineTypeMode(const byte p_line_mode) {
      if (p_line_mode <= 1)
        sendCommandToLineSensor('L', p_line_mode);
    }
};
struct LSA08s
{
  private:
    enum class Direction
    {
      FORWARD,
      SIDEWAYS
    };

    SingleLSA08 m_forward;
    SingleLSA08 m_sideways;

    Direction m_sensor_enabled = Direction::FORWARD;

  public:
    // This forces the Line Sensor to test the connection
    // If any Line Sensor is not connected
    // Displays Error Message
    void testConnection()
    {
      while (!m_forward.isConnected())
      {
        Serial.println("Unable to Connect to Forward Line Sensor");
        Serial.println("Please Verify All Values such as if");
        Serial.print("Address is\t:");
        Serial.println(m_forward.getAddress());
        Serial.println("And the Baud Rate Provided is Correct");
        delay(1000 /*Wait for 1 Second*/);
        Serial.println("Retrying...");
      }
      /*		while (!m_sideways.isConnected())
      		{
      			Serial.println("Unable to Connect to Sideways Line Sensor");
      			Serial.println("Please Verify All Values such as if");
      			Serial.print("Address is\t:");
      			Serial.println(m_sideways.getAddress());
      			Serial.println("And the Baud Rate Provided is Correct");
      			delay(1000 /*Wait for 1 Second*//*);
			Serial.println("Retrying...");
		}
*/
    }

    // Set the Forward Sensor
    void setLSA08Forward(const SingleLSA08& p_forward)
    {
      m_forward = p_forward;
    }
    // Set the Sideways Sensor
    void setLSA08Sideways(const SingleLSA08& p_sideways)
    {
      m_sideways = p_sideways;
    }
    // Returns the Value Read from the Line Sensor from the Appropriate
    // LSA08 Connected
    byte readLineSensor()
    {
      switch (m_sensor_enabled)
      {
        case Direction::FORWARD:
          return m_forward.readLineSensor();
          break;
        case Direction::SIDEWAYS:
          return m_sideways.readLineSensor();
          break;
      }
      // This statement is impossible to reach
      // As FORWARD and SIDEWAYS are the
      // Only Two Line Sensors
      // Kept to impress GCC's Compiler
      // And Remove Warnings
      // 255 indicates no line detected
      return 255;
    }
    bool isJunctionDetected()
    {
      switch (m_sensor_enabled)
      {
        case Direction::FORWARD:
          return m_forward.isJunctionDetected();
        case Direction::SIDEWAYS:
          return m_sideways.isJunctionDetected();
      }
      // This statement is impossible to reach
      // As FORWARD and SIDEWAYS are the
      // Only Two Line Sensors
      // Kept to impress GCC's Compiler
      // And Remove Warnings
      // false indicates no Junction Detected
      return false;
    }
    void setForwardActive()
    {
      m_sensor_enabled = Direction::FORWARD;
    }
    void setSidewaysActive()
    {
      m_sensor_enabled = Direction::SIDEWAYS;
    }
};
