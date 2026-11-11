#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char* ssid = "Dungxnguyenzzzz";
const char* pass = "hoilamgi";
const char* broker = "test.mosquitto.org";
const char* SV_ID = "nhom10";

WiFiClient wifi;
PubSubClient mqtt(wifi);
String TOPIC_TELE;

unsigned long lan = 0;

float docNhiet() {
  return 27.0 + (random(0, 30) / 10.0);
}

float docAm() {
  return 65.0 + (random(0, 30) / 10.0);
}

void noiBroker() {
  while (!mqtt.connected()) {
    String cid = "ESP32_" + String(SV_ID) + "_" + String(random(0xffff), HEX);
    if (mqtt.connect(cid.c_str())) {
      Serial.println("MQTT connected");
    } else {
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  TOPIC_TELE = "aet2022/" + String(SV_ID) + "/telemetry";

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  mqtt.setServer(broker, 1883);
}

void loop() {
  if (!mqtt.connected()) noiBroker();
  mqtt.loop();

  if (millis() - lan >= 2000) {
    lan = millis();

    JsonDocument doc;
    doc["id"] = "25112031";
    doc["temp"] = docNhiet();
    doc["hum"] = docAm();
    doc["uptime"] = millis() / 1000;
    doc["rssi"] = WiFi.RSSI();

    char b[128];
    serializeJson(doc, b);
    mqtt.publish(TOPIC_TELE.c_str(), b);
    Serial.printf(">pub: %s\n", b);
  }
}
