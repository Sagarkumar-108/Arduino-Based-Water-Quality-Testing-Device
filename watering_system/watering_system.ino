#include <Wire.h> 
#include <LiquidCrystal_I2C.h> 

// Initialize the LCD with I2C address 0x27, 16 columns, and 2 rows
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pin and calibration constants
const int TDS_PIN = A0;               // Analog pin connected to TDS sensor
const float VREF = 5.0;               // Analog reference voltage
const int SAMPLES = 10;               // Number of readings to average
const float TDS_THRESHOLD = 300.0;    // TDS threshold in ppm for water quality

void setup() {
  lcd.init();            // Initialize LCD
  lcd.backlight();       // Turn on LCD backlight
  lcd.print("Initializing...");
  delay(2000);

  lcd.clear();
  lcd.print("TDS Monitor Ready");
  delay(1500);
  lcd.clear();
}

void loop() {
  int tdsValue = getAverageTDS();                        // Average analog readings
  float voltage = tdsValue * (VREF / 1024.0);            // Convert ADC value to voltage
  float tds = (133.42 * voltage * voltage * voltage      // TDS estimation formula
              - 255.86 * voltage * voltage
              + 857.39 * voltage) * 0.5;

  lcd.setCursor(0, 0);
  lcd.print("TDS: ");
  lcd.print(tds, 1);                                     // Show one decimal point
  lcd.print(" ppm   ");                                  // Extra spaces clear old values

  lcd.setCursor(0, 1);
  if (tds > TDS_THRESHOLD) {
    lcd.print("Water Unsafe!  ");
  } else {
    lcd.print("Water Safe     ");
  }

  delay(1000);  // Wait 1 second before next reading
}

// Function to get average ADC value
int getAverageTDS() {
  int total = 0;
  for (int i = 0; i < SAMPLES; i++) {
    total += analogRead(TDS_PIN);
    delay(10);  // Small delay between samples
  }
  return total / SAMPLES;
}