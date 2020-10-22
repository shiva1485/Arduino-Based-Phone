void setup()
{
  Serial.begin(9600);
}

void loop()
{
  int volt = analogRead(A5);
  double voltage = map(volt, 0, 1023, 0, 2500);

  voltage /= 100;
  Serial.print("Voltage: ");
  Serial.print(voltage);
  Serial.println("V");
  delay(500);
}
