#include <Adafruit_GFX.h>
#include <TouchScreen.h>
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;

const int XP = 8, XM = A2, YP = A3, YM = 9;
const int TS_LEFT = 119, TS_RT = 910, TS_TOP = 955, TS_BOT = 84;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define MINPRESSURE 200
#define MAXPRESSURE 1000

int pixel_x, pixel_y;

bool touch (void)
{
  TSPoint p;

  do {
    p = ts.getPoint();
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
  } while ((p.z < MINPRESSURE ) || (p.z > MAXPRESSURE));
  pixel_x = map(p.x, TS_LEFT, TS_RT, 0, 320);
  pixel_y = map(p.y, TS_TOP, TS_BOT, 0, 480);
}
// Assign human-readable names to some common 16-bit color values:
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


void setup(void)
{
  Serial.begin(9600);
  tft.begin(0x9486);
  tft.fillScreen(BLACK);
  tft.setRotation(0);
  tft.setCursor(0,30);
  tft.setTextSize(3);
  tft.setTextColor(YELLOW);
  tft.print("VOLTAGE:");
}



void loop()
{
  int volt = analogRead(A5);// read the input
  double voltage = map(volt,0,1023, 0, 2500);// map 0-1023 to 0-2500 and add correction offset
  
  voltage /=100;// divide by 100 to get the decimal values

  tft.fillRect(150,30,100,100,BLACK);
  tft.setCursor(150,30);
  tft.setTextSize(3);
  tft.setTextColor(GREEN);
  tft.print(voltage);
  tft.print("V");
}
