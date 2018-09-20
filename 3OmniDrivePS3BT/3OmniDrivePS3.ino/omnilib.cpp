
#include "omnilib.h"

void B_Wheel::SetWheel(int WDegrees, int MSpeed)
{
  Degrees = WDegrees;
  ServoPin = MSpeed;

#ifdef DEBUG_PLATFORM
  Serial.print("B_Wheel::SetWheel");
  Serial.print("  ");
  Serial.print(WDegrees);
  Serial.print(" :: ");
  Serial.print(MSpeed);
#endif
}


//=======================================================
// implementation of Driver_Wheel functions
//=======================================================

Driver_Wheel::Driver_Wheel()
{
  //  B_Wheel();
  AN1 = 0;
  AN2 = 0;
}

Driver_Wheel::Driver_Wheel(int WDegrees, int SChannel, int iOffset, int A1, int A2)
{
  SetWheel(WDegrees, SChannel, iOffset, A1, A2);
}


void Driver_Wheel::SetWheel(int WDegrees, int SChannel, int iOffset, int A1, int A2)
{
  SetDegrees(WDegrees);
  SetSChannel(SChannel);
  SetOffset(iOffset);
  AN1 = A1;
  AN2 = A2;

#ifdef DEBUG_PLATFORM
  Serial.print("Driver_Wheel::SetWheel");
  Serial.print("  ");
  Serial.print(WDegrees);
  Serial.print(" :: ");
  Serial.print(SChannel);
  Serial.print(" :: ");
  Serial.print(iOffset);
  Serial.print(" :: ");
  Serial.print(A1);
  Serial.print(" :: ");
  Serial.print(A2);
  Serial.print(" :: ");
#endif

}

void Driver_Wheel::ServoWrite(int Speed)
{
  bool dir1 = HIGH;
  bool dir2 = LOW;


  if (Speed < 0)
  {
    dir1 = LOW;
    dir2 = HIGH;
    Speed = -Speed;
  }

  //
  //We determine the turning direction an then set the outputs accordingly
  //

  digitalWrite(AN1, dir1);
  digitalWrite(AN2, dir2);

  //
  // then we set the speed
  //
  analogWrite(ReadSChannel(), Speed);

#ifdef DEBUG_PLATFORM
  Serial.print("Driver_Wheel::ServoWrite");
  Serial.print("  ");
  Serial.print(Speed);
  Serial.print(" :: ");
#endif

}

//=======================================================
// implementation of Platform functions
//=======================================================



Platform::Platform(int NW, int ena)
{
  NoWheels = NW;
  O_Enable = ena;
}

signed char Platform::AddWheel(int NrW, B_Wheel *W)
{
  int i;

  if ((NrW < 0) || (NrW > 3)) return ERR_ILLEGAL_WHEEL_NR;

  MWheel[NrW] = W;

  NoWheels = max(NrW + 1, NoWheels);

#ifdef DEBUG_PLATFORM
  for (i = 0; i < NoWheels; i++)
  {
    Serial.println("Platform::SetWheel");
    Serial.print("WheelNr ");
    Serial.print(i);
    Serial.print(" : WDegrees ");
    Serial.print(MWheel[i]->ReadDegrees());
    Serial.print(" : SChannel ");
    Serial.print(MWheel[i]->ReadSChannel());
    Serial.println();
  }
#endif
}

void Platform::Disable()
{
  int i;
  for (i = 0; i < NoWheels; i++) MWheel[i]->Disable();
  _enabled = false;

  digitalWrite(O_Enable, LOW);


#ifdef DEBUG_PLATFORM
  Serial.println("Platform::Disable");
#endif
}

void Platform::Enable()
{
  int i;

  if (!_enabled)
    for (i = 0; i < NoWheels; i++) MWheel[i]->Enable();

  _enabled = true;

  digitalWrite(O_Enable, HIGH);

#ifdef DEBUG_PLATFORM
  Serial.println("Platform::Enable");
#endif
}

void Platform::Move(float Vel, float Dir, float Turn)
{
  int i;
  float mSpd[3], deg, maxV, ms;

  switch (NoWheels)
  {
    case 3:
      {
        Vel = Vel * 2 / 3;
        maxV = 0;
        for (i = 0; i < 3; i++)
        {

          //
          //      Calculation of each individual speed
          //      Have a look on my blog to better understand this calculation
          //

          deg = MWheel[i]->ReadDegrees();
          mSpd[i] = (cos( (deg - Dir) * M_PI / 180 ) * Vel) + Turn;
          ms = abs(mSpd[i]);
          maxV = max(maxV, ms);
        }

        //
        //    I take care not request a speed higher than 255
        //
        if (maxV > 255)
        {

          //
          //      if a speed higher than 255 is found, then we normalize all the speeds accordingly
          //
          maxV = maxV / 255;
          for (i = 0; i < 3; i++)
          {
            mSpd[i] = mSpd[i] / maxV;
          }
        }

        //
        //   Then we ask the wheels to move!
        //
        for (i = 0; i < 3; i++) {
          
          MWheel[i]->Move(mSpd[i]);
          Serial.println(mSpd[i]);
        }
      }
      break;

    case 4:
      {
        // to be programmed... it's quite easy!
      }

      _enabled = false;


  }

#ifdef DEBUG_PLATFORM
  Serial.println("Platform::Move");
  Serial.print("Vel ");
  Serial.print(Vel);
  Serial.print(" : Dir ");
  Serial.print(Dir);
  Serial.print(" : Turn ");
  Serial.print(Turn);
  Serial.println();
#endif


}

Omni_Wheel::Omni_Wheel()
{
  //  B_Wheel();
  AN1 = 0;
}

Omni_Wheel::Omni_Wheel(int WDegrees, int SChannel, int iOffset, int A1)
{
  SetWheel(WDegrees, SChannel, iOffset, A1);
}


void Omni_Wheel::SetWheel(int WDegrees, int SChannel, int iOffset, int A1)
{
  SetDegrees(WDegrees);
  SetSChannel(SChannel);
  SetOffset(iOffset);
  AN1 = A1;


#ifdef DEBUG_PLATFORM
  Serial.print("Driver_Wheel::SetWheel");
  Serial.print("  ");
  Serial.print(WDegrees);
  Serial.print(" :: ");
  Serial.print(SChannel);
  Serial.print(" :: ");
  Serial.print(iOffset);
  Serial.print(" :: ");
  Serial.print(A1);

#endif

}

void Omni_Wheel::ServoWrite(int Speed)
{
  bool dir = HIGH;


  if (Speed < 0)
  {
    dir = LOW;
    Speed = -Speed;
  }

  //
  //We determine the turning direction an then set the outputs accordingly
  //

  digitalWrite(AN1, dir);

  //
  // then we set the speed
  //
  Serial.print("PWMIS : ");
  Serial.print(AN1);
  Serial.print("\t");
  Serial.print(dir == LOW ? "LOW" : "HIGH");
  Serial.print("\t");	
  Serial.println(Speed);
  analogWrite(ReadSChannel(), Speed);

#ifdef DEBUG_PLATFORM
  Serial.print("Driver_Wheel::ServoWrite");
  Serial.print("  ");
  Serial.print(Speed);
  Serial.print(" :: ");
#endif

}
