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
bool play = false;
bool home = false;
bool calcu = false;
bool radio = false;
bool video = false;
bool notes = false;
bool settings = false;
bool clock = false;
bool phone = false;
bool photos = false;
bool maps = false;
bool music = false;
bool photoy = false;
bool photosz = false;
bool mapy = false;
bool mapz = false;
int photox = 0;
int mapx = 0;

void setup(void)
{
  Serial.begin(9600);
  tft.begin(0x9486);;//change the driver name in accordence to your display driver.
  if (!SD.begin(53))
  {
    return;
  }
  tft.setRotation(0);            //PORTRAIT
  tft.fillScreen(BLACK);
  bmpDraw("logo.bmp", 0, 0);
  delay(1000);
  tft.fillScreen(BLACK);
  bmpDraw("home.bmp", 0, 0);

  calcu = true;
  radio = true;
  video = true;
  notes = true;
  settings = true;
  clock = true;
  phone = true;
  photos = true;
  maps = true;
  music = true;
  photoy = true;
  mapy = true;
}

/* two buttons are quite simple
*/
void loop(void)
{
  Touch_getXY();

  //calculator sector
  if (pixel_x > 12 && pixel_x < 73 && pixel_y > 29 && pixel_y < 90)
  {
    if (home == false  && calcu == true)
    {
      tft.fillScreen(BLACK);
      bmpDraw("calcu.bmp", 0, 0);
      home = true;
      calcu = false;
    }
  }
  else if (pixel_x > 9 && pixel_x < 76 && pixel_y > 9 && pixel_y < 33)
  {
    if (home == true && calcu == false)
    {
      tft.fillScreen(BLACK);
      home = false;
      calcu = true;
      bmpDraw("home.bmp", 0, 0);
    }
  }

  //video sector
  if (pixel_x > 16 && pixel_x < 77 && pixel_y > 137 && pixel_y < 198)
  {
    if (home == false  && video == true)
    {
      tft.fillScreen(BLACK);
      bmpDraw("topvideo.bmp", 0, 0);
      home = true;
      video = false;
    }
  }

  else if (pixel_x > 9 && pixel_x < 60 && pixel_y > 8 && pixel_y < 28)
  {
    if (home == true && video == false)
    {
      tft.fillScreen(BLACK);
      home = false;
      video = true;
      bmpDraw("home.bmp", 0, 0);
    }
  }
  if (pixel_x > 250 && pixel_x < 297 && pixel_y > 4 && pixel_y < 28)
  {
    play = true;
    while (pixel_x > 250 && pixel_x < 297 && pixel_y > 4 && pixel_y < 28)
    {
      Touch_getXY();
    }
  }

  if (home == true && video == false)
  {
    if (play == true)
    {
      bmpDraw("x.bmp", 70, 150);
      bmpDraw("z.bmp", 70, 150);
      bmpDraw("y.bmp", 70, 150);
      play = false;
    }
  }

  //phone sector
  if (pixel_x > 21 && pixel_x < 76 && pixel_y > 421 && pixel_y < 465)
  {
    if (home == false  && video == true)
    {
      tft.fillScreen(BLACK);
      home = true;
      phone = false;
      bmpDraw("keypad1.bmp", 0, 0);
    }
  }

  else if (pixel_x > 236 && pixel_x < 320 && pixel_y > 457 && pixel_y < 477)
  {
    if (home == true && phone == false)
    {
      tft.fillScreen(BLACK);
      home = false;
      phone = true;
      bmpDraw("home.bmp", 0, 0);
    }
  }

  //photos sector
  if (pixel_x > 103 && pixel_x < 158 && pixel_y > 422 && pixel_y < 466)
  {
    if (home == false  && photos == true)
    {
      tft.fillScreen(BLACK);
      home = true;
      photos = false;
      bmpDraw("phototop.bmp", 0, 0);
    }
  }

  if (pixel_x > 5 && pixel_x < 69 && pixel_y > 8 && pixel_y < 30) //back
  {
    if (home == true && photos == false)
    {
      tft.fillScreen(BLACK);
      home = false;
      photos = true;
      bmpDraw("home.bmp", 0, 0);
    }
  }

  if (home == true && photos == false)
  {
    tft.fillRect(83, 16, 10, 18, TEST);
    tft.setCursor(83, 16);
    tft.setTextSize(2);
    tft.setTextColor(GREEN);
    tft.print(photox);
  }

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
  if (home == true && photos == false)
  {
    if (pixel_x > 262 && pixel_x < 317 && pixel_y > 7 && pixel_y < 31)
    {
      photox++;
      while (pixel_x > 264 && pixel_x < 318 && pixel_y > 11 && pixel_y < 31)
      {
        Touch_getXY();
      }
    }
    if (pixel_x > 204 && pixel_x < 250 && pixel_y > 10 && pixel_y < 31)
    {
      photox--;
      while (pixel_x > 204 && pixel_x < 250 && pixel_y > 10 && pixel_y < 31)
      {
        Touch_getXY();
      }
    }

    if (photox == 0)
    {
      photox = 1;
    }
    else if (photox == 5)
    {
      photox = 4;
    }
  }

  //Maps sector
  if (pixel_x > 179 && pixel_x < 234 && pixel_y > 418 && pixel_y < 462)
  {
    if (home == false  && maps == true)
    {
      tft.fillScreen(BLACK);
      home = true;
      maps = false;
      bmpDraw("mapstop.bmp", 0, 0);
    }
  }

  else if (pixel_x > 3 && pixel_x < 71 && pixel_y > 7 && pixel_y < 30) //back
  {
    if (home == true && maps == false)
    {
      tft.fillScreen(BLACK);
      home = false;
      maps = true;
      bmpDraw("home.bmp", 0, 0);
    }
  }

  else if (home == true && maps == false)
  {
    tft.fillRect(83, 16, 28, 18, TEST);
    tft.setCursor(83, 16);
    tft.setTextSize(2);
    tft.setTextColor(GREEN);
    tft.print(mapx);
  }

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
  if (home == true && maps == false)
  {
    if (pixel_x > 259 && pixel_x < 319 && pixel_y > 7 && pixel_y < 29)
    {
      mapx++;
      while (pixel_x > 259 && pixel_x < 319 && pixel_y > 7 && pixel_y < 29)
      {
        Touch_getXY();
      }
    }
    if (pixel_x > 201 && pixel_x < 254 && pixel_y > 8 && pixel_y < 29)
    {
      mapx--;
      while (pixel_x > 201 && pixel_x < 254 && pixel_y > 8 && pixel_y < 29)
      {
        Touch_getXY();
      }
    }

    if (mapx == 0)
    {
      mapx = 1;
    }
    else if (mapx == 19)
    {
      mapx = 18;
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
