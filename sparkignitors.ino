#include <Wire.h>
#include <DHT.h>
#include <MPU6050_tockn.h>
#include <TinyGPS++.h>

#define DHTPIN 18
#define DHTTYPE DHT11

#define LED_PIN 2

#define MPU6050_SDA 21
#define MPU6050_SCL 22

#define ALCOHOL_SENSOR_PIN A0

DHT dht(DHTPIN, DHTTYPE);
MPU6050 mpu6050(Wire);
TinyGPSPlus gps;

void setup() {
  Serial.begin(115200);
  Wire.begin(MPU6050_SDA, MPU6050_SCL);
  dht.begin();
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  float alcoholLevel = getAlcoholLevel();
  float acceleration = getAcceleration();

  if (temperature > 30 || humidity > 70) {
    sendAlert("High temperature or humidity detected!");
  }

  if (alcoholLevel > 0.5) {
    sendAlert("Alcohol detected!");
  }

  if (acceleration > 10) {
    sendAlert("Excessive acceleration detected!");
    sendAccidentAlert();
  }

  delay(1000);
}

float getAlcoholLevel() {
  int alcoholValue = analogRead(ALCOHOL_SENSOR_PIN);
  float alcoholLevel = alcoholValue * 5.0 / 1023.0;
  return alcoholLevel;
}

float getAcceleration() {
  mpu6050.update();
  float accelerationX = mpu6050.getAccX();
  float accelerationY = mpu6050.getAccY();
  float accelerationZ = mpu6050.getAccZ();
  float acceleration = sqrt(pow(accelerationX, 2) + pow(accelerationY, 2) + pow(accelerationZ, 2));
  return acceleration;
}

void sendAlert(String message) {
  digitalWrite(LED_PIN, HIGH);
  delay(1000);
  digitalWrite(LED_PIN, LOW);
  delay(1000);
  Serial.println(message);
}

void sendAccidentAlert() {
  Serial.println("Accident detected! Sending emergency alert...");

  // Replace with your GSM module's AT command instructions
  Serial.println("AT"); // Send AT command to initialize communication
  delay(500);
  Serial.println("AT+CMGF=1"); // Set SMS mode to text
  delay(500);
  Serial.print("AT+CMGS=\"9019457539\"\r"); // Replace with emergency contact number
  delay(500);
  Serial.print("Accident detected! Please check the location."); // SMS message content
  Serial.write(26); // Ctrl+Z to send the SMS
  delay(500);
}