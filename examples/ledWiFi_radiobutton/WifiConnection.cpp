#include <ESP8266WiFi.h>      // Библиотека для работы с Wi-Fi на ESP8266

/* Установка своих SSID и пароль */
const char* ssid = "LedPanel";
const char* password = "12345678";

/* Настройки IP адреса */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

void WifiConnection() {

    Serial.println();
    Serial.print("Configuring access point...");

    WiFi.softAP(ssid, password);
	WiFi.softAPConfig(local_ip, gateway, subnet);
    delay(100);

    Serial.println("HTTP server started");
}