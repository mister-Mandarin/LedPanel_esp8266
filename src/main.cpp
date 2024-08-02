#include <ESP8266WiFi.h>      // Библиотека для работы с Wi-Fi на ESP8266
#include <ESP8266WebServer.h> // Библиотека для создания веб-сервера на ESP8266
#include <DNSServer.h>        // Библиотека для создания DNS сервера, необходимого для Captive Portal

/* Установка своих SSID и пароль */
const char* ssid = "LedPanel";
const char* password = "12345678";

/* Настройки IP адреса */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

// Создание объекта веб-сервера и DNS-сервера
ESP8266WebServer server(80);
DNSServer dnsServer;

uint8_t LEDpin = 2;
bool LEDstatus = LOW;

// Функция генерации HTML страницы
String generateHTML(bool ledStatus) {
	String ledState = ledStatus ? "ON" : "OFF";

	String html = "<!DOCTYPE html> <html>";
	html +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">";
	html +="<title>LED Control</title>";
	html +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}";
	html +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}";
	html +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}";
	html +=".button-on {background-color: #1abc9c;}";
	html +=".button-on:active {background-color: #16a085;}";
	html +=".button-off {background-color: #34495e;}";
	html +=".button-off:active {background-color: #2c3e50;}";
	html +="p {font-size: 14px;color: #888;margin-bottom: 10px;}";
	html +="</style>";
	html +="</head>";
	html +="<body>";
	html +="<h1>ESP8266 Web Server</h1>";
	html +="<h3>Using Access Point(AP) Mode</h3>";
	html +="<p>LED Status: " + ledState + "</p>";

	if(ledStatus)
		html +="<a class='button button-on' href='/ledOn'>ON</a>";
	else
		html +="<a class='button button-off' href='/ledOff'>OFF</a>";

	html +="</body>";
	html +="</html>";
	return html;
}

// Обработчик для главной страницы
void onConnect() {
	LEDstatus = !LEDstatus;
	//digitalWrite(LEDpin, LEDstatus);
	String html = generateHTML(LEDstatus);
	server.send(200, "text/html", html); // Отправка HTML страницы в браузер клиента
}

// Обработчики для включения и выключения светодиода
void ledOn() {
	//LEDstatus = HIGH;
	onConnect();
}

void ledOff() {
	//LEDstatus = LOW;
	onConnect();
}

void setup() {
	// Инициализация последовательного соединения с компьютером на скорости 115200 бод
	Serial.begin(115200);
	// Установка пина D7 в режим вывода (OUTPUT), чтобы можно было управлять светодиодом
	pinMode(LEDpin, OUTPUT);
	// Создание точки доступа Wi-Fi с заданными SSID и паролем
	WiFi.softAP(ssid, password);
	// Конфигурация IP, шлюза и маски подсети для точки доступа
	WiFi.softAPConfig(local_ip, gateway, subnet);
	// Небольшая задержка в 100 мс, чтобы дать время на настройку Wi-Fi
	delay(500);

	// Запуск DNS-сервера и перенаправление всех доменов на IP ESP8266
	dnsServer.start(53, "*", local_ip);

	server.on("/", onConnect);  // Определение обработчика для главной страницы "/", который срабатывает при подключении к серверу
	server.on("/ledOn", ledOn);  // Определение обработчика для запроса "/led1on", включающего первый светодиод
	server.on("/ledOff", ledOff); // Определение обработчика для запроса "/led1off", выключающего первый светодиод
	server.onNotFound(onConnect);  // Перенаправление всех несуществующих страниц на главную страницу
	
	server.begin();                    // Запуск веб-сервера
}

void loop() {
	dnsServer.processNextRequest();    // Обработка запросов DNS для Captive Portal
	server.handleClient();             // Обработка входящих HTTP-запросов от клиентов
	digitalWrite(LEDpin, LEDstatus); 
	// if(LEDstatus)                     // Если LED1status равен true (светодиод должен быть включен)
	//   digitalWrite(LEDpin, LOW);        // Включение светодиода (подача высокого уровня на пин)
	// else                               // Иначе (LED1status равен false)
	//   digitalWrite(LEDpin, HIGH);     // Выключение светодиода (подача низкого уровня на пин)
}