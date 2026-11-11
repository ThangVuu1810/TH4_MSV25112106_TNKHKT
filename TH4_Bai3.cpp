#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#define LED 26

const char* ssid = "Dungxnguyenzzzz";
const char* pass = "hoilamgi";
const char* broker = "test.mosquitto.org";
const char* SV_ID = "nhom10";

WiFiClient wifi;
PubSubClient mqtt(wifi);
String TOPIC_TELE;
String TOPIC_CMD;

unsigned long lan = 0;

float docNhiet() {
  return 28.0 + (random(0, 30) / 10.0);
}

float docAm() {
  return 62.0 + (random(0, 40) / 10.0);
}

void callback(char* topic, byte* payload, unsigned int len) {
  char cmd = (char)payload[0];
  if (cmd == '1') {
    digitalWrite(LED, HIGH);
  } else if (cmd == '0') {
    digitalWrite(LED, LOW);
  }
}

void noiBroker() {
  while (!mqtt.connected()) {
    String cid = "ESP32_B3_" + String(SV_ID) + "_" + String(random(0xffff), HEX);
    if (mqtt.connect(cid.c_str())) {
      mqtt.subscribe(TOPIC_CMD.c_str());
    } else {
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  TOPIC_TELE = "aet2022/" + String(SV_ID) + "/telemetry";
  TOPIC_CMD  = "aet2022/" + String(SV_ID) + "/cmd/led";

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  mqtt.setServer(broker, 1883);
  mqtt.setCallback(callback);
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

    char b[128];
    serializeJson(doc, b);
    mqtt.publish(TOPIC_TELE.c_str(), b);
  }
}
