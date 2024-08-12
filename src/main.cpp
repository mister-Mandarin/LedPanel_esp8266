#include <PxMatrix.h>
#include <Ticker.h>

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
 uint8_t display_draw_time=30; //30-70 is usually fine

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

// uint8_t static weather_icons[]={
//    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x00,0xff,0xe0,0x00,0x00,0x00,0x00,0xff,0xe0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xff,0x07,0xff,0x07,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x07,0xff,0x07,0xff,0x07,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0xff,0xe0,0x00,0x00,0x00,0x00,0x00,0x00
// };

uint16_t myCOLORS[8]={myRED,myGREEN,myBLUE,myWHITE,myYELLOW,myCYAN,myMAGENTA,myBLACK};
unsigned long lastTime1 = 0;
unsigned long lastTime2 = 0;
unsigned long lastTime3 = 0;
unsigned long lastTime4 = 0;

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
  // Линейная интерполяция между 16 и 255 т.к. меньше 16 матрица не видит!
  int minBrightness = 16;
  int maxBrightness = 255;

  int brightness = minBrightness + ((maxBrightness - minBrightness) * position) / totalLength;
  return display.color565(0, brightness, 0);
}


void rotateLine(int degr) {
switch (degr) {
case 0:
  display.setRotate(false);
  display.setFlip(false);
  break;
case 90:
  display.setFlip(false);
  display.setRotate(true);
  break;
case 180:
  display.setRotate(false);
  display.setFlip(true);
  break;
case 270:
  display.setRotate(true);
  display.setFlip(true);
  break;

default:
  break;
}
}

void generateLineSegmentXY(short lineLength, short lineHeight) {
  display.clearDisplay();
  short height = lineHeight - 1; // координаты начинаются с 0


  for (short xx = 0; xx < lineLength; xx++) {
    uint16_t color = getLineColor(xx, lineLength);
    short x0 = xx + offset3;
 
    display.drawLine(x0, 0, x0, height, color); // первая линия с 0 0 
    
    if (offset3 > matrix_width -1 - lineLength) {
      short x1 = offset3 - matrix_width + xx + lineHeight;
      display.drawLine(x1, 0, x1, height, color);
    }
  }

  offset3 = (offset3 + 1) % (matrix_width - lineHeight);
  display.display();
}

// void generateLineSegmentXY(short lineLength, short lineHeight, int offset) {
//   display.clearDisplay();
//   short height = lineHeight - 1;

//   // Первая линия, начинается с координат x = 0, y = 0
//   for (short xx = 0; xx < lineLength; xx++) {
//     uint16_t color = getLineColor(xx, lineLength);
//     short x0 = (xx + offset) % (matrix_width);

//     display.drawLine(x0, 0, x0, height, color);
//   }

//   display.display();
// }



void setup() {
	Serial.begin(115200);
	display.begin(32);

  display_update_enable(true);
}

void loop() {
  unsigned long currentTime = millis();
  unsigned long time = 40;
  
  if (currentTime - lastTime3 >= time) {
    lastTime3 = currentTime;
    generateLineSegmentXY(30, 3);
  }
  // if (currentTime - lastTime1 >= time) {
  //   lastTime1 = currentTime;
  //   generateLineSegmentYX(30, 3, 0, offset1);
  //   offset1 = (offset1 + 1) % matrix_width;
  // }

  // if (currentTime - lastTime2 >= time) {
  //   lastTime2 = currentTime;
  //   generateLineSegmentYX(30, 3, 90, offset2);
  //   offset2 = (offset2 + 1) % matrix_width;
  // }

  // if (currentTime - lastTime4 >= time) {
  //   lastTime4 = currentTime;
  //   generateLineSegmentXY(30, 3, 270, offset4);
  //   offset4 = (offset4 + 1) % matrix_width;
  // }

}