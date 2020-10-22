#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include <SD.h>
#include <SPI.h>
#include <TouchScreen.h>
#define MINPRESSURE 200
#define MAXPRESSURE 1000

const int XP = 8, XM = A2, YP = A3, YM = 9; //320x480 ID=0x9486
const int TS_LEFT = 119, TS_RT = 910, TS_TOP = 955, TS_BOT = 84;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

int pixel_x, pixel_y;     //Touch_getXY() updates global vars
bool Touch_getXY(void)
{
  TSPoint p = ts.getPoint();
  pinMode(YP, OUTPUT);      //restore shared pins
  pinMode(XM, OUTPUT);
  digitalWrite(YP, HIGH);   //because TFT control pins
  digitalWrite(XM, HIGH);
  bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
  if (pressed) {
    pixel_x = map(p.x, TS_LEFT, TS_RT, 0, tft.width()); //.kbv makes sense to me
    pixel_y = map(p.y, TS_TOP, TS_BOT, 0, tft.height());
  }
  return pressed;
}

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define AQUA    0x5D1C
#define TEST    0xE75D

int s = 0;
int h = 12;
int m = 54;
int flag = 0;
void setup()
{
  tft.begin(0x9486);
  Serial.begin(9600);
}

void loop()
{
  s = s + 1;
  delay(1000);

  if (s == 60)
  {
    m = m + 1;
    s = 0;
  }

  if (m == 60)
  {
    m = 0;
    h = h + 1;
    flag = flag + 1;
  }

  if (h == 13)
  {
    h = 1;
    flag = 0;
  }

  Serial.print(h);
  Serial.print(":");
  Serial.print(m);
  Serial.print(":");
  Serial.print(s);

  if (flag < 12) Serial.println(" AM");
  if (flag == 12) Serial.println(" PM");
  if (flag > 12) Serial.println(" PM");
  if (flag == 24) flag = 0;
}
