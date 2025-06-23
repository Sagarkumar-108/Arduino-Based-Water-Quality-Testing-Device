#define BLYNK_TEMPLATE_ID "TMPL3q12bUCme"
#define BLYNK_TEMPLATE_NAME "smart watering system"
#define BLYNK_AUTH_TOKEN "ra7m-tjjj6A6N3TCyFN-Nx2VptjzBP"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// WiFi credentials
char ssid[] = "Reecha";           // Replace with your WiFi SSID
char pass[] = "123456789";        // Replace with your WiFi Password

#define RELAY_PIN D1              // GPIO5 on NodeMCU
#define SOIL_SENSOR_PIN A0        // Analog pin on ESP8266
#define PUMP_CONTROL_PIN V1       // Virtual pin in Blynk app

BlynkTimer timer;

void setup() {
  Serial.begin(115200);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);  // Pump OFF initially (active LOW)

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(2000L, checkSoilMoisture);  // Every 2 seconds
}

// Manual control from Blynk app
BLYNK_WRITE(PUMP_CONTROL_PIN) {
  int value = param.asInt();
  digitalWrite(RELAY_PIN, value ? LOW : HIGH);  // LOW = ON
}

void checkSoilMoisture() {
  int moistureValue = analogRead(SOIL_SENSOR_PIN);
  int moisturePercent = map(moistureValue, 1023, 0, 0, 100);  // Adjust for your sensor

  Serial.print("Soil Moisture: ");
  Serial.print(moisturePercent);
  Serial.println("%");

  if (moisturePercent < 30) {
    digitalWrite(RELAY_PIN, LOW);   // Turn pump ON
  } else {
    digitalWrite(RELAY_PIN, HIGH);  // Turn pump OFF
  }

  Blynk.virtualWrite(V0, moisturePercent);  // Optional gauge in app
}

void loop() {
  Blynk.run();
  timer.run();
}