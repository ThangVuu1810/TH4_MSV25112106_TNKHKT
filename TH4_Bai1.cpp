#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

const char* ssid = "Dungxnguyenzzzz";
const char* pass = "hoilamgi";

WebServer server(80);

float docNhiet() {
  return 26.5 + (random(0, 30) / 10.0);
}

float docAm() {
  return 60.0 + (random(0, 40) / 10.0);
}

void handleSensor() {
  JsonDocument doc;
  doc["id"] = "25112031";
  doc["temp"] = docNhiet();
  doc["hum"] = docAm();
  doc["uptime"] = millis() / 1000;
  doc["rssi"] = WiFi.RSSI();

  String out;
  serializeJson(doc, out);
  server.send(200, "application/json", out);
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("\nIP: ");
  Serial.println(WiFi.localIP());

  server.on("/api/sensor", HTTP_GET, handleSensor);
  server.begin();
}

void loop() {
  server.handleClient();
}
