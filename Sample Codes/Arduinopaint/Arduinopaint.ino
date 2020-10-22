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

int PENRADIUS = 4;
int currentcolor;
int xpos;
int ypos;

#define BLACK   0x0000
#define BLUE    0x051D
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

bool notes = false;
bool more = false;

void setup(void)
{
  tft.begin(0x9486);
  if (!SD.begin(53))
  {
    return;
  }
  Serial.begin(9600);
  tft.setRotation(0);
  tft.fillScreen(BLACK);
  bmpDraw("notestop.bmp", 0, 0);
  currentcolor = RED;
  more = true;
}

void loop()
{
  touch();

  if (pixel_x > 192 && pixel_x < 251 && pixel_y > 4 && pixel_y < 30)
  {
    if (more == true && notes == false)
    {
      tft.fillScreen(BLACK);
      bmpDraw("more.bmp", 0, 0);
      more = false;
      notes = true;
    }
  }

  else if (pixel_x > 4 && pixel_x < 70 && pixel_y > 6 && pixel_y < 34)
  {
    if (more == false && notes == true)
    {
      tft.fillScreen(BLACK);
      bmpDraw("notestop.bmp", 0, 0);
      more = true;
      notes = false;
    }
  }

  else if (notes == false && more == true)
  {
    xpos = pixel_x;
    ypos = pixel_y;
    if (((ypos - PENRADIUS) > 35) && ((ypos + PENRADIUS) < 480))//drawing function
    {
      tft.fillCircle(xpos, ypos, PENRADIUS, currentcolor);
    }

    else if (pixel_x > 262 && pixel_x < 315 && pixel_y > 7 && pixel_y < 32)//clear button
    {
      tft.fillRect(0,35,320,445,BLACK);
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
      else if (pixel_x > 52 && pixel_x < 94)
      {
        currentcolor = WHITE;
      }
      else if (pixel_x > 94 && pixel_x < 136)
      {
        currentcolor = YELLOW;
      }
      else if (pixel_x > 136 && pixel_x < 178)
      {
        currentcolor = GREEN;
      }
      else if (pixel_x > 178 && pixel_x < 220)
      {
        currentcolor = BLUE;
      }
      else if (pixel_x > 220 && pixel_x < 262)
      {
        currentcolor = CYAN;
      }
      else if (pixel_x > 262 && pixel_x < 304)
      {
        currentcolor = MAGENTA;
      }
    }


    else if (pixel_y > 257 && pixel_y < 301)
    {
      if (pixel_x > 11 && pixel_x < 65)//selecting bush thickness
      {
        PENRADIUS = 1;
      }
      else if (pixel_x > 93 && pixel_x < 147)
      {
        PENRADIUS = 2;
      }
      else if (pixel_x > 174 && pixel_x < 228)
      {
        PENRADIUS = 3;
      }
      else if (pixel_x > 253 && pixel_x < 307)
      {
        PENRADIUS = 4;
      }
    }
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
