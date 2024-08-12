#include <PxMatrix.h>
#include <Adafruit_GFX.h>

// Размер матрицы
#define MATRIX_WIDTH 64
#define MATRIX_HEIGHT 64

// Определение пинов подключения
#define P_LAT 16
#define P_A 5
#define P_B 4
#define P_C 15
#define P_D 12
#define P_E 0
#define P_OE 2
#define P_CLK 14

// Инициализация объекта матрицы
PxMATRIX display(MATRIX_WIDTH, MATRIX_HEIGHT, P_LAT, P_OE, P_A, P_B, P_C, P_D, P_E);

void setup() {
  // Установка частоты обновления. Только 32 для моей панели
  display.begin(32);

  // Установка интенсивности яркости (от 0 до 255)
  display.setBrightness(10);

  // Установка цвета всей панели на синий
  display.fillScreen(display.color565(0, 0, 255));
}

void loop() {
  // Обновление матрицы
  display.display(50);
}