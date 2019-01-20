#pragma once

// Required for Interfacing with I2C
#include "I2C.hxx"

namespace Pi
{
   // Implementation of a Library Similar to EasyTransfer Lib present on
   // Arduino
   // In order to make transfer of code simpler and more readable on both sides
   // Please see
   // https://github.com/madsci1016/Arduino-EasyTransfer
   struct EasyTransferI2C
   {
    private:
      // Variable used for I2C Communication
      Pi::I2C const m_comm;

      using byte = std::uint8_t;

    public:
      // Requires the Unique Address to be set for this Object
      // This Address must be same on both sides
      // And not clash with any other I2C Device connected
      // On Both sides
      EasyTransferI2C(byte const p_address_i2c) : m_comm{p_address_i2c} {}

      // Remember to send small sized data
      // The Data Elements on both sides must be of the same type
      // And in the Same Order
      template <typename T>
      void write(T& p_val) const noexcept
      {
         std::uint8_t static constexpr const size = sizeof(T);

         byte* as_byte = static_cast<byte*>(&p_val);

         write(as_byte, size);
      }
      // Remember to send small sized data
      void write(byte* p_data, std::uint8_t const p_size) const noexcept
      {
         // Send the given Header
         // Library uses Header to Verify Type of Values
         // Coming
         m_comm.write(0x06);
         m_comm.write(0x85);

         // Library uses following methods to calculate/find checksum
         byte check_sum = p_size;
         // Converts the Type to Bytes
         // Then Sends respective bytes to other side
         for (std::uint8_t i = 0; i < p_size; ++i)
         {
            byte const elem = p_data[i];
            // Library uses this to calculate checksum
            check_sum ^= elem;
            // Send 1 Byte over
            m_comm.write(elem);
         }
         // Library sends CheckSum over for Future Verification
         m_comm.write(check_sum);
      }
   };
} // namespace Pi