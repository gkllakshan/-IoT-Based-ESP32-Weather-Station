#include <WiFi.h>
#include <WiFiUdp.h>
#include <DHT.h>
#include <Adafruit_BMP280.h>

#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

Adafruit_BMP280 bmp;

#define WATER_SENSOR_PIN 34
#define RAIN_SENSOR_PIN 35

const char* ssid = "M14";
const char* password = "12345678";

// මෙතන receiver board එකේ IP address එක දාන්න
const char* udpAddress = "192.168.120.73"; 
const int udpPort = 4210;

WiFiUDP udp;

void setup() {
  Serial.begin(115200);
  dht.begin();

  // BMP280 initialize with address 0x76 (try 0x77 if 0x76 fails)
  if (!bmp.begin(0x76)) {
    Serial.println("❌ BMP280 sensor not detected! Check wiring or I2C address.");
    while (1); // Stop execution here
  }

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\n✅ WiFi connected!");
  Serial.print("📡 Sender IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  float temp = dht.readTemperature();
  float humidity = dht.readHumidity();
  float pressure = bmp.readPressure() / 100.0F;  // pressure in hPa
  int water = analogRead(WATER_SENSOR_PIN);
  int rain = analogRead(RAIN_SENSOR_PIN);

  String data = "Temp:" + String(temp) + "C, Humi:" + String(humidity) +
                "%, Pres:" + String(pressure) + "hPa, Water:" + String(water) +
                ", Rain:" + String(rain);

  udp.beginPacket(udpAddress, udpPort);
  udp.print(data);
  udp.endPacket();

  Serial.println("📤 Sent: " + data);
  delay(3000);
}