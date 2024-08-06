#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <handleControl.h>

uint8_t LEDpin = 2;
bool LEDstatus = LOW;
bool ledBlinking = false;
unsigned long previousMillis = 0;
const long interval = 500;

void initLed() {
  pinMode(LEDpin, OUTPUT);
  digitalWrite(LEDpin, !LEDstatus);
}

void blinkLed() {
  unsigned long currentMillis = millis(); // Получаем текущее время

  if (ledBlinking) {
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      int ledState = digitalRead(LEDpin);
      digitalWrite(LEDpin, !ledState);
    }
  } else {
    digitalWrite(LEDpin, !LEDstatus);
  }
}

void handleControl() {
  String active = webServer.arg("active");
  int stateActive = active.toInt();
  
  switch (stateActive) {
  case 0:
    digitalWrite(LEDpin, HIGH);
    ledBlinking = false;
    break;
  case 1:
    digitalWrite(LEDpin, LOW);
    ledBlinking = false;
    break;
  case 2:
    ledBlinking = true;
    previousMillis = millis();
    break;
  default:
    break;
  }

  webServer.send(200, "text/html", "Состояние изменено");
}