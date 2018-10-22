#pragma once
// Required for Serial Communication
#include <wiringSerial.h>

#include <cstdint>
#include <stdexcept>

// For Sample Code, please check out
// https://github.com/WiringPi/WiringPi/blob/master/examples/serialTest.c
// Note that this doesn't use Our Api
// So you will have to Modify it
namespace Pi
{
   struct Serial
   {
    private:
      const int m_serial_handle;

    public:
      // Specify the Serial Device and Baud Rate
      // Note that Baud Rate on Both Ends must match
      Serial(const char* p_serial_device, const std::int32_t p_baud_rate) :
          m_serial_handle{serialOpen(p_serial_device, p_baud_rate)}
      {
         // Returns -1 as Handle on Error
         if (m_serial_handle == -1)
            throw std::runtime_error("Unable to Access I2C Device Handle");
      }
      ~Serial()
      {
         close();
      }
      // Use this to close the serial connection
      void close() const noexcept
      {
         serialClose(m_serial_handle);
      }
      // Write a Character
      void write(const unsigned char p_ch)
      {
         serialPutchar(m_serial_handle, p_ch);
      }
      // Write A String
      void write(const char* p_str) const noexcept
      {
         serialPuts(m_serial_handle, p_str);
      }
      // Write String like Printf
      template <typename... Args>
      void write(const char* buf, Args&&... args) const noexcept
      {
         serialPrintf(m_serial_handle, ...);
      }
      // Count Number of Bytes Available to Read
      std::int32_t available() const noexcept
      {
         return serialDataAvail(m_serial_handle);
      }
      // Read Single Byte
      std::uint8_t read() const noexcept
      {
         return static_cast<std::uint8_t>(serialGetchar(m_serial_handle));
      }
   };
} // namespace Pi