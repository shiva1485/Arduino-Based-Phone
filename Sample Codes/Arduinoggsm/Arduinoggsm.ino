#include<SoftwareSerial.h>
SoftwareSerial gsm (3, 4);
void setup()
{
  Serial.begin(9600);
  gsm.begin(9600);
  Serial.println("GSM program started!");
}

void loop()
{
  if (gsm.available())
  {
    Serial.write(gsm.read());
  }
  if (Serial.available())
  {
    byte a = Serial.read();
    if (a == '#')//to send
    {
      gsm.write(0x1a);
    }
    else
    {
      gsm.write(a);
    }
  }
}
