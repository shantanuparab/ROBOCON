void debug() {
  Serial.print(Lx);
  Serial.print(",");
  Serial.print(Ly);
  Serial.print(",");
  Serial.print(Rx);
  Serial.print(",");
  Serial.print(Ry);
  Serial.print(",");
  Serial.print(R);
  Serial.print("\trotation = ");
  Serial.print(rotation);
  Serial.print("\trotationStatus = ");
  Serial.println(rotateOn);
}

