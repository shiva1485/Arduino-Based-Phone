#include <Adafruit_GFX.h>
#include <TouchScreen.h>
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include <SD.h>
#include <SPI.h>
#include <TouchScreen.h>

#define MINPRESSURE 200
#define MAXPRESSURE 1000

const int XP=8,XM=A2,YP=A3,YM=9; //320x480 ID=0x9486
const int TS_LEFT=175,TS_RT=913,TS_TOP=941,TS_BOT=91;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

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

float Num1, Num2, Number;
char op;
boolean answer = false;

void setup(void)
{
  Serial.begin(9600);
  tft.begin(0x9486);;//change the driver name in accordence to your display driver.
  if (!SD.begin(53))
  {
    return;
  }
  tft.fillScreen(BLACK);
  tft.setRotation(0);

  bmpDraw("calcu.bmp", 0, 0);
}

void loop()
{
  touch();
  if (touch())
  {
    Serial.print("Number:");
    Serial.println(Number);
    Serial.print("Num1:");
    Serial.println(Num1);
    Serial.print("Num2:");
    Serial.println(Num2);
  }
  if (pixel_x > 13 && pixel_x < 79 && pixel_y > 156 && pixel_y < 211)
  {
    Serial.println ("Cancel Button");
    Number = Num1 = Num2 = 0;
    answer = false;
  }
  else if (pixel_x > 13 && pixel_x < 79 && pixel_y > 351 && pixel_y < 406) //If Button 1 is pressed
  {
    Serial.println ("Button 1");
    if (Number == 0)
      Number = 1;
    else
      Number = (Number * 10) + 1; //Pressed twice
  }

  else if (pixel_x > 13 && pixel_x < 79 && pixel_y > 285 && pixel_y < 340) //If Button 4 is pressed
  {
    Serial.println ("Button 4");
    if (Number == 0)
      Number = 4;
    else
    {
      Number = (Number * 10) + 4; Serial.println ("Button 1");
    }//Pressed twice
  }

  else if (pixel_x > 13 && pixel_x < 79 && pixel_y > 219 && pixel_y < 274) //If Button 7 is pressed
  {
    Serial.println ("Button 7");
    if (Number == 0)
      Number = 7;
    else
      Number = (Number * 10) + 7; //Pressed twice
  }

  else if (pixel_x > 13 && pixel_x < 154 && pixel_y > 415 && pixel_y < 472)
  {
    Serial.println ("Button 0"); //Button 0 is Pressed
    if (Number == 0)
      Number = 0;
    else
      Number = (Number * 10) + 0; //Pressed twice
  }

  else if (pixel_x > 90 && pixel_x < 156 && pixel_y > 351 && pixel_y < 406)
  {
    Serial.println ("Button 2");
    if (Number == 0)
      Number = 2;
    else
      Number = (Number * 10) + 2; //Pressed twice
  }

  else if (pixel_x > 90 && pixel_x < 156 && pixel_y > 285 && pixel_y < 340)
  {
    Serial.println ("Button 5");
    if (Number == 0)
      Number = 5;
    else
      Number = (Number * 10) + 5; //Pressed twic
  }

  else if (pixel_x > 90 && pixel_x < 156 && pixel_y > 219 && pixel_y < 274)
  {
    Serial.println ("Button 8");
    if (Number == 0)
      Number = 8;
    else
      Number = (Number * 10) + 8; //Pressed twic
  }

  else if (pixel_x > 167 && pixel_x < 222 && pixel_y > 351 && pixel_y < 406)
  {
    Serial.println ("Button 3");
    if (Number == 0)
      Number = 3;
    else
      Number = (Number * 10) + 3; //Pressed twice
  }

  else if (pixel_x > 167 && pixel_x < 222 && pixel_y > 285 && pixel_y < 340)
  {
    Serial.println ("Button 6");
    if (Number == 0)
      Number = 6;
    else
      Number = (Number * 10) + 6; //Pressed twice
  }

  if (pixel_x > 167 && pixel_x < 222 && pixel_y > 219 && pixel_y < 274)
  {
    Serial.println ("Button 9");
    if (Number == 0)
      Number = 9;
    else
      Number = (Number * 10) + 9; //Pressed twice
  }

  else if (pixel_x > 243 && pixel_x < 309 && pixel_y > 415 && pixel_y < 470) ///Equal to button
  {
    Serial.println ("Button Equal");
    Num2 = Number;
    answer = true;
  }
  else if (pixel_x > 243 && pixel_x < 309) //Detecting Buttons on Column 4
  {

    Num1 = Number;
    Number = 0;

    tft.setCursor(260, 70);
    tft.setTextColor(BLACK);

    if (pixel_y > 351 && pixel_y < 406)
    {
      Serial.println ("Addition Button");
      op = 1;
      tft.print("+");
    }

    if (pixel_y > 285 && pixel_y < 340)
    {
      Serial.println ("subtraction Button");
      op = 2;
      tft.print("-");
    }

    if (pixel_y > 219 && pixel_y < 274)
    {
      Serial.println ("multiplication Button");
      op = 3;
      tft.print("*");
    }

    if (pixel_y > 156 && pixel_y < 211)
    {
      Serial.println ("Division Button");
      op = 4;
      tft.print("/");
    }
  }
  
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
