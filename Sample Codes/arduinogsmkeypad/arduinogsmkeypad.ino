#include <Adafruit_GFX.h>
#include <TouchScreen.h>
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include <SD.h>
#include <SPI.h>
#include <TouchScreen.h>

#define MINPRESSURE 200
#define MAXPRESSURE 1000

const int XP = 8, XM = A2, YP = A3, YM = 9; //320x480 ID=0x9486
const int TS_LEFT = 175, TS_RT = 913, TS_TOP = 941, TS_BOT = 91;

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

int no = 0;
String number = "";
int count = 0;
bool home = false;
bool phone = false;
bool call = false;
void setup(void)
{
  Serial.begin(9600);
  Serial3.begin(9600);
  tft.begin(0x9486);
  if (!SD.begin(53))
  {
    return;
  }
  tft.fillScreen(BLACK);
  tft.setRotation(0);

  bmpDraw("home.bmp", 0, 0);
  phone = true;
  call = true;
}

void loop()
{
  touch();

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
  //tft.fillRect(20, 80, 280, 70, WHITE);
  //sprintf(num, "%d", no);
  //strcat(number, num);
  //Serial.println(number.length());
  /*Serial.println(number);
    tft.setCursor(20, 100);
    tft.setTextSize(3);
    tft.setTextColor(BLACK);
    tft.print(number);*/
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
