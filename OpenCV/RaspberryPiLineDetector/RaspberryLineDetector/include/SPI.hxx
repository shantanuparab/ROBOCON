#pragma once

#include <algorithm>
#include <cstdint>
#include <memory>
#include <stdexcept>

// Use this to access RaspberryPi SPI
#include <wiringPiSPI.h>

namespace Pi
{
   struct SPI
   {
    private:
      // This API Primarily relies on Data Hiding
      // The Hidden Value is the SPI Handle
      // To the Given File
      const int m_spi_channel;

    public:
      SPI(const int channel, const int speed, const int mode = 0) : m_spi_channel{channel}
      {
         // Returns -1 as Handle on Error
         if (wiringPiSPISetupMode(channel, speed, mode) == -1)
            throw std::runtime_error("Unable to Access I2C Device Handle");
      }
      bool write(std::uint8_t* p_data, const std::uint8_t length)
      {
         return (wiringPiSPIDataRW(m_spi_channel, p_data, length) >= 0);
      }
      bool read(std::uint8_t* p_data, const std::uint8_t length)
      {
         return (wiringPiSPIDataRW(m_spi_channel, p_data, length) >= 0);
      }
      template <typename Integral, typename = std::enable_if_t<std::is_integral<Integral>::value>>
      bool write(const Integral p_no)
      {
         constexpr const std::uint8_t size = 1 /*Size Byte*/ + 1 /*Sign Byte*/ + 
			 sizeof(Integral);

         std::uint8_t buf[size];

         buf[0] = sizeof(Integral);
         buf[1] = (p_no < 0) ? 1 /*Negative*/ : 0 /*Positive*/;

         const std::make_unsigned_t<Integral> number =
             ((p_no < 0) ? -p_no /*Negative to Positive*/ : p_no);

         int i = 0;
         for (int j = sizeof(number) - 1; j >= 0; j--)
         {
            buf[2 + i] = static_cast<std::uint8_t>(number >> (8 * j) & 0xff);
            i++;
         }

         return write(buf.get(), size);
      }
   };
} // namespace Pi
