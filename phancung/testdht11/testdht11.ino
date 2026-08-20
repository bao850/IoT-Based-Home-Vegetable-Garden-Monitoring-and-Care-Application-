#include <WiFi.h>
#include <FirebaseESP32.h>
#include "DHT.h"

#define DHTPIN 26
#define DHTTYPE DHT11 // hoặc DHT22

#define WIFI_SSID "Oanh-2"
#define WIFI_PASSWORD "covid21abc"
#define FIREBASE_AUTH "nJHUyfkUaKHdgZNZONKRs1RDgCxKrDnNMDcyYAbq"
#define FIREBASE_HOST "https://smartgarden-85ec4-default-rtdb.asia-southeast1.firebasedatabase.app"

FirebaseData firebaseData;
FirebaseAuth auth;
FirebaseConfig config;
DHT dht(DHTPIN, DHTTYPE);

// Biến thời gian và giờ đếm
unsigned long lastHour = 0;
int hourIndex = 1;

void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");

  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  Serial.println("Connected to Firebase!");
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(2000);
    return;
  }

  // Gửi dữ liệu hiện tại (live)
  Firebase.setFloat(firebaseData, "smart_garden/control/temp/live", t);
  Firebase.setFloat(firebaseData, "smart_garden/control/humi/live", h);

  Serial.print("Temp live: ");
  Serial.print(t);
  Serial.print(" | Humi live: ");
  Serial.println(h);

  // Mỗi 1 giờ (3600000 ms) lưu vào 1h, 2h, 3h... trong Firebase
  if (millis() - lastHour > 3600000) {
    String tempPath = "smart_garden/temp/" + String(hourIndex) + "h";
    String humiPath = "smart_garden/humi/" + String(hourIndex) + "h";

    Firebase.setFloat(firebaseData, tempPath, t);
    Firebase.setFloat(firebaseData, humiPath, h);

    Serial.print("Đã lưu dữ liệu vào giờ ");
    Serial.print(hourIndex);
    Serial.println("h");

    hourIndex++;
    if (hourIndex > 9) hourIndex = 1; // quay lại 1h sau khi tới 9h
    lastHour = millis();
  }

  delay(2000);
}
