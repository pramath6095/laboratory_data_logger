#include <DHT.h>
#include <Wire.h>
#include <LCD_I2C.h>
#include <SD.h>
#include <WiFi.h>
#include <WebServer.h>
#include <time.h>
#include <WiFiUdp.h>

const char* ssid = "LAB";
const char* password = "password";

#define SD_CS 5

#define DHTPIN 4
#define DHTTYPE DHT11
#define LDR_PIN 34
#define MQ_PIN  35

DHT dht(DHTPIN, DHTTYPE);
LCD_I2C lcd(0x27, 16, 2);

WebServer server(80);

float temp = 0.0, humidity = 0.0, ldrPercent = 0.0, gasPercent = 0.0;

// ✅ Global file name for SD logging
String filename = "/default.csv";

void handleRoot() {
  String html = "<html><head><meta http-equiv='refresh' content='5'>";
  html += "<title>Sensor Data</title></head><body>";
  html += "<h2>Live Sensor Data</h2>";
  html += "<p>Temperature: " + String(temp, 1) + " °C</p>";
  html += "<p>Humidity: " + String(humidity, 1) + " %</p>";
  html += "<p>Light Intensity: " + String(ldrPercent, 1) + " %</p>";
  html += "<p>Gas Level: " + String(gasPercent, 1) + " %</p>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  // WiFi setup
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected. IP: " + WiFi.localIP().toString());

  // Web server setup
  server.on("/", handleRoot);
  server.begin();
  Serial.println("Web server started.");

  // Time sync with NTP (IST = GMT+5:30 = 19800 seconds)
  configTime(19800, 0, "pool.ntp.org");
  Serial.print("Waiting for time sync");
  while (time(nullptr) < 100000) {
    Serial.print(".");
    delay(500);
  }
  Serial.println(" Time synced.");

  // Create filename based on date and time
  time_t now = time(nullptr);
  struct tm* timeinfo = localtime(&now);
  char fname[32];
  strftime(fname, sizeof(fname), "/%Y_%m_%d_%H_%M.csv", timeinfo);
  filename = String(fname);
  Serial.println("Log filename: " + filename);

  // Sensors and LCD init
  dht.begin();
  Wire.begin();
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("System Starting...");
  delay(2000);
  lcd.clear();

  // SD Card setup
  if (!SD.begin(SD_CS)) {
    Serial.println("❌ SD Card init failed!");
  } else {
    Serial.println("✅ SD Card initialized.");
    File file = SD.open(filename, FILE_WRITE);
    if (file) {
      file.println("Timestamp,Temperature,Humidity,Light %,Gas %");
      file.close();
      Serial.println("✔️ Log file created: " + filename);
    } else {
      Serial.println("❌ Could not create log file.");
    }
  }
}

void loop() {
  // Read sensors
  int ldrRaw = analogRead(LDR_PIN);
  ldrPercent = 100.0 - ((ldrRaw / 4095.0) * 100.0);

  int mqRaw = analogRead(MQ_PIN);
  gasPercent = (mqRaw / 4095.0) * 100.0;

  temp = dht.readTemperature();
  humidity = dht.readHumidity();

  // LCD output
  lcd.clear();
  lcd.setCursor(0, 0);
  if (!isnan(temp) && !isnan(humidity)) {
    lcd.print("T:");
    lcd.print(temp, 0);
    lcd.print("C H:");
    lcd.print(humidity, 0);
    lcd.print("%");
  } else {
    lcd.print("DHT Read Error");
  }

  lcd.setCursor(0, 1);
  lcd.print("L:");
  lcd.print(ldrPercent, 0);
  lcd.print("% G:");
  lcd.print(gasPercent, 0);
  lcd.print("%");

  // Timestamp
  time_t now = time(nullptr);
  struct tm* timeinfo = localtime(&now);
  char timeStr[16];
  strftime(timeStr, sizeof(timeStr), "%H:%M:%S", timeinfo);

  // Write to SD card
  File file = SD.open(filename, FILE_APPEND);
  if (!file) {
    Serial.println("❌ Failed to open file for appending.");
  } else if (isnan(temp) || isnan(humidity)) {
    Serial.println("❌ Sensor read returned NaN.");
    file.close();
  } else {
    file.print(timeStr);
    file.print(",");
    file.print(temp, 1);
    file.print(",");
    file.print(humidity, 1);
    file.print(",");
    file.print(ldrPercent, 1);
    file.print(",");
    file.println(gasPercent, 1);
    file.close();
  }

  // Handle web client
  server.handleClient();
  delay(1000);
}
