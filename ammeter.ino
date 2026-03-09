/*
 * DC Ammeter using Toroidal Core + Hall Effect Sensor (WSH130NL)
 *
 * Reads the Hall sensor output voltage via Arduino ADC,
 * converts it to current using calibration values.
 *
 *
 * Arduino Connections:
 * A0   <--  Sensor VOUT (Pin 3)
 * 5V   -->  Sensor VDD (Pin 1) + Pull-up resistor
 * GND  -->  Sensor GND (Pin 2)
 * SDA  -->  LCD SDA (I2C)
 * SCL  -->  LCD SCL (I2C)
 * 5V   -->  LCD VCC
 * GND  -->  LCD GND
 *
 *
 * Author: Sagar Saini
 * License: MIT
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ---- Pin Definitions ----
#define SENSOR_PIN A0

// ---- Calibration Values (UPDATE THESE WITH YOUR READINGS) ----
#define V_OFFSET    2.5    // Voltage at 0A (baseline) - measure and update
#define SENSITIVITY 0.05    // Volts per Ampere (V/A) - calculate from readings and update

// ---- ADC Configuration ----
#define ADC_RESOLUTION 1024.0
#define V_REF          5.0      // Arduino reference voltage
#define NUM_SAMPLES    50       // Number of samples for averaging

// ---- LCD Setup (I2C address 0x27, 16 columns, 2 rows) ----
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("DC Ammeter");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(2000);
  lcd.clear();

  // Set ADC reference
  analogReference(DEFAULT); // 5V reference

  Serial.println("================================");
  Serial.println(" DC Ammeter - Hall Effect Sensor");
  Serial.println("================================");
  Serial.println();
  Serial.println("ADC\tVoltage(V)\tCurrent(A)");
  Serial.println("---\t----------\t----------");
}

void loop() {
  // Read sensor with averaging for stable readings
  long adcSum = 0;
  for (int i = 0; i < NUM_SAMPLES; i++) {
    adcSum += analogRead(SENSOR_PIN);
    delayMicroseconds(100);
  }
  float adcAverage = (float)adcSum / NUM_SAMPLES;

  // Convert ADC value to voltage
  float voltage = (adcAverage / ADC_RESOLUTION) * V_REF;

  // Convert voltage to current using calibration
  float current = 0.0;
  if (SENSITIVITY != 0.0) {
    current = (voltage - V_OFFSET) / SENSITIVITY;
  }

  // Prevent negative current display (noise at zero)
  if (current < 0.01) {
    current = 0.0;
  }

  // Print to Serial Monitor
  Serial.print((int)adcAverage);
  Serial.print("\t");
  Serial.print(voltage, 3);
  Serial.print("\t\t");
  Serial.print(current, 3);
  Serial.println(" A");

  // Display on LCD
  lcd.setCursor(0, 0);
  lcd.print("V: ");
  lcd.print(voltage, 3);
  lcd.print(" V    ");

  lcd.setCursor(0, 1);
  lcd.print("I: ");
  lcd.print(current, 3);
  lcd.print(" A    ");

  delay(500); // Update every 500ms
}
