#pragma once

template <size_t, size_t>
struct Piston
{
   static_assert(false, "This Piston is Yet to be implemented");
};

template <>
struct Piston<5, 3>
{
   using Pin = byte;

 private:
    Pin const Up;
    Pin const Down;

 public:
   Piston(Pin const p_up, Pin const p_down) : Up{p_up}, Down{p_down}
   {
      pinMode(Up, OUTPUT);
      pinMode(Down, OUTPUT);
   }
   void MoveRaw(byte const p_up, byte const p_down) const
   {
      digitalWrite(Up, p_up);
      digitalWrite(Down, p_down);
   }
   // If the Piston does not go up
   // Invert Pins
   void MoveUp() const
   {
      MoveRaw(HIGH, LOW);
   }
   // If the Piston does not go down
   // Invert Pins
   void MoveDown() const
   {
      MoveRaw(LOW, HIGH);
   }
   // Halt Both Legs
   void Halt(byte const p_halt = HIGH/*Default Halt Value*/) const
   {
      MoveRaw(p_halt, p_halt);
   }
};