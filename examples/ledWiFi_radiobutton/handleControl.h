#ifndef HANDLECONTROL_H
#define HANDLECONTROL_H

#include <ESP8266WebServer.h> // Добавьте этот include для доступа к типу ESP8266WebServer

extern ESP8266WebServer webServer; // Объявите webServer как внешнюю переменную

void initLed();
void handleControl();
void blinkLed();

#endif