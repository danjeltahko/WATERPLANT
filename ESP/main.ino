#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <time.h>

#include "secrets.h"

const int MQTT_PORT = 8883;
const char MQTT_PUB_TOPIC[] = "esp8266/pub";

BearSSL::X509List cert(cacert);
BearSSL::X509List client_crt(client_cert);
BearSSL::PrivateKey key(privkey);

WiFiClientSecure wifi_client;
PubSubClient mqtt_client(wifi_client);

const int sensor = A0;         // Soil Sensor input at Analog PIN A0
const int delay_time = 1800;   // Seconds (every 30min)
unsigned long prev_millis = 0;
time_t now;
time_t nowish = 1510592825;
struct tm timeinfo;

void NTP_connect(void) {
    // get time from NTP server
    configTime(TIME_ZONE * 3600, 0, "pool.ntp.org", "time.nist.gov");
    now = time(nullptr);
    while (now < nowish) {
        delay(500);
        Serial.print(".");
        now = time(nullptr);
    }
    Serial.println("done!");
    gmtime_r(&now, &timeinfo);
    Serial.print("Current time: ");
    Serial.print(asctime(&timeinfo));
}

void msg_received(char *topic, byte *payload, unsigned int len) {} // set callback template

void mqtt_init() {
    // connect to mqtt broker (AWS IoT)
    Serial.print("Connecting to AWS IOT");
    while (!mqtt_client.connected()) {
        if (mqtt_client.connect(THINGNAME)) {
            Serial.println("connected!");
        } else {
            Serial.print("failed < trying again in 5 seconds");
            delay(5000);
        }
    }
}

void wifi_init() {
    // connect to wifi
    WiFi.hostname(THINGNAME);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("WiFi connected, IP adress: ");
    Serial.println(WiFi.localIP());
}

void send_msg() {
    // send json object to mqtt broker (AWS IoT)
    StaticJsonDocument<200> doc;
    doc["date"] = asctime(&timeinfo);
    doc["moisture"] = analogRead(sensor);
    char jsonBuffer[512];
    serializeJson(doc, jsonBuffer);
    mqtt_client.publish(MQTT_PUB_TOPIC, jsonBuffer);
}

void setup() {
    // general setup
    Serial.begin(115200);
    // connect to wifi & configure wifi_client
    wifi_init();
    // get time from server
    NTP_connect();
    // sets certificate and keys for AWS connection
    wifi_client.setTrustAnchors(&cert);
    wifi_client.setClientRSACert(&client_crt, &key);
    // sets mqtt endpoint & port for AWS connection
    mqtt_client.setServer(MQTT_HOST, MQTT_PORT);
    mqtt_client.setCallback(msg_received);
    // connect to AWS broker & send message 
    mqtt_init();
    send_msg();
}

void loop() {
    // sends plant moist from soil sensor every 30 min
    // and connects to mqtt broker again if disconnected
    now = time(nullptr);
    if (!mqtt_client.connected()) {
        mqtt_init();
    } else {
        mqtt_client.loop();
        delay(2000);
        if (millis() - prev_millis >= (delay_time*1000)) {
            send_msg();
            prev_millis = millis();
        }
    }
}