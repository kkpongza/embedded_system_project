#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// #define LED_PIN 2
#define SOUND_SENSOR_PIN 34
#define MOTION_SENSOR_PIN 19
#define VIBRATE_SENSOR_PIN 2
#define INFRARED_SENSOR_PIN 23
#define ALARM_SENSOR_PIN 18

#define BLYNK_TEMPLATE_ID "TMPL6Op-cq5FV" 
#define BLYNK_TEMPLATE_NAME "Quickstart Template" 
#define BLYNK_AUTH_TOKEN "KOhYdpV6Oi2NVrq-4XcHpXR7KUFwPqr_" 

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

// Your WiFi credentials
char ssid[] = "Kiang (2)";
char pass[] = "kiangggg";

BlynkTimer timer;

LiquidCrystal_I2C lcd(0x27, 16, 2); // 16 columns, 2 rows
const int soundThreshold = 3500;    // Threshold for detecting sound
bool motionDetected = false;
bool soundDetected = false;
bool vibrateDetected = false;
bool infraredDetected = false;

void updateLCD() {
  lcd.clear();
  String message = "";

  if (motionDetected) message += "Motion ";
  if (soundDetected) message += "Sound ";
  if (vibrateDetected) message += "Vibrate ";
  if (infraredDetected) message += "Infrared ";

  if (message.length() == 0) {
    message = "Waiting for input";
  }

  lcd.setCursor(0, 0);
  lcd.print(message);
  Blynk.virtualWrite(V0, message);
}

void checkMotion() {
  int motionState = digitalRead(MOTION_SENSOR_PIN);
  Serial.print("Motion Sensor: ");
  Serial.println(motionState);
  motionDetected = (motionState == 1);
  Blynk.virtualWrite(V2, motionDetected);
}

void checkSound() {
  float soundLevel = analogRead(SOUND_SENSOR_PIN);
  float percent = (soundLevel / 4095) * 100;
  Blynk.virtualWrite(V1, soundLevel);
  Serial.print("Sound Level: ");
  Serial.println(percent);
  soundDetected = (soundLevel > soundThreshold);
}

void checkVibrate() {
  int vibrateState = digitalRead(VIBRATE_SENSOR_PIN);
  Serial.print("Vibrate Sensor: ");
  Serial.println(vibrateState);
  vibrateDetected = (vibrateState == 1);
  Blynk.virtualWrite(V3, vibrateDetected);
}

void checkInfrared() {
  int infraredState = digitalRead(INFRARED_SENSOR_PIN);
  Serial.print("Infrared Sensor: ");
  Serial.println(infraredState);
  infraredDetected = !(infraredState == 1);
  Blynk.virtualWrite(V4, infraredDetected);
}

void controlAlarm() {
  if (motionDetected || soundDetected  || infraredDetected || vibrateDetected) {
    digitalWrite(ALARM_SENSOR_PIN, HIGH);
    Blynk.virtualWrite(V5, "Alarm On");
    Serial.println("Alarm On");
  } else {
    digitalWrite(ALARM_SENSOR_PIN, LOW);
    Blynk.virtualWrite(V5, "Alarm Off");
    Serial.println("Alarm Off");
  }
}

void setup()
{
 Wire.begin();
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  pinMode(ALARM_SENSOR_PIN, OUTPUT);
  pinMode(MOTION_SENSOR_PIN, INPUT);
  pinMode(SOUND_SENSOR_PIN, INPUT);
  pinMode(VIBRATE_SENSOR_PIN, INPUT);
  pinMode(INFRARED_SENSOR_PIN, INPUT);
  digitalWrite(ALARM_SENSOR_PIN, LOW);

  timer.setInterval(500L, checkSound);
  timer.setInterval(600L, checkMotion);
  timer.setInterval(700L, checkVibrate);
  timer.setInterval(900L, checkInfrared);
  timer.setInterval(700L, updateLCD);
  timer.setInterval(600L, controlAlarm);

  Serial.println("Setup complete. Awaiting inputs...");
}

void loop()
{
  Blynk.run();
  timer.run();
}