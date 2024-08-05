#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <handleControl.h>

bool ledBlinking = false; // Флаг для контроля состояния мигания
unsigned long previousMillis = 0; // Хранение времени последнего мигания
const long interval = 500; // Интервал мигания в миллисекундах

void blinkLed(uint8_t LEDpin) {
  unsigned long currentMillis = millis(); // Получаем текущее время

  if (ledBlinking) {
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis; // Сохраняем время последнего мигания
      int ledState = digitalRead(LEDpin);
      digitalWrite(LEDpin, !ledState); // Меняем состояние светодиода
    }
  } else {
    digitalWrite(LEDpin, LOW); // Если мигание выключено, выключаем светодиод
  }
}

void handleControl(uint8_t LEDpin) {
  String active = webServer.arg("active");
  int stateActive = active.toInt();
  
  switch (stateActive) {
  case 0:
    digitalWrite(LEDpin, HIGH);
    ledBlinking = false; // Останавливаем мигание
    break;
  case 1:
    digitalWrite(LEDpin, LOW);
    ledBlinking = false; // Останавливаем мигание
    break;
  case 2:
    ledBlinking = true; // Включаем мигание
    previousMillis = millis(); // Сохраняем текущее время
    break;
  default:
    break;
  }

  webServer.send(200, "text/html", "Состояние изменено");
}

void setup() {
  // Настройка пинов и веб-сервера
  pinMode(LED_BUILTIN, OUTPUT);
  webServer.on("/control", []() {
    handleControl(LED_BUILTIN);
  });
  webServer.begin();
}

void loop() {
  webServer.handleClient();
  blinkLed(LED_BUILTIN); // Проверка и выполнение мигания
}
