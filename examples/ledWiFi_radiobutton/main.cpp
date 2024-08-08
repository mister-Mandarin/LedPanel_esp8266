#include <ESP8266WebServer.h> // Библиотека для создания веб-сервера на ESP8266
#include <DNSServer.h>        // Библиотека для создания DNS сервера, необходимого для Captive Portal
#include <LittleFS.h>		  // Библиотека для работы с файловой системой
#include <handleControl.h>
#include <WifiConnection.h>
// Загрузить файлы из папки data: pio run --target uploadfs

// Создание объекта веб-сервера и DNS-сервера
ESP8266WebServer webServer(80);
DNSServer dnsServer;

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
	delay(10);

	initLed();

	WifiConnection();

	// Инициализация файловой системы
	if (!LittleFS.begin()) {
		Serial.println("Ошибка монтирования LittleFS");
		return;
	}

	dnsServer.start(53, "*", IPAddress(192,168,1,1));
	
	webServer.on("/", generateHTML);
	webServer.on("/styles.css", generateCSS);
	webServer.on("/update", handleControl);
	webServer.onNotFound(generateHTML);

	webServer.begin();
}

void loop() {
	dnsServer.processNextRequest();
	webServer.handleClient();
	blinkLed();
}