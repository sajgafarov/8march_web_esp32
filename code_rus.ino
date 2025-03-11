#include <WiFi.h>
#include <WebServer.h>
#include <EasyDDNS.h> // Подключаем библиотеку EasyDDNS

// Настройки Wi-Fi
const char* ssid = "ИМЯ СЕТИ WIFI";
const char* password = "ПАРОЛЬ WIFI";

// Настройки DDNS
const char* ddnsDomain = "ВАШ ДОМЕН"; // Замените на ваш домен ddns
const char* ddnsKey = "КЛЮЧ"; // Замените на ваш ключ ddns

WebServer server(80); // Порт

const char* ddns_name = "ваш провайдер ddns, например, 'duckdns'" // Указываем сервис DuckDNS

// Массив для хранения пожеланий
String wishes[100]; // Максимум 100 пожеланий
int wishCount = 0;  // Счётчик пожеланий

void handleRoot() {
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>Пожелания женщинам</title>";
  html += "<style>";
  html += "body { font-family: Arial, sans-serif; margin: 20px; background-color: #ffe6e6; }";
  html += "h1 { font-size: 24px; color: #ff4d4d; text-align: center; }";
  html += "form { text-align: center; margin-top: 20px; }";
  html += "textarea { width: 80%; height: 100px; font-size: 16px; padding: 10px; }";
  html += "input[type='submit'] { font-size: 16px; padding: 10px 20px; margin-top: 10px; background-color: #ff4d4d; color: white; border: none; cursor: pointer; }";
  html += ".wish { background-color: white; padding: 10px; margin: 10px 0; border-radius: 5px; box-shadow: 0 0 5px rgba(0, 0, 0, 0.1); }";
  html += ".heart { color: #ff4d4d; font-size: 24px; }";
  html += "</style>";
  html += "</head><body>";
  html += "<h1>❤️ С 8 марта! Ниже можете написать и прочитать пожелания женщинам ❤️</h1>";

  // Форма для ввода пожеланий
  html += "<form action='/addWish' method='POST'>";
  html += "<textarea name='wish' placeholder='Напишите ваше пожелание...'></textarea><br>";
  html += "<input type='submit' value='Отправить'>";
  html += "</form>";

  // Отображение всех пожеланий
  html += "<h2>Все пожелания:</h2>";
  for (int i = 0; i < wishCount; i++) {
    html += "<div class='wish'><span class='heart'>❤️</span> " + wishes[i] + "</div>";
  }

  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleAddWish() {
  if (server.hasArg("wish")) {
    String wish = server.arg("wish");
    if (wishCount < 100) {
      wishes[wishCount] = wish;
      wishCount++;
    }
  }
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(115200);

  // Подключение к Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Подключение к Wi-Fi...");
  }
  Serial.println("Подключено к Wi-Fi");
  Serial.println(WiFi.localIP());

  // Настройка EasyDDNS
  EasyDDNS.service(ddns_name); // Указываем сервис DuckDNS
  EasyDDNS.client(ddnsDomain, ddnsKey); // Указываем домен и ключ

  // Настройка маршрутов сервера
  server.on("/", handleRoot);
  server.on("/addWish", handleAddWish);

  // Запуск сервера
  server.begin();
  Serial.println("HTTP сервер запущен");
}

void loop() {
  server.handleClient();
  
  // Обновление DDNS через EasyDDNS
  EasyDDNS.update(1800000); // Обновление каждые 30 минут (в миллисекундах)
}
