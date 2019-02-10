#pragma once

#include <Encoder.h>

struct Encoders
{
 private:
   Encoder m_fl;
   Encoder m_fr;
   Encoder m_bl;
   Encoder m_br;

   using Pin = byte;

 public:
   Encoders(Encoder&& p_fl, Encoder&& p_fr, Encoder&& p_bl, Encoder&& p_br) :
       m_fl{p_fl}, m_fr{p_fr}, m_bl{p_bl}, m_br{p_br}
   {
   }

   int32_t FL()
   {
      return m_fl.read();
   }
   void FL(int32_t const counts)
   {
      m_fl.write(counts);
   }
   int32_t FR()
   {
      return m_fr.read();
   }
   void FR(int32_t const counts)
   {
      m_fr.write(counts);
   }
   int32_t BL()
   {
      return m_bl.read();
   }
   void BL(int32_t const counts)
   {
      m_bl.write(counts);
   }
   int32_t BR()
   {
      return m_br.read();
   }
   void BR(int32_t const counts)
   {
      m_br.write(counts);
   }

   // Difference in FLBR Readings
   int32_t FLBRDiff()
   {
      return FL() - BR();
   }
   // Difference in FRBL Readings
   int32_t FRBLDiff()
   {
      return FR() - BL();
   }

   void Reset(int32_t const counts = 0)
   {
      FL(counts);
      FR(counts);
      BL(counts);
      BR(counts);
   }
};