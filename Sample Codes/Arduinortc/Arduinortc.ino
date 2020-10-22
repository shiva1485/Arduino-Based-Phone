#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;

#include <DS3231.h>
DS3231  rtc(SDA, SCL);
Time  t;
int activate;

#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF
#define GREY 0x8410

void setup()
{
  Serial.begin(9600);
  uint16_t ID = tft.readID();
  Serial.print("TFT ID = 0x");
  Serial.println(ID, HEX);
  rtc.begin();
  if (ID == 0xD3D3) ID = 0x9486; // write-only shield
  tft.begin(ID);
  tft.setRotation(0);
  
  tft.fillScreen(BLACK);
}

void loop()
{
  t = rtc.getTime();

  tft.fillRect(170, 50, 50, 50, BLACK);


  // update
  if (t.sec <= 1) {
    tft.fillRect(0,40,200,200,BLACK);
  }
  // Day
  tft.setCursor(50, 180);
  tft.setTextSize(4);
  tft.setTextColor(CYAN);
  tft.print(rtc.getDOWStr());


  //time
  tft.setCursor(30, 60);
  tft.setTextSize(4);
  tft.setTextColor(GREEN);

  if (t.hour >= 12) {
    activate = 1;
    t.hour = t.hour - 12;
    if (t.hour == 0) {
      t.hour = 12; // Day 12 PM
    }
    if (t.hour <= 9) {
      tft.print("0");
      tft.print(t.hour);
      tft.print(":");
    }
    else {
      tft.print(t.hour);
      tft.print(":");
    }
  }
  else {
    activate = 0;
    if (t.hour == 0) {
      t.hour = 12; // Night 12 AM
    }
    if (t.hour <= 9) {
      tft.print("0");
      tft.print(t.hour);
      tft.print(":");
    }
    else {
      tft.print(t.hour);
      tft.print(":");
    }
  }

  if (t.min <= 9) {
    tft.print("0");
    tft.print(t.min);
    tft.print(":");
  }
  else {
    tft.print(t.min);
    tft.print(":");
  }

  if (t.sec <= 9) {
    tft.print("0");
    tft.print(t.sec);
    tft.print(" ");
  }
  else {
    tft.print(t.sec);
    tft.print(" ");
  }

  //AM/PM
  if (activate == 1) {
    tft.print("AM");
  } else {
    tft.print("PM");
  }

  //Date
  tft.setCursor(30, 120);
  tft.setTextColor(YELLOW);
  if (t.date <= 9) {
    tft.print("0");
    tft.print(t.date);
    tft.print(" ");
  }
  else {
    tft.print(t.date);
    tft.print(" ");
  }

  switch (t.mon)
  {
    case 1:
      tft.print("JAN");
      break;
    case 2:
      tft.print("FEB");
      break;
    case 3:
      tft.print("MAR");
      break;
    case 4:
      tft.print("APR");
      break;
    case 5:
      tft.print("MAY");
      break;
    case 6:
      tft.print("JUN");
      break;
    case 7:
      tft.print("JUL");
      break;
    case 8:
      tft.print("AUG");
      break;
    case 9:
      tft.print("SEP");
      break;
    case 10:
      tft.print("OCT");
      break;
    case 11:
      tft.print("NOV");
      break;
    case 12:
      tft.print("DEC");
      break;
  }

  tft.setCursor(200, 120);
  tft.print(t.year);
  delay(1000);

}
