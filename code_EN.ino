#include <WiFi.h>
#include <WebServer.h>
#include <EasyDDNS.h> // Connecting the EasyDDNS library

// Wi-Fi settings
const char* ssid = "WIFI NETWORK NAME";
const char* password = "WIFI PASSWORD";

// DDNS settings
const char* ddnsDomain = "YOUR DOMAIN"; // Replace with your ddns domain
const char* ddnsKey = "KEY"; // Replace with your ddns key

const char* ddns_name = "ddns provider name, eg duckdns" // Specify the DuckDNS service


WebServer server(80); // port

// Array for storing wishes
String wishes[100]; // Maximum 100 wishes
int wishCount = 0;  // Wish counter

void handleRoot() {
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>Wishes for women</title>";
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
  html += "<h1>❤️ Happy March 8! Below you can write and read wishes to women ❤️</h1>";

// Form for entering wishes
  html += "<form action='/addWish' method='POST'>";
  html += "<textarea name='wish' placeholder='Write your wish...'></textarea><br>";
  html += "<input type='submit' value='Отправить'>";
  html += "</form>";

// Display all wishes
  html += "<h2>All wishes:</h2>";
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

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wi-Fi...");
  }
  Serial.println("Connected to Wi-Fi");
  Serial.println(WiFi.localIP());

// EasyDDNS setup
  EasyDDNS.service(ddns_name); // Specify the DuckDNS service
  EasyDDNS.client(ddnsDomain, ddnsKey); // Specify the domain and key

  // Setting up server routes
  server.on("/", handleRoot);
  server.on("/addWish", handleAddWish);

  // Запуск сервера
  server.begin();
  Serial.println("HTTP server is running");
}

void loop() {
  server.handleClient();
  
  // Update DDNS via EasyDDNS
  EasyDDNS.update(1800000); // Update every 30 minutes (in milliseconds)
}
