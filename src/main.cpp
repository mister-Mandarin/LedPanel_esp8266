#include <ESP8266WiFi.h>      // Библиотека для работы с Wi-Fi на ESP8266
#include <ESP8266WebServer.h> // Библиотека для создания веб-сервера на ESP8266
#include <DNSServer.h>        // Библиотека для создания DNS сервера, необходимого для Captive Portal
#include <LittleFS.h>		  // Библиотека для работы с файловой системой
#include <handleControl.h>
// Загрузить файлы из папки data: pio run --target uploadfs


/* Установка своих SSID и пароль */
const char* ssid = "LedPanel";
const char* password = "12345678";

/* Настройки IP адреса */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

// Создание объекта веб-сервера и DNS-сервера
ESP8266WebServer webServer(80);
DNSServer dnsServer;

uint8_t LEDpin = 2;
bool LEDstatus = LOW;

void generateHTML() {
  File fileHTML = LittleFS.open("/index.html", "r");
  if (!fileHTML) {
    webServer.send(500, "text/plain", "Ошибка загрузки файла");
    return;
  }
  
  String html = fileHTML.readString();

  webServer.send(200, "text/html", html);
  fileHTML.close();
}

void generateCSS() {
  File fileCSS = LittleFS.open("/styles.css", "r");
  if (!fileCSS) {
	webServer.send(404, "text/plain", "CSS файл не найден");
	return;
	}
	
	String css = fileCSS.readString();

  webServer.send(200, "text/css", css);
  fileCSS.close();
}

void setup() {
	Serial.begin(115200);

	pinMode(LEDpin, OUTPUT);
	//digitalWrite(LEDpin, !LEDstatus);

	WiFi.softAP(ssid, password);
	WiFi.softAPConfig(local_ip, gateway, subnet);
	delay(500);

	// Инициализация файловой системы
	if (!LittleFS.begin()) {
		Serial.println("Ошибка монтирования LittleFS");
		return;
	}

	dnsServer.start(53, "*", local_ip);
	
	webServer.on("/", generateHTML);
	webServer.on("/styles.css", generateCSS);
	webServer.on("/update", []() {
		handleControl(LEDpin);
		});
	webServer.onNotFound(generateHTML);

	webServer.begin();
}

void loop() {
	dnsServer.processNextRequest();
	webServer.handleClient();
}