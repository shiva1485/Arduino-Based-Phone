/*
  MIT License

  Copyright (c) 2020 ARDUINOTECH

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

   video: https://youtu.be/wpyd6bKWxIQ
   Instructions : https://www.instructables.com/Arduino-Based-Phone-Prototype/
   This code is about how to make phone.
   Remmber than you need to use 3.5 inch mcu friend tft display.
   And also calibrate the display to use the code.
   Also change the display driver name.
*/
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include <SD.h>
#include <SPI.h>
#include <DS3231.h>
#include <TouchScreen.h>
#define MINPRESSURE 20
#define MAXPRESSURE 1000

DS3231  rtc(SDA, SCL);
Time  t;
int activate;

const int XP=8,XM=A2,YP=A3,YM=9; //320x480 ID=0x9486
const int TS_LEFT=109,TS_RT=904,TS_TOP=944,TS_BOT=84; //calibrate the lcd values here.

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

int pixel_x, pixel_y;     //Touch_getXY() updates global vars
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

#define BLACK   0x0000
#define BLUE    0x051D
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define AQUA    0x5D1C
#define PAUSED  0x18E5
#define CLOCK   0x3DDF
#define TEST    0xE75D
#define BLUE1    0x001F
#define TEST1   0x2124//side color
#define VOLUME  0x047C// ball color
#define TEST2   0x1A0D//slider color

# define Start_Byte 0x7E
# define Version_Byte 0xFF
# define Command_Length 0x06
# define End_Byte 0xEF
# define Acknowledge 0x00 //Returns info with command 0x41 [0x01: info, 0x00: no info]
# define ACTIVATED LOW

bool play = false;
bool home = false;
bool calcu = false;
bool radio = false;
bool video = false;
bool notes = false;
bool more = false;
bool settings = false;
bool volume = false;
bool about = false;
bool language = false;
bool enable = false;
bool battery = false;
bool wallpaper = false;
bool restart = false;
bool clock = false;
bool phone = false;
bool call = false;
bool photos = false;
bool maps = false;
bool music = false;
bool like = false;
bool musicx = false;
bool musicy = false;
bool pause = false;
bool shuffle = false;
bool repeat = false;
bool photoy = false;
bool photosz = false;
bool mapy = false;
bool mapz = false;

int photox = 0;
int mapx = 0;
int s = 0;
int pic = 0;
int PENRADIUS = 4;
int currentcolor;
int xpos;
int ypos;

float Num1, Num2, Number;
char op;
boolean answer = false;

int onetime = 0;
int value = 15;

int no;
String number = "";
int count = 1;

void setup(void)
{
  pinMode(49, OUTPUT);//shutdown
  pinMode(46, OUTPUT); //buzzer
  pinMode(48, OUTPUT);//restart
  pinMode(47, OUTPUT);//led pin

  digitalWrite(48, LOW);
  digitalWrite(49, LOW);
  digitalWrite(46, LOW);
  digitalWrite(47, HIGH);
  delay(500);
  digitalWrite(47, LOW);

  Serial.begin(9600);
  Serial1.begin(9600);//df player mini
  Serial3.begin(9600);//gsm
  tft.begin(0x9486);;//change the driver name in accordence to your display driver.
  if (!SD.begin(53))
  {
    digitalWrite(46, HIGH);
    delay(1000);
    digitalWrite(46,LOW);
    tft.fillScreen(BLUE1);
    tft.setCursor(10, 10);
    tft.setTextSize(2);
    tft.setTextColor(WHITE);
    tft.print("SD card Failed!");
    return;
  }
  rtc.begin();
  tft.setRotation(0);            //PORTRAIT
  tft.fillScreen(BLACK);
  bmpDraw("logo.bmp", 0, 0);
  delay(1000);
  tft.fillScreen(BLACK);
  bmpDraw("home.bmp", 0, 0);

  execute_CMD(0x3F, 0, 0);//get info
  execute_CMD(0x06, 0, value);// Set the volume (0x00~0x30)

  calcu = true;
  radio = true;
  video = true;
  notes = true;
  more = true;
  settings = true;
  volume = true;
  language = true;
  about = true;
  battery = true;
  wallpaper = true;
  restart = true;
  clock = true;
  phone = true;
  call = true;
  photos = true;
  maps = true;
  music = true;
  pause = true;
  shuffle = true;
  repeat = true;
  like = true;
  musicx = true;
  photoy = true;
  mapy = true;

  currentcolor = RED;
}

