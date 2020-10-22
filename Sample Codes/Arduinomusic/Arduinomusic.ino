#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include <SD.h>
#include <SPI.h>
#include <TouchScreen.h>
#define MINPRESSURE 200
#define MAXPRESSURE 1000

const int XP = 8, XM = A2, YP = A3, YM = 9; //320x480 ID=0x9486
const int TS_LEFT = 163, TS_RT = 911, TS_TOP = 945, TS_BOT = 90;

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
#define PAUSED  0x18E5

# define Start_Byte 0x7E
# define Version_Byte 0xFF
# define Command_Length 0x06
# define End_Byte 0xEF
# define Acknowledge 0x00 //Returns info with command 0x41 [0x01: info, 0x00: no info]
# define ACTIVATED LOW

bool home1 = false;
bool music = false;
bool like = false;
bool musicx = false;
bool musicy = false;
bool pause1 = false;
bool shuffle = false;
bool repeat = false;

int s = 0;
int pic = 0;
int onetime = 0;
void setup(void)
{
  Serial1.begin(9600);
  tft.begin(0x9486);
  if (!SD.begin(53))
  {
    return;
  }
  tft.setRotation(0);
  tft.fillScreen(BLACK);
  bmpDraw("home.bmp", 0, 0);

  execute_CMD(0x3F, 0, 0);//get info
  execute_CMD(0x06, 0, 23);// Set the volume (0x00~0x30)

  music = true;
  pause1 = true;
  shuffle = true;
  repeat = true;
  like = true;
  musicx = true;
}

void loop()
{
  touch();

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
