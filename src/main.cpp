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
uint8_t display_draw_time=10; //30-70 is usually fine

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

void setup() {
	Serial.begin(115200);
	display.begin(32);

  display_update_enable(true);

  // delay(3000);
}

void loop() {
  display.clearDisplay();

  /*!
  x\y 0 1 2 3 4 5 6 ... 63
  0
  1
  2
  3
  4
  ...
  63

	display.drawLine(0, 0, 10, 0, display.color565(0, 255, 0));
	@param    x0  Start point x coordinate
	@param    y0  Start point y coordinate
	@param    x1  End point x coordinate
	@param    y1  End point y coordinate
	@param    color 16-bit 5-6-5 Color to draw with 
	
	*/

// bool flag = false;

for (int offset = 0; offset < matrix_width; offset++) {
  for (int xx = 0; xx < 16; xx++) {
    display.drawLine(xx + offset, 0, xx + offset, 2, display.color565(0, xx * 16, 0));

	if (offset > matrix_width - 16) {
		display.drawLine(xx + (offset - matrix_width), 0, xx + (offset - matrix_width), 2, display.color565(0, xx * 16, 0));
	}
}

  delay(40); // задержка для видимого перемещения
  display.clearDisplay(); // очистка экрана перед следующей итерацией
}


//   for (uint8_t dimm=255; dimm>0; dimm--) {
//     display.setBrightness(dimm);
//     delay(5);
//   }

//   for (uint8_t dimm=0; dimm<255; dimm++) {
//     display.setBrightness(dimm);
//     delay(5);
//   }

}