void loop()
{
  touch();

  //calculator sector
  if (pixel_x > 12 && pixel_x < 73 && pixel_y > 29 && pixel_y < 90)//calculator icon is pressed.
  {
    if (home == false  && calcu == true)
    {
      tft.fillScreen(BLACK);
      bmpDraw("calcu.bmp", 0, 0);
      home = true;
      calcu = false;
    }
  }
  else if (pixel_x > 8 && pixel_x < 76 && pixel_y > 7 && pixel_y < 30)//back to home.
  {
    if (home == true && calcu == false)
    {
      tft.fillScreen(BLACK);
      home = false;
      calcu = true;
      bmpDraw("home.bmp", 0, 0);
    }
  }

  else if (home == true && calcu == false)//calculator function
  {
    if (pixel_x > 13 && pixel_x < 79 && pixel_y > 156 && pixel_y < 211)
    {
      //Serial.println ("Cancel Button");
      Number = Num1 = Num2 = 0;
      answer = false;
      delay(300);
      tft.fillRect(10, 50, 300, 80, WHITE);  //clear result box
      tft.setCursor(10, 50);
      tft.setTextSize(3);
      tft.setTextColor(BLACK);
      tft.println(Number); //update new value
    }
    else if (pixel_x > 13 && pixel_x < 79 && pixel_y > 351 && pixel_y < 406) //If Button 1 is pressed
    {
      //Serial.println ("Button 1");
      if (Number == 0)
      {
        Number = 1;
        tft.fillRect(10, 50, 300, 80, WHITE);  //clear result box
        tft.setCursor(10, 50);
        tft.setTextSize(3);
        tft.setTextColor(BLACK);
        tft.println(Number); //update new value
        delay(300);
      }
      else
      {
        Number = (Number * 10) + 1; //Pressed twice
        tft.fillRect(10, 50, 300, 80, WHITE);  //clear result box
        tft.setCursor(10, 50);
        tft.setTextSize(3);
        tft.setTextColor(BLACK);
        tft.println(Number); //update new value
        delay(300);
      }
    }

    else if (pixel_x > 13 && pixel_x < 79 && pixel_y > 285 && pixel_y < 340) //If Button 4 is pressed
    {
      //Serial.println ("Button 4");
      if (Number == 0)
      {
        Number = 4;
        tft.fillRect(10, 50, 300, 80, WHITE);  //clear result box
        tft.setCursor(10, 50);
        tft.setTextSize(3);
        tft.setTextColor(BLACK);
        tft.println(Number); //update new value
        delay(300);
      }
      else
      {
        Number = (Number * 10) + 4; //Serial.println ("Button 1");
        tft.fillRect(10, 50, 300, 80, WHITE);  //clear result box
        tft.setCursor(10, 50);
        tft.setTextSize(3);
        tft.setTextColor(BLACK);
        tft.println(Number); //update new value
        delay(300);
      }//Pressed twice
    }

    else if (pixel_x > 13 && pixel_x < 79 && pixel_y > 219 && pixel_y < 274) //If Button 7 is pressed
    {
      //Serial.println ("Button 7");
      if (Number == 0)
      {
        Number = 7;
        tft.fillRect(10, 50, 300, 80, WHITE);  //clear result box
        tft.setCursor(10, 50);
        tft.setTextSize(3);
        tft.setTextColor(BLACK);
        tft.println(Number); //update new value
        delay(300);
      }
      else
      {
        Number = (Number * 10) + 7; //Pressed twice
        tft.fillRect(10, 50, 300, 80, WHITE);  //clear result box
        tft.setCursor(10, 50);
        tft.setTextSize(3);
        tft.setTextColor(BLACK);
        tft.println(Number); //update new value
        delay(300);
      }
    }

    else if (pixel_x > 13 && pixel_x < 154 && pixel_y > 415 && pixel_y < 472)
    {
      //Serial.println ("Button 0"); //Button 0 is Pressed
      if (Number == 0)
      {
        Number = 0;
        tft.fillRect(10, 50, 300, 80, WHITE);  //clear result box
        tft.setCursor(10, 50);
        tft.setTextSize(3);
        tft.setTextColor(BLACK);
        tft.println(Number); //update new value
        delay(300);
      }
      else
      {
        Number = (Number * 10) + 0; //Pressed twice
        tft.fillRect(10, 50, 300, 80, WHITE);  //clear result box
        tft.setCursor(10, 50);
        tft.setTextSize(3);
        tft.setTextColor(BLACK);
        tft.println(Number); //update new value
        delay(300);
      }
    }

    else if (pixel_x > 90 && pixel_x < 156 && pixel_y > 351 && pixel_y < 406)
    {
      //Serial.println ("Button 2");
      if (Number == 0)
      {
        Number = 2;
        tft.fillRect(10, 50, 300, 80, WHITE);  //clear result box
        tft.setCursor(10, 50);
        tft.setTextSize(3);
        tft.setTextColor(BLACK);
        tft.println(Number); //update new value
        delay(300);
      }
      else
      {
        Number = (Number * 10) + 2; //Pressed twice
        tft.fillRect(10, 50, 300, 80, WHITE);  //clear result box
        tft.setCursor(10, 50);
        tft.setTextSize(3);
        tft.setTextColor(BLACK);
        tft.println(Number); //update new value
        delay(300);
      }
    }

    else if (pixel_x > 90 && pixel_x < 156 && pixel_y > 285 && pixel_y < 340)
    {
      //Serial.println ("Button 5");
      if (Number == 0)
      {
        Number = 5;
        tft.fillRect(10, 50, 300, 80, WHITE);  //clear result box
        tft.setCursor(10, 50);
        tft.setTextSize(3);
        tft.setTextColor(BLACK);
        tft.println(Number); //update new value
        delay(300);
      }
      else
      {
        Number = (Number * 10) + 5; //Pressed twic
        tft.fillRect(10, 50, 300, 80, WHITE);  //clear result box
        tft.setCursor(10, 50);
        tft.setTextSize(3);
        tft.setTextColor(BLACK);
        tft.println(Number); //update new value
        delay(300);
      }
    }

    else if (pixel_x > 90 && pixel_x < 156 && pixel_y > 219 && pixel_y < 274)
    {
      //Serial.println ("Button 8");
      if (Number == 0)
      {
        Number = 8;
        tft.fillRect(10, 50, 300, 80, WHITE);  //clear result box
        tft.setCursor(10, 50);
        tft.setTextSize(3);
        tft.setTextColor(BLACK);
        tft.println(Number); //update new value
        delay(300);
      }
      else
      {
        Number = (Number * 10) + 8; //Pressed twic
        tft.fillRect(10, 50, 300, 80, WHITE);  //clear result box
        tft.setCursor(10, 50);
        tft.setTextSize(3);
        tft.setTextColor(BLACK);
        tft.println(Number); //update new value
      }
    }

    else if (pixel_x > 167 && pixel_x < 222 && pixel_y > 351 && pixel_y < 406)
    {
      //Serial.println ("Button 3");
      if (Number == 0)
      {
        Number = 3;
        tft.fillRect(10, 50, 300, 80, WHITE);  //clear result box
        tft.setCursor(10, 50);
        tft.setTextSize(3);
        tft.setTextColor(BLACK);
        tft.println(Number); //update new value
        delay(300);
      }
      else
      {
        Number = (Number * 10) + 3; //Pressed twice
        tft.fillRect(10, 50, 300, 80, WHITE);  //clear result box
        tft.setCursor(10, 50);
        tft.setTextSize(3);
        tft.setTextColor(BLACK);
        tft.println(Number); //update new value
        delay(300);
      }
    }

    else if (pixel_x > 167 && pixel_x < 222 && pixel_y > 285 && pixel_y < 340)
    {
      //Serial.println ("Button 6");
      if (Number == 0)
      {
        Number = 6;
        tft.fillRect(10, 50, 300, 80, WHITE);  //clear result box
        tft.setCursor(10, 50);
        tft.setTextSize(3);
        tft.setTextColor(BLACK);
        tft.println(Number); //update new value
        delay(300);
      }
      else
      {
        Number = (Number * 10) + 6; //Pressed twice
        tft.fillRect(10, 50, 300, 80, WHITE);  //clear result box
        tft.setCursor(10, 50);
        tft.setTextSize(3);
        tft.setTextColor(BLACK);
        tft.println(Number); //update new value
        delay(300);
      }
    }

    if (pixel_x > 167 && pixel_x < 222 && pixel_y > 219 && pixel_y < 274)
    {
      //Serial.println ("Button 9");
      if (Number == 0)
      {
        Number = 9;
        tft.fillRect(10, 50, 300, 80, WHITE);  //clear result box
        tft.setCursor(10, 50);
        tft.setTextSize(3);
        tft.setTextColor(BLACK);
        tft.println(Number); //update new value
        delay(300);
      }
      else
      {
        Number = (Number * 10) + 9; //Pressed twice
        tft.fillRect(10, 50, 300, 80, WHITE);  //clear result box
        tft.setCursor(10, 50);
        tft.setTextSize(3);
        tft.setTextColor(BLACK);
        tft.println(Number); //update new value
        delay(300);
      }
    }

    else if (pixel_x > 243 && pixel_x < 309 && pixel_y > 415 && pixel_y < 470) ///Equal to button
    {
      //Serial.println ("Button Equal");
      Num2 = Number;
      answer = true;
      if (answer == true)
      {
        switch (op)
        {
          case 1 :
            Number = Num1 + Num2;
            break;
          case 2:
            Number = Num1 - Num2;
            break;
          case 3:
            Number = Num1 * Num2;
            break;
          case 4:
            Number = Num1 / Num2;
        }
      }
      tft.fillRect(10, 50, 300, 80, WHITE);  //clear result box
      tft.setCursor(10, 50);
      tft.setTextSize(3);
      tft.setTextColor(BLACK);
      tft.println(Number); //update new value
      delay(300);
    }
    else if (pixel_x > 243 && pixel_x < 309) //Detecting Buttons on Column 4
    {

      Num1 = Number;
      Number = 0;

      tft.setCursor(260, 70);
      tft.setTextColor(BLACK);

      if (pixel_y > 351 && pixel_y < 406)
      {
        //Serial.println ("Addition Button");
        op = 1;
        tft.print("+");
        delay(300);
      }

      if (pixel_y > 285 && pixel_y < 340)
      {
        //Serial.println ("subtraction Button");
        op = 2;
        tft.print("-");
        delay(300);
      }

      if (pixel_y > 219 && pixel_y < 274)
      {
        //Serial.println ("multiplication Button");
        op = 3;
        tft.print("*");
        delay(300);
      }

      if (pixel_y > 156 && pixel_y < 211)
      {
        //Serial.println ("Division Button");
        op = 4;
        tft.print("/");
        delay(300);
      }
    }
  }

  //video sector
  if (pixel_x > 16 && pixel_x < 77 && pixel_y > 137 && pixel_y < 198)//video icon is pressed
  {
    if (home == false  && video == true)
    {
      tft.fillScreen(BLACK);
      bmpDraw("topvideo.bmp", 0, 0);
      home = true;
      video = false;
    }
  }

  else if (pixel_x > 6 && pixel_x < 73 && pixel_y > 5 && pixel_y < 31)//back to home
  {
    if (home == true && video == false)
    {
      tft.fillScreen(BLACK);
      home = false;
      video = true;
      bmpDraw("home.bmp", 0, 0);
    }
  }

  if (home == true && video == false)
  {
    if (pixel_x > 237 && pixel_x < 286 && pixel_y > 5 && pixel_y < 31)//play video
    {
      tft.fillRect(70, 50, 180, 320, BLACK);
      play = true;
      delay(300);
    }

    if (play == true)
    {
      bmpDraw("x.bmp", 70, 150);
      bmpDraw("z.bmp", 70, 150);
      bmpDraw("y.bmp", 70, 150);
      play = false;
    }
  }

  //phone sector
  if (pixel_x > 21 && pixel_x < 76 && pixel_y > 421 && pixel_y < 465)//phone icon is pressed
  {
    if (home == false  && phone == true)
    {
      tft.fillScreen(BLACK);
      home = true;
      phone = false;
      bmpDraw("phone1.bmp", 0, 0);
    }
  }

  else if (pixel_x > 0 && pixel_x < 61 && pixel_y > 43 && pixel_y < 71)//back to home
  {
    if (home == true && phone == false)
    {
      tft.fillScreen(BLACK);
      home = false;
      phone = true;
      bmpDraw("home.bmp", 0, 0);
    }
  }

  else if (home == true && phone == false)
  {
    if (pixel_x > 45 && pixel_x < 95 && pixel_y > 184 && pixel_y < 234)// '1' pressed!
    {
      no = 1;
      if (count <= 10 && count <= 13)
      {
        number = number + no;
        count++;
        delay(300);
      }
      tft.fillRect(20, 80, 280, 70, WHITE);
      tft.setCursor(20, 100);
      tft.setTextSize(3);
      tft.setTextColor(BLACK);
      tft.print(number);
    }

    else if (pixel_x > 135 && pixel_x < 185 && pixel_y > 184 && pixel_y < 234)// '2' pressed!
    {
      no = 2;
      if (count <= 10 && count <= 13)
      {
        number = number + no;
        count++;
        delay(300);
      }
      tft.fillRect(20, 80, 280, 70, WHITE);
      tft.setCursor(20, 100);
      tft.setTextSize(3);
      tft.setTextColor(BLACK);
      tft.print(number);
    }

    else if (pixel_x > 227 && pixel_x < 277 && pixel_y > 184 && pixel_y < 234)// '3' pressed!
    {
      no = 3;
      if (count <= 10 && count <= 13)
      {
        number = number + no;
        count++;
        delay(300);
      }
      tft.fillRect(20, 80, 280, 70, WHITE);
      tft.setCursor(20, 100);
      tft.setTextSize(3);
      tft.setTextColor(BLACK);
      tft.print(number);
    }

    else if (pixel_x > 45 && pixel_x < 95 && pixel_y > 237 && pixel_y < 287)// '4' pressed!
    {
      no = 4;
      if (count <= 10 && count <= 13)
      {
        number = number + no;
        count++;
        delay(300);
      }
      tft.fillRect(20, 80, 280, 70, WHITE);
      tft.setCursor(20, 100);
      tft.setTextSize(3);
      tft.setTextColor(BLACK);
      tft.print(number);
    }

    else if (pixel_x > 135 && pixel_x < 185 && pixel_y > 237 && pixel_y < 287)// '5' pressed!
    {
      no = 5;
      if (count <= 10 && count <= 13)
      {
        number = number + no;
        count++;
        delay(300);
      }
      tft.fillRect(20, 80, 280, 70, WHITE);
      tft.setCursor(20, 100);
      tft.setTextSize(3);
      tft.setTextColor(BLACK);
      tft.print(number);
    }

    else if (pixel_x > 227 && pixel_x < 277 && pixel_y > 237 && pixel_y < 287)// '6' pressed!
    {
      no = 6;
      if (count <= 10 && count <= 13)
      {
        number = number + no;
        count++;
        delay(300);
      }
      tft.fillRect(20, 80, 280, 70, WHITE);
      tft.setCursor(20, 100);
      tft.setTextSize(3);
      tft.setTextColor(BLACK);
      tft.print(number);
    }

    else if (pixel_x > 45 && pixel_x < 95 && pixel_y > 290 && pixel_y < 340)// '7' pressed!
    {
      no = 7;
      if (count <= 10 && count <= 13)
      {
        number = number + no;
        count++;
        delay(300);
      }
      tft.fillRect(20, 80, 280, 70, WHITE);
      tft.setCursor(20, 100);
      tft.setTextSize(3);
      tft.setTextColor(BLACK);
      tft.print(number);
    }

    else if (pixel_x > 135 && pixel_x < 185 && pixel_y > 290 && pixel_y < 340)// '8' pressed!
    {
      no = 8;
      if (count <= 10 && count <= 13)
      {
        number = number + no;
        count++;
        delay(300);
      }
      tft.fillRect(20, 80, 280, 70, WHITE);
      tft.setCursor(20, 100);
      tft.setTextSize(3);
      tft.setTextColor(BLACK);
      tft.print(number);
    }

    else if (pixel_x > 227 && pixel_x < 277 && pixel_y > 290 && pixel_y < 340)// '9' pressed!
    {
      no = 9;
      if (count <= 10 && count <= 13)
      {
        number = number + no;
        count++;
        delay(300);
      }
      tft.fillRect(20, 80, 280, 70, WHITE);
      tft.setCursor(20, 100);
      tft.setTextSize(3);
      tft.setTextColor(BLACK);
      tft.print(number);
    }

    else if (pixel_x > 135 && pixel_x < 185 && pixel_y > 349 && pixel_y < 399)// '0' pressed!
    {
      no = 0;
      if (count <= 10 && count <= 13)
      {
        number = number + no;
        count++;
        delay(300);
      }
      tft.fillRect(20, 80, 280, 70, WHITE);
      tft.setCursor(20, 100);
      tft.setTextSize(3);
      tft.setTextColor(BLACK);
      tft.print(number);
    }

    else if (pixel_x > 240 && pixel_x < 265 && pixel_y > 423 && pixel_y < 436)// 'clear' pressed!
    {
      tft.fillRect(20, 80, 280, 70, WHITE);
      count = 0;
      number = "";
      delay(300);
      Serial.println(number);
      tft.setCursor(20, 100);
      tft.setTextSize(3);
      tft.setTextColor(BLACK);
      tft.print(number);
    }

    else if (pixel_x > 111 && pixel_x < 210 && pixel_y > 415 && pixel_y < 447)// 'call' pressed!
    {
      if (count >= 10)
      {
        if (call == true && phone == false )
        {
          tft.fillScreen(BLACK);
          bmpDraw("call.bmp", 0, 0);
          delay(300);
          phone = true;
          call = false;
        }
      }
    }
  }

  else if (pixel_x > 133 && pixel_x < 187 && pixel_y > 412 && pixel_y < 454)//back to phone.
  {
    if (call == false && phone == true)
    {
      tft.fillScreen(BLACK);
      bmpDraw("phone1.bmp", 0, 0);
      call = true;
      phone = false;
    }
  }

  //photos sector
  if (pixel_x > 103 && pixel_x < 158 && pixel_y > 422 && pixel_y < 466)//photos icon pressed
  {
    if (home == false  && photos == true)
    {
      tft.fillScreen(BLACK);
      home = true;
      photos = false;
      bmpDraw("phototop.bmp", 0, 0);
    }
  }

  else if (pixel_x > 6 && pixel_x < 75 && pixel_y > 6 && pixel_y < 30) //back to home
  {
    if (home == true && photos == false)
    {
      tft.fillScreen(BLACK);
      home = false;
      photos = true;
      bmpDraw("home.bmp", 0, 0);
    }
  }

  if (home == true && photos == false)//photos function
  {
    if (pixel_x > 265 && pixel_x < 319 && pixel_y > 11 && pixel_y < 29)
    {
      photox++;
      delay(300);
    }
    if (pixel_x > 213 && pixel_x < 264 && pixel_y > 7 && pixel_y < 33)
    {
      photox--;
      delay(300);
    }

    if (photox <= 0)
    {
      photox = 1;
    }
    else if (photox >= 5)
    {
      photox = 4;
    }
    tft.fillRect(83, 16, 10, 18, TEST);
    tft.setCursor(83, 16);
    tft.setTextSize(2);
    tft.setTextColor(BLACK);
    tft.print(photox);

    switch (photox)
    {
      case 1:
        if (photosz == false && photoy == true)
        {
          tft.fillRect(0, 35, 320, 445, BLACK);
          bmpDraw("photos1.bmp", 0, 35);
          photoy = false;
          photosz = true;
        }
        break;

      case 2:
        if (photosz == true && photoy == false)
        {
          tft.fillRect(0, 35, 320, 445, BLACK);
          bmpDraw("photos2.bmp", 0, 35);
          photosz = false;
          photoy = true;
        }
        break;

      case 3:
        if (photosz == false && photoy == true)
        {
          tft.fillRect(0, 35, 320, 445, BLACK);
          bmpDraw("photos3.bmp", 0, 35);
          photosz = true;
          photoy = false;
        }
        break;
      case 4:
        if (photosz == true && photoy == false)
        {
          tft.fillRect(0, 35, 320, 445, BLACK);
          bmpDraw("photos4.bmp", 0, 35);
          photosz = false;
          photoy = true;
        }
        break;
    }
  }

  //Maps sector
  if (pixel_x > 179 && pixel_x < 234 && pixel_y > 418 && pixel_y < 462)//maps icon is pressed
  {
    if (home == false  && maps == true)
    {
      tft.fillScreen(BLACK);
      home = true;
      maps = false;
      bmpDraw("mapstop.bmp", 0, 0);
    }
  }

  else if (pixel_x > 4 && pixel_x < 70 && pixel_y > 4 && pixel_y < 29) //back to home
  {
    if (home == true && maps == false)
    {
      tft.fillScreen(BLACK);
      home = false;
      maps = true;
      bmpDraw("home.bmp", 0, 0);
    }
  }

  if (home == true && maps == false)//maps function
  {
    if (pixel_x > 264 && pixel_x < 318 && pixel_y > 7 && pixel_y < 28)
    {
      mapx++;
      delay(300);
    }
    if (pixel_x > 196 && pixel_x < 256 && pixel_y > 7 && pixel_y < 32)
    {
      mapx--;
      delay(300);
    }

    if (mapx == 0)
    {
      mapx = 1;
    }
    else if (mapx == 19)
    {
      mapx = 18;
    }
    tft.fillRect(83, 16, 28, 18, TEST);
    tft.setCursor(83, 16);
    tft.setTextSize(2);
    tft.setTextColor(BLACK);
    tft.print(mapx);

    switch (mapx)
    {
      case 1:
        if (mapz == false && mapy == true)
        {
          tft.fillRect(0, 35, 320, 445, BLACK);
          bmpDraw("map1.bmp", 0, 35);
          mapy = false;
          mapz = true;
        }
        break;

      case 2:
        if (mapz == true && mapy == false)
        {
          tft.fillRect(0, 35, 320, 445, BLACK);
          bmpDraw("map2.bmp", 0, 35);
          mapz = false;
          mapy = true;
        }
        break;

      case 3:
        if (mapz == false && mapy == true)
        {
          tft.fillRect(0, 35, 320, 445, BLACK);
          bmpDraw("map3.bmp", 0, 35);
          mapz = true;
          mapy = false;
        }
        break;
      case 4:
        if (mapz == true && mapy == false)
        {
          tft.fillRect(0, 35, 320, 445, BLACK);
          bmpDraw("map4.bmp", 0, 35);
          mapz = false;
          mapy = true;
        }
        break;
      case 5:
        if (mapz == false && mapy == true)
        {
          tft.fillRect(0, 35, 320, 445, BLACK);
          bmpDraw("map5.bmp", 0, 35);
          mapz = true;
          mapy = false;
        }
        break;
      case 6:
        if (mapz == true && mapy == false)
        {
          tft.fillRect(0, 35, 320, 445, BLACK);
          bmpDraw("map6.bmp", 0, 35);
          mapz = false;
          mapy = true;
        }
        break;
      case 7:
        if (mapz == false && mapy == true)
        {
          tft.fillRect(0, 35, 320, 445, BLACK);
          bmpDraw("map7.bmp", 0, 35);
          mapz = true;
          mapy = false;
        }
        break;
      case 8:
        if (mapz == true && mapy == false)
        {
          tft.fillRect(0, 35, 320, 445, BLACK);
          bmpDraw("map8.bmp", 0, 35);
          mapz = false;
          mapy = true;
        }
        break;
      case 9:
        if (mapz == false && mapy == true)
        {
          tft.fillRect(0, 35, 320, 445, BLACK);
          bmpDraw("map9.bmp", 0, 35);
          mapz = true;
          mapy = false;
        }
        break;
      case 10:
        if (mapz == true && mapy == false)
        {
          tft.fillRect(0, 35, 320, 445, BLACK);
          bmpDraw("map10.bmp", 0, 35);
          mapz = false;
          mapy = true;
        }
        break;
      case 11:
        if (mapz == false && mapy == true)
        {
          tft.fillRect(0, 35, 320, 445, BLACK);
          bmpDraw("map11.bmp", 0, 35);
          mapz = true;
          mapy = false;
        }
        break;
      case 12:
        if (mapz == true && mapy == false)
        {
          tft.fillRect(0, 35, 320, 445, BLACK);
          bmpDraw("map12.bmp", 0, 35);
          mapz = false;
          mapy = true;
        }
        break;
      case 13:
        if (mapz == false && mapy == true)
        {
          tft.fillRect(0, 35, 320, 445, BLACK);
          bmpDraw("map13.bmp", 0, 35);
          mapz = true;
          mapy = false;
        }
        break;
      case 14:
        if (mapz == true && mapy == false)
        {
          tft.fillRect(0, 35, 320, 445, BLACK);
          bmpDraw("map14.bmp", 0, 35);
          mapz = false;
          mapy = true;
        }
        break;
      case 15:
        if (mapz == false && mapy == true)
        {
          tft.fillRect(0, 35, 320, 445, BLACK);
          bmpDraw("map15.bmp", 0, 35);
          mapz = true;
          mapy = false;
        }
        break;
      case 16:
        if (mapz == true && mapy == false)
        {
          tft.fillRect(0, 35, 320, 445, BLACK);
          bmpDraw("map16.bmp", 0, 35);
          mapz = false;
          mapy = true;
        }
        break;
      case 17:
        if (mapz == false && mapy == true)
        {
          tft.fillRect(0, 35, 320, 445, BLACK);
          bmpDraw("map17.bmp", 0, 35);
          mapz = true;
          mapy = false;
        }
        break;
      case 18:
        if (mapz == true && mapy == false)
        {
          tft.fillRect(0, 35, 320, 445, BLACK);
          bmpDraw("map18.bmp", 0, 35);
          mapz = false;
          mapy = true;
        }
    }
  }

  //Clock sector
  if (pixel_x > 246 && pixel_x < 307 && pixel_y > 30 && pixel_y < 91)//clock icon is pressed
  {
    if (home == false  && clock == true)
    {
      tft.fillScreen(BLACK);
      bmpDraw("clocktop.bmp", 0, 0);
      home = true;
      clock = false;
    }
  }
  else if (pixel_x > 2 && pixel_x < 73 && pixel_y > 5 && pixel_y < 29)//back to home
  {
    if (home == true && clock == false)
    {
      tft.fillScreen(BLACK);
      home = false;
      clock = true;
      bmpDraw("home.bmp", 0, 0);
    }
  }
  if (home == true && clock == false)//clock function
  {
    t = rtc.getTime();
    tft.fillRect(170, 50, 50, 50, BLACK); // seconds update
    tft.fillRect(30, 240, 320, 50, BLACK);//temprature update

    // update
    if (t.sec <= 1)
    {
      tft.fillRect(0, 40, 200, 200, BLACK);
    }

    //Temperature
    tft.setCursor(30, 240);
    tft.setTextColor(CLOCK);
    tft.setTextSize(4);
    tft.print("Temp:");
    tft.print(rtc.getTemp());
    tft.print("C");

    // Day
    tft.setCursor(50, 180);
    tft.setTextSize(4);
    tft.setTextColor(CLOCK);
    tft.print(rtc.getDOWStr());

    //time
    tft.setCursor(30, 60);
    tft.setTextSize(4);
    tft.setTextColor(CLOCK);

    if (t.hour >= 12)
    {
      activate = 1;
      t.hour = t.hour - 12;
      if (t.hour == 0) {
        t.hour = 12; // Day 12 PM
      }
      if (t.hour <= 9)
      {
        tft.print("0");
        tft.print(t.hour);
        tft.print(":");
      }
      else
      {
        tft.print(t.hour);
        tft.print(":");
      }
    }
    else {
      activate = 0;
      if (t.hour == 0)
      {
        t.hour = 12; // Night 12 AM
      }
      if (t.hour <= 9) {
        tft.print("0");
        tft.print(t.hour);
        tft.print(":");
      }
      else
      {
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
      tft.print("PM");
    } else {
      tft.print("AM");
    }

    //Date
    tft.setCursor(30, 120);
    tft.setTextColor(CLOCK);
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

  //Notes Sector
  if (pixel_x > 138 && pixel_x < 199 && pixel_y > 135 && pixel_y < 196)//notes icon is pressed
  {
    if (home == false  && notes == true)
    {
      tft.fillScreen(BLACK);
      bmpDraw("notestop.bmp", 0, 0);
      home = true;
      notes = false;
    }
  }

  else if (pixel_x > 2 && pixel_x < 69 && pixel_y > 7 && pixel_y < 29)//back to home
  {
    if (home == true && notes == false)
    {
      tft.fillScreen(BLACK);
      home = false;
      notes = true;
      bmpDraw("home.bmp", 0, 0);
    }
  }

  if (pixel_x > 192 && pixel_x < 251 && pixel_y > 4 && pixel_y < 30)//in settings
  {
    if (more == true && notes == false)
    {
      tft.fillScreen(BLACK);
      bmpDraw("more.bmp", 0, 0);
      more = false;
      notes = true;
    }
  }

  else if (pixel_x > 4 && pixel_x < 70 && pixel_y > 6 && pixel_y < 34)//back to notes
  {
    if (more == false && notes == true)
    {
      tft.fillScreen(BLACK);
      bmpDraw("notestop.bmp", 0, 0);
      more = true;
      notes = false;
    }
  }
  if (notes == false && home == true)//drawing area
  {
    xpos = pixel_x;
    ypos = pixel_y;
    if (((ypos - PENRADIUS) > 35) && ((ypos + PENRADIUS) < 480))//drawing function
    {
      tft.fillCircle(xpos, ypos, PENRADIUS, currentcolor);
    }

    else if (pixel_x > 262 && pixel_x < 315 && pixel_y > 7 && pixel_y < 32)//clear button
    {
      tft.fillRect(0, 35, 320, 445, BLACK);
    }
  }

  if (more == false && notes == true)
  {
    if (pixel_y > 103 && pixel_y < 145)//selecting color
    {
      if (pixel_x > 11 && pixel_x < 52 )
      {
        currentcolor = RED;
      }
      if (pixel_x > 52 && pixel_x < 94)
      {
        currentcolor = WHITE;
      }
      if (pixel_x > 94 && pixel_x < 136)
      {
        currentcolor = YELLOW;
      }
      if (pixel_x > 136 && pixel_x < 178)
      {
        currentcolor = GREEN;
      }
      if (pixel_x > 178 && pixel_x < 220)
      {
        currentcolor = BLUE;
      }
      if (pixel_x > 220 && pixel_x < 262)
      {
        currentcolor = CYAN;
      }
      if (pixel_x > 262 && pixel_x < 304)
      {
        currentcolor = MAGENTA;
      }
    }


    else if (pixel_y > 257 && pixel_y < 301)//selecting bush thickness
    {
      if (pixel_x > 11 && pixel_x < 65)
      {
        PENRADIUS = 1;
      }
      if (pixel_x > 93 && pixel_x < 147)
      {
        PENRADIUS = 2;
      }
      if (pixel_x > 174 && pixel_x < 228)
      {
        PENRADIUS = 3;
      }
      if (pixel_x > 253 && pixel_x < 307)
      {
        PENRADIUS = 4;
      }
    }
  }

  //Settings Sector
  if (pixel_x > 245 && pixel_x < 306 && pixel_y > 135 && pixel_y < 196) // Settings icon is pressed.
  {
    if (home == false && settings == true)
    {
      tft.fillScreen(BLACK);
      bmpDraw("settings.bmp", 0, 0);
      home = true;
      settings = false;
    }
  }

  else if (pixel_x > 14 && pixel_x < 83 && pixel_y > 10 && pixel_y < 31)// back to home.
  {
    if (home == true && settings == false)
    {
      tft.fillScreen(BLACK);
      bmpDraw("home.bmp", 0, 0);
      home = false;
      settings = true;
    }
  }

  else if (pixel_x > 7 && pixel_x < 314 && pixel_y > 98 && pixel_y < 136)// wallpaper icon is pressed.
  {
    if (settings == false && wallpaper == true)
    {
      tft.fillScreen(BLACK);
      bmpDraw("wall.bmp", 0, 0);
      wallpaper = false;
      settings = true;
    }
  }

  else if (pixel_x > 15 && pixel_x < 119 && pixel_y > 6 && pixel_y < 32)//back to settings.
  {
    if (settings == true && wallpaper == false)
    {
      tft.fillScreen(BLACK);
      bmpDraw("settings.bmp", 0, 0);
      wallpaper = true;
      settings = false;
    }
  }

  else if (pixel_x > 9 && pixel_x < 314 && pixel_y > 149 && pixel_y < 183) //battery icon is pressed.
  {
    if (settings == false && battery == true)
    {
      tft.fillScreen(BLACK);
      bmpDraw("battery.bmp", 0, 0);
      battery = false;
      settings = true;
    }
  }

  else if (pixel_x > 17 && pixel_x < 185 && pixel_y > 9 && pixel_y < 32) //back to settings.
  {
    if (settings == true && battery == false)
    {
      tft.fillScreen(BLACK);
      bmpDraw("settings.bmp", 0, 0);
      battery = true;
      settings = false;
    }
  }

  else if (settings == true && battery == false)
  {
    int volt = analogRead(A7);// read the input
    double voltage = map(volt, 0, 1023, 0, 2500);// map 0-1023 to 0-2500.

    voltage /= 100;// divide by 100 to get the decimal values

    tft.fillRect(21, 115, 102, 15, TEST1);
    tft.setCursor(21, 115);
    tft.setTextSize(1);
    tft.setTextColor(WHITE);
    tft.print(voltage);
    tft.print("V");
    delay(50);
  }

  else if (pixel_x > 10 && pixel_x < 310 && pixel_y > 193 && pixel_y < 227)//sounds icon is pressed.
  {
    if (settings == false && volume == true)
    {
      tft.fillScreen(BLACK);
      bmpDraw("volume.bmp", 0, 0);
      settings = true;
      volume = false;
    }
  }

  else if (pixel_x > 9 && pixel_x < 101 && pixel_y > 14 && pixel_y < 52)//back to home.
  {
    if (settings == true && volume == false)
    {
      tft.fillScreen(BLACK);
      bmpDraw("settings.bmp", 0, 0);
      settings = false;
      volume = true;
    }
  }

  else if (settings == true && volume == false)
  {
    if (pixel_x > 63 && pixel_x < 301 && pixel_y > 134 && pixel_y < 139)//slider function.
    {
      tft.fillRect(61, 129, 243, 5, TEST1);//upside
      tft.fillRect(61, 137, 243, 8, TEST1);//downside
      tft.fillRect(61, 134, 243, 3, TEST2);//middle
      tft.fillRect(61, 134, pixel_x - 60, 3, WHITE);
      tft.fillCircle(pixel_x, pixel_y - 2, 4, VOLUME);

      value = map(pixel_x, 62, 300, 8, 30);
      Serial.println(value);
    }
    execute_CMD(0x06, 0, value);// Set the volume (0x00~0x30)
  }

  else if (pixel_x > 12 && pixel_x < 307 && pixel_y > 244 && pixel_y < 278)//language icon is pressed.
  {
    if (settings == false && language == true)
    {
      tft.fillScreen(BLACK);
      bmpDraw("lang.bmp", 0, 0);
      language = false;
      settings = true;
    }
  }

  else if (pixel_x > 16 && pixel_x < 118 && pixel_y > 12 && pixel_y < 37) //back to settings.
  {
    if (settings == true && language == false)
    {
      tft.fillScreen(BLACK);
      bmpDraw("settings.bmp", 0, 0);
      settings = false;
      language = true;
    }
  }

  else if (settings == true && language == false)
  {
    if (pixel_x > 12 && pixel_x < 311 && pixel_y > 151 && pixel_y < 185)//en(uk)
    {
      tft.fillRect(264, 198, 40, 30, TEST1);//in
      tft.fillRect(258, 237, 40, 30, TEST1);//us
      tft.fillCircle(285, 170, 3, WHITE);
      Serial.println("uk");
    }

    else if (pixel_x > 5 && pixel_x < 316 && pixel_y > 191 && pixel_y < 229)//en(in)
    {
      tft.fillRect(263, 156, 40, 30, TEST1);//uk
      tft.fillRect(258, 237, 40, 30, TEST1);//us
      tft.fillCircle(284, 212, 3, WHITE);
      Serial.println("in");
    }

    else if (pixel_x > 7 && pixel_x < 310 && pixel_y > 236 && pixel_y < 276)//en(us)
    {
      tft.fillRect(263, 156, 40, 30, TEST1);//uk
      tft.fillRect(264, 198, 40, 30, TEST1);//in
      tft.fillCircle(285, 257, 3, WHITE);
      Serial.println("us");
    }

    if (!enable)
    {
      tft.fillCircle(285, 170, 3, WHITE);//uk
      enable = true;
    }
  }

  else if (pixel_x > 13 && pixel_x < 278 && pixel_y > 286 && pixel_y < 322)//About is pressed.
  {
    if (settings == false && about == true)
    {
      tft.fillScreen(BLACK);
      bmpDraw("about.bmp", 0, 0);
      settings = true;
      about = false;
    }
  }

  else if (pixel_x > 3 && pixel_x < 127 && pixel_y > 17 && pixel_y < 45)//back to settings.
  {
    if (settings == true && about == false)
    {
      tft.fillScreen(BLACK);
      bmpDraw("settings.bmp", 0, 0);
      settings = false;
      about = true;
    }
  }

  else if (pixel_x > 9 && pixel_x < 304 && pixel_y > 340 && pixel_y < 374) //restart button is pressed.
  {
    if (settings == false && restart == true)
    {
      digitalWrite(48, HIGH);
    }
  }

  else if (pixel_x > 4 && pixel_x < 315 && pixel_y > 387 && pixel_y < 429) //shutdown button is pressed.
  {
    if (settings == false && restart == true)
    {
      digitalWrite(49, HIGH);
    }
  }

  //Music Sector
  if (pixel_x > 244 && pixel_x < 299 && pixel_y > 421 && pixel_y < 465)//Music icon is pressed.
  {
    if (home == false && music == true)
    {
      tft.fillScreen(BLACK);
      bmpDraw("music.bmp", 0, 0);
      music = false;
      home = true;
      if (onetime == 0)
      {
        execute_CMD(0x11, 0, 1);// repeat all
        onetime = 1;
      }
    }
  }

  else if (pixel_x > 3 && pixel_x < 60 && pixel_y > 33 && pixel_y < 62)//back to home.
  {
    if (music == false && home == true)
    {
      tft.fillScreen(BLACK);
      bmpDraw("home.bmp", 0, 0);
      music = true;
      home = false;
    }
  }

  else if (music == false && home == true)
  {
    if (pixel_x > 128 && pixel_x < 191 && pixel_y > 390 && pixel_y < 463) //pause button is pressed.
    {
      if (pause == true)
      {
        tft.fillRect(25, 358, 100, 17, PAUSED);
        tft.setCursor(25, 360);
        tft.setTextColor(WHITE);
        tft.setTextSize(2);
        tft.print("PAUSED!");
        bmpDraw("pause.bmp", 128, 400);
        execute_CMD(0x0E, 0, 0);
        pause = false;
      }

      else if (pause == false)
      {
        tft.fillRect(25, 357, 71, 19, PAUSED);
        tft.setCursor(25, 360);
        tft.setTextColor(WHITE);
        tft.setTextSize(2);
        tft.print("PLAYING!");
        bmpDraw("resume.bmp", 128, 400);
        execute_CMD(0x0D, 0, 1);
        pause = true;
      }
    }

    else if (pixel_x > 225 && pixel_x < 240 && pixel_y > 413 && pixel_y < 442) //next button is pressed.
    {
      tft.fillRect(25, 357, 71, 19, PAUSED);
      tft.setCursor(25, 360);
      tft.setTextColor(WHITE);
      tft.setTextSize(2);
      tft.print("PLAYING!");
      bmpDraw("resume.bmp", 128, 400);
      execute_CMD(0x01, 0, 1);
      delay(300);
    }

    else if (pixel_x > 102 && pixel_x < 123 && pixel_y > 420 && pixel_y < 440) //previous button is pressed.
    {
      tft.fillRect(25, 357, 71, 19, PAUSED);
      tft.setCursor(25, 360);
      tft.setTextColor(WHITE);
      tft.setTextSize(2);
      tft.print("PLAYING!");
      bmpDraw("resume.bmp", 128, 400);
      execute_CMD(0x02, 0, 1);
      delay(300);
    }

    else if (pixel_x > 20 && pixel_x < 60 && pixel_y > 410 && pixel_y < 450) //shuffle button is pressed.
    {
      if (shuffle == true)
      {
        tft.fillRect(25, 357, 71, 19, PAUSED);
        tft.setCursor(25, 360);
        tft.setTextColor(WHITE);
        tft.setTextSize(2);
        tft.print("PLAYING!");
        bmpDraw("resume.bmp", 128, 400);
        delay(300);
        bmpDraw("shuffon.bmp", 20, 410);
        execute_CMD(0x08, 0, 3);
        shuffle = false;
      }
      else if (shuffle == false)
      {
        bmpDraw("shuffoff.bmp", 20, 410);
        shuffle = true;
      }
    }

    else if (pixel_x > 237 && pixel_x < 310 && pixel_y > 397 && pixel_y < 463) //repeat button is pressed.
    {
      if (repeat == true)//ON
      {
        tft.fillRect(25, 357, 71, 19, PAUSED);
        tft.setCursor(25, 360);
        tft.setTextColor(WHITE);
        tft.setTextSize(2);
        tft.print("PLAYING!");
        bmpDraw("resume.bmp", 128, 400);
        delay(300);
        bmpDraw("repon.bmp", 269, 417);
        execute_CMD(0x08, 0, 1);
        repeat = false;
      }
      else if (repeat == false)//OFF
      {
        bmpDraw("repoff.bmp", 269, 415);
        repeat = true;
      }
    }

    else if (pixel_x > 273 && pixel_x < 313 && pixel_y > 349 && pixel_y < 389) //like button is pressed.
    {
      if (like == true)
      {
        bmpDraw("like.bmp", 273, 349);
        like = false;
      }
      else if (like == false)
      {
        bmpDraw("dislike.bmp", 273, 349);
        like = true;
      }
    }
  }
  if (home == true && music == false)//pic moving function.
  {
    s = s + 1;
    pic++;
    delay(300);
    if (pic > 6)
    {
      pic = 1;
      s = 1;
    }
    switch (pic)
    {
      case 1:
        if (musicx == true && musicy == false)
        {
          bmpDraw("m1.bmp", 18, 74);
          musicx = false;
          musicy = true;
        }
        break;

      case 2:
        if (musicx == false && musicy == true)
        {
          bmpDraw("m2.bmp", 18, 74);
          musicx = true;
          musicy = false;
        }
        break;

      case 3:
        if (musicx == true && musicy == false)
        {
          bmpDraw("m3.bmp", 18, 74);
          musicx = false;
          musicy = true;
        }
        break;

      case 4:
        if (musicx == false && musicy == true)
        {
          bmpDraw("m4.bmp", 18, 74);
          musicx = true;
          musicy = false;
        }
        break;

      case 5:
        if (musicx == true && musicy == false)
        {
          bmpDraw("m5.bmp", 18, 74);
          musicx = false;
          musicy = true;
        }
        break;

      case 6:
        if (musicx == false && musicy == true)
        {
          bmpDraw("m6.bmp", 18, 74);
          musicx = true;
          musicy = false;
        }
        break;
    }
  }
}

void execute_CMD(byte CMD, byte Par1, byte Par2)// Excecute the command and parameters
{
  word checksum = -(Version_Byte + Command_Length + CMD + Acknowledge + Par1 + Par2);// Calculate the checksum (2 bytes)
  byte Command_line[10] = { Start_Byte, Version_Byte, Command_Length, CMD, Acknowledge, Par1, Par2, highByte(checksum), lowByte(checksum), End_Byte}; // Build the command line
  for (byte k = 0; k < 10; k++)//Send the command line to the module
  {
    Serial1.write( Command_line[k]);
  }
}

#define BUFFPIXEL 20

void bmpDraw(char *filename, int x, int y) {

  File     bmpFile;
  int      bmpWidth, bmpHeight;   // W+H in pixels
  uint8_t  bmpDepth;              // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;        // Start of image data in file
  uint32_t rowSize;               // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3 * BUFFPIXEL]; // pixel in buffer (R+G+B per pixel)
  uint16_t lcdbuffer[BUFFPIXEL];  // pixel out buffer (16-bit per pixel)
  uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  //boolean  goodBmp = false;       // Set to true on valid header parse
  boolean  flip    = true;        // BMP is stored bottom-to-top
  int      w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0;
  uint8_t  lcdidx = 0;
  boolean  first = true;

  if ((x >= tft.width()) || (y >= tft.height())) return;
  if ((bmpFile = SD.open(filename)) == NULL) {
    return;
  }

  // Parse BMP header
  if (read16(bmpFile) == 0x4D42) { // BMP signature
    (void)read32(bmpFile);
    (void)read32(bmpFile); // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile); // Start of image data
    // Read DIB header
    (void)read32(bmpFile);
    bmpWidth  = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    if (read16(bmpFile) == 1) { // # planes -- must be '1'
      bmpDepth = read16(bmpFile); // bits per pixel
      if ((bmpDepth == 24) && (read32(bmpFile) == 0)) { // 0 = uncompressed

        // goodBmp = true; // Supported BMP format -- proceed!
        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * 3 + 3) & ~3;

        // Crop area to be loaded
        w = bmpWidth;
        h = bmpHeight;
        if ((x + w - 1) >= tft.width())  w = tft.width()  - x;
        if ((y + h - 1) >= tft.height()) h = tft.height() - y;

        // Set TFT address window to clipped image bounds
        tft.setAddrWindow(x, y, x + w - 1, y + h - 1);

        for (row = 0; row < h; row++) {
          if (flip) // Bitmap is stored bottom-to-top order (normal BMP)
            pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
          else     // Bitmap is stored top-to-bottom
            pos = bmpImageoffset + row * rowSize;
          if (bmpFile.position() != pos) { // Need seek?
            bmpFile.seek(pos);
            buffidx = sizeof(sdbuffer); // Force buffer reload
          }

          for (col = 0; col < w; col++) { // For each column...
            // Time to read more pixel data?
            if (buffidx >= sizeof(sdbuffer)) { // Indeed
              // Push LCD buffer to the display first
              if (lcdidx > 0) {
                tft.pushColors(lcdbuffer, lcdidx, first);
                lcdidx = 0;
                first  = false;
              }
              bmpFile.read(sdbuffer, sizeof(sdbuffer));
              buffidx = 0; // Set index to beginning
            }

            // Convert pixel from BMP to TFT format
            b = sdbuffer[buffidx++];
            g = sdbuffer[buffidx++];
            r = sdbuffer[buffidx++];
            lcdbuffer[lcdidx++] = tft.color565(r, g, b);
          } // end pixel
        } // end scanline
        // Write any remaining data to LCD
        if (lcdidx > 0) {
          tft.pushColors(lcdbuffer, lcdidx, first);
          first = false;
          col += lcdidx;
        }
      }
      // end goodBmp
    }
  }
  bmpFile.close();
}

uint16_t read16(File f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t read32(File f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}
