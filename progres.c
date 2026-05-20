#define BLYNK_TEMPLATE_ID "TMPLxxxx"
#define BLYNK_TEMPLATE_NAME "Monitoring Sensor"
#define BLYNK_AUTH_TOKEN "YourAuthToken"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

char ssid[] = "NamaWiFi";
char pass[] = "PasswordWiFi";

// Pin sensor dan aktuator
#define DHTPIN 4
#define DHTTYPE DHT11
#define LDRPIN 34
#define LEDPIN 2

DHT dht(DHTPIN, DHTTYPE);

BlynkTimer timer;

// Fungsi kirim data sensor ke Blynk
void sendSensor()
{
  float suhu = dht.readTemperature();
  float kelembaban = dht.readHumidity();

  int cahaya = analogRead(LDRPIN);

  // Cek pembacaan sensor
  if (isnan(suhu) || isnan(kelembaban)) {
    Serial.println("Gagal membaca DHT11");
    return;
  }

  // Kirim ke Blynk
  Blynk.virtualWrite(V0, suhu);
  Blynk.virtualWrite(V1, kelembaban);
  Blynk.virtualWrite(V2, cahaya);

  // Tampilkan di Serial Monitor
  Serial.print("Suhu: ");
  Serial.print(suhu);

  Serial.print(" | Kelembaban: ");
  Serial.print(kelembaban);

  Serial.print(" | Cahaya: ");
  Serial.println(cahaya);
}

// Kontrol LED dari Blynk
BLYNK_WRITE(V3)
{
  int nilai = param.asInt();

  digitalWrite(LEDPIN, nilai);
}

void setup()
{
  Serial.begin(115200);

  pinMode(LEDPIN, OUTPUT);

  dht.begin();

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Kirim data setiap 2 detik
  timer.setInterval(2000L, sendSensor);
}

void loop()
{
  Blynk.run();
  timer.run();
}