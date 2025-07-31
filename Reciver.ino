#include <WiFi.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <HTTPClient.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char* ssid = "M14";
const char* password = "12345678";

WiFiUDP udp;
const int udpPort = 4210;
char incomingPacket[255];

// Buzzer pin
#define BUZZER_PIN 15

// ඔබගේ Google Apps Script webhook URL (ඔබගේ URL එක මෙතන දාන්න)
const char* googleScriptURL = "https://script.google.com/macros/s/AKfycbwnKz_v3cxoF9vdDvpQkhUb1DewptQ9kJIa34CbEGFjUgXPZT-wQOWq2fhyEi2Vmn0g/exec";

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Connecting WiFi...");
  display.display();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi connected!");
  Serial.print("📡 Receiver IP Address: ");
  Serial.println(WiFi.localIP());

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("WiFi connected");
  display.display();
  delay(1000);

  udp.begin(udpPort);
}

void loop() {
  int packetSize = udp.parsePacket();
  if (packetSize) {
    int len = udp.read(incomingPacket, 255);
    if (len > 0) incomingPacket[len] = 0;
    String data = String(incomingPacket);
    Serial.println("📥 Received: " + data);

    // Parse data (expected format: Temp:xx.xC, Humi:xx.x%, Pres:xxxx.xhPa, Water:xxx, Rain:xxx)
    float temp = parseValue(data, "Temp:", "C");
    float humi = parseValue(data, "Humi:", "%");
    float pres = parseValue(data, "Pres:", "hPa");
    int water = (int)parseValue(data, "Water:", ",");
    int rain = (int)parseValue(data, "Rain:", ",");

    // Display data on OLED
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Sensor Data:");
    display.printf("Temp: %.1f C\n", temp);
    display.printf("Humi: %.1f %%\n", humi);
    display.printf("Pres: %.1f hPa\n", pres);
    display.printf("Water: %d\n", water);
    display.printf("Rain: %d\n", rain);
    display.display();

    // Upload data to Google Sheets
    if(WiFi.status() == WL_CONNECTED){
      HTTPClient http;
      String url = String(googleScriptURL) + "?temp=" + String(temp) + "&humi=" + String(humi) + "&pres=" + String(pres) + "&water=" + String(water) + "&rain=" + String(rain);
      http.begin(url);
      int httpResponseCode = http.GET();
      if (httpResponseCode > 0) {
        Serial.println("Data uploaded to Google Sheets");
      } else {
        Serial.println("Error uploading data");
      }
      http.end();
    }

    // Buzzer Alert: Water sensor value > 1000 හෝ Rain sensor > 500 නම් beep එකක් දෙන්න
    if(water > 1000 || rain > 500){
      tone(BUZZER_PIN, 2000); // 2kHz beep
      delay(200);
      noTone(BUZZER_PIN);
    }
  }
}

float parseValue(String data, String keyStart, String keyEnd) {
  int startIndex = data.indexOf(keyStart);
  if(startIndex == -1) return NAN;
  startIndex += keyStart.length();

  int endIndex = data.indexOf(keyEnd, startIndex);
  if(endIndex == -1) return NAN;

  String valStr = data.substring(startIndex, endIndex);
  valStr.trim();
  return valStr.toFloat();
}