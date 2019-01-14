#pragma once

#include <algorithm>
// Use this to access RaspberryPi SPI
#include <wiringPiSPI.h>

namespace Pi
{
   struct SPI
   {
    private:
      using byte = std::uint8_t;

      // This API Primarily relies on Data Hiding
      // The Hidden Value is the SPI Handle
      // To the Given File
      const int m_spi_channel;

      template <typename Integral,
                typename = typename std::enable_if<std::is_integral<Integral>::value>::type>
      constexpr inline static auto absolute(Integral const p_no) noexcept ->
          typename std::make_unsigned<Integral>::type
      {
         return ((p_no < 0) ? -p_no /*Negative to Positive*/ : p_no);
      }

    public:
      SPI(int const p_channel, int const p_speed, int const p_mode = 0) : m_spi_channel{p_channel}
      {
         // Returns -1 as Handle on Error
         if (wiringPiSPISetupMode(m_spi_channel, p_speed, p_mode) == -1)
            throw std::runtime_error("Unable to Access I2C Device Handle");
      }
      bool read(byte* p_data, std::uint8_t const p_length) noexcept
      {
         return (wiringPiSPIDataRW(m_spi_channel, p_data, p_length) >= 0);
      }
      bool write(byte* p_data, std::uint8_t const p_length) noexcept
      {
         return (wiringPiSPIDataRW(m_spi_channel, p_data, p_length) >= 0);
      }
      template <typename Integral,
                typename = typename std::enable_if<std::is_integral<Integral>::value>::type>
         // Write a Given Integral Value
         // To the Output Stream
      bool write(Integral const p_no) noexcept
      {
         std::uint8_t constexpr const BUF_SIZE = 1 /*Size Byte*/ + 1 /*Sign Byte*/ + sizeof(p_no);

         // Buffer that contains the Bytes to be sent
         byte buf[BUF_SIZE];

         // The Size Bit Contains Number of Bytes Required to Represent Number
         buf[0] = sizeof(p_no);
         // The Sign Bit Contains if the Number is Negative or Positive
         buf[1] = (p_no < 0) ? 1 /*Negative*/ : 0 /*Positive*/;

         // Display Some Debugging Info
         std::cout << "\nSPI Size, Sign, Value is " << buf[0] << ':' << buf[1] << ':' << p_no;

         // Convert to absolute value
         auto const number = absolute(p_no);

         // Performs the Byte Splitting of the
         // Given Number

         for (std::int8_t i = 0, j = sizeof(number) - 1; j >= 0; --j, ++i)
         {
            // CHAR_BIT Specifies the
            // Number of Bits
            // That a Single Byte has
            buf[2 + i] = static_cast<byte>((number >> (CHAR_BIT * j)) & 0xff);
         }
         // Writes the Entire Buf Array to SPI Channel
         return write(buf, BUF_SIZE);
      }
   };
} // namespace Pi
