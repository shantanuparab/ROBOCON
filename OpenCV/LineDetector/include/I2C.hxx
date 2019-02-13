#pragma once

#include <cstdint>
#include <stdexcept>

// Use this to access RaspberryPi I2C
#include <wiringPiI2C.h>

#include <unistd.h>


// Please Use
// https://learn.sparkfun.com/tutorials/raspberry-pi-spi-and-i2c-tutorial/all
// To Perform Connections Required for RaspBerryPi

namespace Pi
{
   struct I2C
   {
    private:
      using byte = std::uint8_t;
      // This API Primarily relies on Data Hiding
      // The Hidden Value is the I2C Handle
      // To the Given File
      int const m_i2c_handle;

    public:
      // Send the I2C Device Address
      I2C(std::uint8_t const p_device_address) : m_i2c_handle{wiringPiI2CSetup(p_device_address)}
      {
         // Returns -1 as Handle on Error
         if (m_i2c_handle == -1)
            throw std::runtime_error("Unable to Access I2C Device Handle");
      }
      ~I2C()
      {
         // Close the I2C Handle
         close(m_i2c_handle);
      }

      // Reads a Byte without the need of a register
      byte readByte() const noexcept
      {
         return wiringPiI2CRead(m_i2c_handle);
      }
      byte readByte(byte const p_reg) const
      {
         auto const reading = wiringPiI2CReadReg8(m_i2c_handle, p_reg);
         if (reading == -1)
            throw std::runtime_error("Unable to Read Data from Device");
         else
            return static_cast<byte>(reading);
      }
      std::uint16_t readData(byte const p_reg) const
      {
         auto const reading = wiringPiI2CReadReg16(m_i2c_handle, p_reg);
         if (reading == -1)
            throw std::runtime_error("Unable to Read Data from Device");
         else
            return static_cast<std::uint16_t>(reading);
      }
      // Writes to a Byte without the need for a register
      bool write(byte const p_data) const noexcept
      {
         // Returns -1 on Failure
         return wiringPiI2CWrite(m_i2c_handle, p_data) != -1;
      }
      // Write Data to Specified Address
      bool write(byte const p_reg, byte const p_data) const noexcept
      {
         return wiringPiI2CWriteReg8(m_i2c_handle, p_reg, p_data) != -1;
      }
      bool write(byte const p_reg, std::int16_t const p_data) const noexcept
      {
         return wiringPiI2CWriteReg16(m_i2c_handle, p_reg, p_data) != -1;
      }

      int getHandle() const noexcept
      {
         return m_i2c_handle;
      }
   };
} // namespace Pi
