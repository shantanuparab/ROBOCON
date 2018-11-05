// The Sensor9250 Sample Code Library
// Use this to Access the Readings of the 9250 Sensor
// The Readings are Parsed Using the Madgwick Filter

// Header File to Include
#include <SensorMPU9250.h>

// The Angle Sensor Object
// We Can Pass Different Parameters to the Constructor such as
// Sample Rate (in Hz)
// Gyro Full Scale Range
// Accel Full Scale Range
// Sensors Used
// LPF Rate (in Hz)
SensorMPU9250 g_angle_sensor;

// Use this Pin for Interrupts
const uint8_t g_interrupt_pin = 2;

// The ISR Function called Whenever the Interrupt is Triggered	
void AngleISR()
{
	// The ISR Function must always call this
	// To Update Angle Reading
	g_angle_sensor.CallFromISR();
}

void setup()
{
   pinMode(g_interrupt_pin, INPUT);
   // Loop till Angle Sensor does not begin
   while (!g_angle_sensor.begin())
   {
      Serial.println("Unable to Connect to MPU9250");
      delay(1000 /*1 Second Delay*/);
      Serial.println("Retrying...");
   }
	g_angle_sensor.EnableInterrupt(g_interrupt_pin, AngleISR, FALLING);
}

// Add the main program code into the continuous loop() function
void loop()
{
	// Read the Angle Value from the Sensor
   AngleType angle = g_angle_sensor.ReadAngle();

	// Display the Angle
   Serial.print("Roll:\t");
   Serial.print(angle.Roll);
   Serial.print("Pitch:\t");
   Serial.print(angle.Pitch);
   Serial.print("Yaw:\t");
   Serial.print(angle.Yaw);
   Serial.println();
}
