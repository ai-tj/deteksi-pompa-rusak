#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

const char* ssid = "Bot_Trojan"; // id wifi
const char* password = "asdfghjkl"; // password wi-fi

#define BOTtoken "7027418199:AAHpzfnRij3Q7EuJGEc_-hhFle8Ud_Vft34"
#define CHAT_ID "7410982280"

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

int lcdColumns = 16;
int lcdRows = 2;

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

int vs = 25;
int buzzer = 26;

unsigned long previousMillis = 0;
const long interval = 1000;
int vibrationCount = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Initializing LCD...");

  lcd.init();
  lcd.backlight();

  pinMode(vs, INPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);

  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println(" Connected!");
  client.setInsecure();
}

void loop() {
  if (digitalRead(vs) == HIGH) {
    vibrationCount++;
    delay(10);
  }

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    float frequency = vibrationCount / 10.0;
    vibrationCount = 0;

    lcd.clear();
    
    lcd.setCursor(0, 0);
    lcd.print("Frekuensi   ");
    lcd.setCursor(0, 1);
    lcd.print("Getaran   : ");
    lcd.print(frequency, 1);

    Serial.print("Measurement: "); 
    Serial.println(frequency);

    if (frequency >= 4.5 && frequency <= 8.0) { 
      String message = "PERINGATAN! Kavitasi terdeteksi: " + String(frequency);
      bot.sendMessage(CHAT_ID, message, "");

      digitalWrite(buzzer, HIGH);
      delay(1000);
      digitalWrite(buzzer, LOW);

      delay(1000);
    }
  }
}
