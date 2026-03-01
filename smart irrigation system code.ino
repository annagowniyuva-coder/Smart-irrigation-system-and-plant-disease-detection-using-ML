#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT11
#define SOIL_PIN 34

#define IN1 18
#define IN2 19

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// 🔧 Adjust after calibration
int dryValue = 3600;   // sensor value in dry soil
int wetValue = 1500;   // sensor value in wet soil

int soilValue;
int soilPercent;
float humidity;

void setup() {
  Serial.begin(115200);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  // Motor OFF initially
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  dht.begin();
  lcd.init();
  lcd.backlight();
}

void loop() {

  // Read soil moisture
  soilValue = analogRead(SOIL_PIN);

  // Convert to percentage
  soilPercent = map(soilValue, dryValue, wetValue, 0, 100);
  soilPercent = constrain(soilPercent, 0, 100);

  // Read humidity
  humidity = dht.readHumidity();

  // 🌱 Pump control
  if (soilPercent < 30) {     // Soil is dry
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  } 
  else {                      // Soil is wet
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
  }

  // 📟 LCD Display
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Soil:");
  lcd.print(soilPercent);
  lcd.print("%");

  lcd.setCursor(10, 0);
  lcd.print("H:");
  lcd.print(humidity);

  lcd.setCursor(0, 1);
  if (soilPercent < 30)
    lcd.print("Pump: ON ");
  else
    lcd.print("Pump: OFF");

  delay(2000);
}
