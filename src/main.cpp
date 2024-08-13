#include <PxMatrix.h>
#include <Ticker.h>
#include <imageHome.h>

Ticker display_ticker;
#define P_LAT 16
#define P_A 5
#define P_B 4
#define P_C 15
#define P_D 12
#define P_E 0
#define P_OE 2

#define matrix_width 64
#define matrix_height 64

// This defines the 'on' time of the display is us. The larger this number,
// the brighter the display. If too large the ESP will crash
 uint8_t display_draw_time=5; //30-70 is usually fine

PxMATRIX display(64,64,P_LAT, P_OE,P_A,P_B,P_C,P_D,P_E);

// Some standard colors
uint16_t myRED = display.color565(255, 0, 0);
uint16_t myGREEN = display.color565(0, 255, 0);
uint16_t myBLUE = display.color565(0, 0, 255);
uint16_t myWHITE = display.color565(255, 255, 255);
uint16_t myYELLOW = display.color565(255, 255, 0);
uint16_t myCYAN = display.color565(0, 255, 255);
uint16_t myMAGENTA = display.color565(255, 0, 255);
uint16_t myBLACK = display.color565(0, 0, 0);

uint16_t myCOLORS[8]={myRED,myGREEN,myBLUE,myWHITE,myYELLOW,myCYAN,myMAGENTA,myBLACK};

unsigned long lastTime = 0;

int offset1 = 0;
int offset2 = 0;
int offset3 = 0;
int offset4 = 0;
// ISR for display refresh
void display_updater() {
  display.display(display_draw_time);
}

void display_update_enable(bool is_enable) {
  if (is_enable)
    display_ticker.attach(0.004, display_updater);
  else
    display_ticker.detach();
}

uint16_t getLineColor(int position, int totalLength) {
  // Линейная интерполяция между 16 и 255. меньше 16 матрица не видит
  int minBrightness = 15; // для того, чтобы первая линия была всегда черная
  int maxBrightness = 255;

  int brightness = minBrightness + ((maxBrightness - minBrightness) * position) / totalLength;
  return display.color565(0, brightness, 0);
}

void generateLineSegmentXY(short lineLength, short lineHeight) {
  short height = lineHeight - 1;


  for (short xx = 0; xx < lineLength; xx++) {
    uint16_t color = getLineColor(xx, lineLength);
    short x0 = xx + offset1;
 
    display.drawLine(x0, 0, x0, height, color);
   
    if (offset1 > matrix_width - 1 - lineLength) {
      short x1 = x0 - matrix_width + lineHeight;
      display.drawLine(x1, 0, x1, height, color);
    }
  }

  offset1 = (offset1 + 1) % (matrix_width - lineHeight);
}

void generateLineSegmentYX(short lineLength, short lineHeight) {
  short height = lineHeight - 1;

  for (short yy = 0; yy < lineLength; yy++) {
    uint16_t color = getLineColor(yy, lineLength);
    short y0 = yy + offset2;
 
    display.drawLine(matrix_width - 1, y0, matrix_width - 1 - height, y0, color);
   
    if (offset2 > matrix_width - 1 - lineLength) {
      short y1 = y0 - matrix_width + lineHeight;
      display.drawLine(matrix_width - 1, y1, matrix_width - 1 - height, y1, color);
    }
  }

  offset2 = (offset2 + 1) % (matrix_width - lineHeight);
}

void generateLineSegmentxY(short lineLength, short lineHeight) {
  short height = lineHeight - 1;


  for (short xx = 0; xx < lineLength; xx++) {
    uint16_t color = getLineColor(xx, lineLength);
    short x0 = matrix_width - 1 - xx - offset3;
 
    display.drawLine(x0, matrix_height - 1, x0, matrix_height - 1 - height, color);
   
    if (offset3 > matrix_width - 1 - lineLength) {
      short x1 = matrix_width + x0 - lineHeight;
      display.drawLine(x1, matrix_height - 1, x1, matrix_height - 1 - height, color);
    }
  }

  offset3 = (offset3 + 1) % (matrix_width - lineHeight);
}

void generateLineSegmentXy(short lineLength, short lineHeight) {
  short height = lineHeight - 1;

  for (short yy = 0; yy < lineLength; yy++) {
    uint16_t color = getLineColor(yy, lineLength);
    short y0 = matrix_height - 1 - yy - offset4;
 
    display.drawLine(0, y0, height, y0, color);
   
    if (offset4 > matrix_width - 1 - lineLength) {
      short y1 = matrix_width + y0 - lineHeight;
      display.drawLine(0, y1, height, y1, color);
    }
  }
  
  offset4 = (offset4 + 1) % (matrix_width - lineHeight);
}

template <size_t height, size_t width>
void displayImage(const uint16_t (&image)[height][width]) {
    size_t x_center = (matrix_width - width) / 2;
    size_t y_center = (matrix_height - height) / 2;

    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            display.drawPixel(x_center + j, y_center + i, image[i][j]);
        }
    }
}

void setup() {
	Serial.begin(115200);
	display.begin(32);

  displayImage(imageHome);

  display_update_enable(true);
}


void loop() {
  unsigned long currentTime = millis();
  unsigned long time = 50;
  short lenght = 40;
  short height = 3;
  
  if (currentTime - lastTime >= time) {
    lastTime = currentTime;

    generateLineSegmentXY(lenght, height);
    generateLineSegmentYX(lenght, height);
    generateLineSegmentxY(lenght, height);
    generateLineSegmentXy(lenght, height);
  }

}