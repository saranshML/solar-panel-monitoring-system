/********************************************************
   SOLAR PANEL MONITORING SYSTEM V2
   XIAO ESP32-C3
   ADS1115
   ACS758-50B
   DS18B20
   16x2 I2C LCD
   Blynk IoT
********************************************************/

#define BLYNK_TEMPLATE_ID "YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "Solar PV Monitoring"
#define BLYNK_AUTH_TOKEN "YOUR_AUTH_TOKEN"

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_ADS1X15.h>
#include <BlynkSimpleEsp32.h>
#include <OneWire.h>
#include <DallasTemperature.h>

//================ WiFi ==================
char ssid[] = "YOUR_WIFI_NAME";
char pass[] = "YOUR_WIFI_PASSWORD";

//================ LCD ===================
LiquidCrystal_I2C lcd(0x27, 16, 2);

//================ ADS1115 ===============
Adafruit_ADS1115 ads;

//================ DS18B20 ===============
#define ONE_WIRE_BUS 4

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

//============== Calibration =============

// Voltage Divider
const float R1 = 100000.0;
const float R2 = 10000.0;

// ACS758 Parameters
const float sensitivity = 0.040; // 40mV/A
const float offsetVoltage = 2.50;

// ADS1115 LSB
const float ADS_LSB = 0.1875 / 1000.0;

// Electricity Cost
const float costPerkWh = 6.0;

//============== Variables ===============

float voltage = 0;
float current = 0;
float power = 0;
float energy = 0;
float temperature = 0;
float savings = 0;

unsigned long previousMillis = 0;

//============== Blynk Timer =============
BlynkTimer timer;

//========================================
// Read Sensors
//========================================

void readSensors()
{
  // ADS1115 Channel A0 = Voltage
  int16_t voltageRaw = ads.readADC_SingleEnded(0);

  float adcVoltage = voltageRaw * ADS_LSB;

  voltage = adcVoltage * ((R1 + R2) / R2);

  if (voltage < 0.1)
    voltage = 0;

  // ADS1115 Channel A1 = ACS758

  int16_t currentRaw = ads.readADC_SingleEnded(1);

  float currentADCVoltage = currentRaw * ADS_LSB;

  current =
      (currentADCVoltage - offsetVoltage) /
      sensitivity;

  if (abs(current) < 0.05)
    current = 0;

  current = abs(current);

  // Temperature

  sensors.requestTemperatures();
  temperature = sensors.getTempCByIndex(0);

  // Power

  power = voltage * current;

  // Energy

  unsigned long currentMillis = millis();

  float elapsedHours =
      (currentMillis - previousMillis) /
      3600000.0;

  energy += power * elapsedHours;

  previousMillis = currentMillis;

  // Savings

  savings = (energy / 1000.0) * costPerkWh;
}

//========================================
// Update LCD
//========================================

void updateLCD()
{
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("V:");
  lcd.print(voltage, 1);

  lcd.setCursor(9, 0);
  lcd.print("I:");
  lcd.print(current, 1);

  lcd.setCursor(0, 1);
  lcd.print("P:");
  lcd.print(power, 1);
  lcd.print("W");
}

//========================================
// Send to Blynk
//========================================

void sendToBlynk()
{
  Blynk.virtualWrite(V0, voltage);
  Blynk.virtualWrite(V1, current);
  Blynk.virtualWrite(V2, power);
  Blynk.virtualWrite(V3, energy);
  Blynk.virtualWrite(V4, temperature);
  Blynk.virtualWrite(V5, savings);
}

//========================================
// Main Update Function
//========================================

void updateSystem()
{
  readSensors();
  updateLCD();
  sendToBlynk();

  Serial.println("---------------------");
  Serial.print("Voltage: ");
  Serial.println(voltage);

  Serial.print("Current: ");
  Serial.println(current);

  Serial.print("Power: ");
  Serial.println(power);

  Serial.print("Energy: ");
  Serial.println(energy);

  Serial.print("Temp: ");
  Serial.println(temperature);

  Serial.print("Savings: ");
  Serial.println(savings);
}

//========================================
// Setup
//========================================

void setup()
{
  Serial.begin(115200);

  Wire.begin();

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Solar Monitor");

  if (!ads.begin())
  {
    lcd.clear();
    lcd.print("ADS1115 Error");

    while (1);
  }

  ads.setGain(GAIN_TWOTHIRDS);

  sensors.begin();

  WiFi.begin(ssid, pass);

  lcd.setCursor(0, 1);
  lcd.print("Connecting");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Blynk.begin(
      BLYNK_AUTH_TOKEN,
      ssid,
      pass);

  previousMillis = millis();

  timer.setInterval(
      2000L,
      updateSystem);

  lcd.clear();
  lcd.print("Connected");
  delay(1500);
}

//========================================
// Loop
//========================================

void loop()
{
  Blynk.run();
  timer.run();
}
