// Define Blynk Template ID and Name before including Blynk headers
#define BLYNK_TEMPLATE_ID "TMPL6Op-cq5FV" 
#define BLYNK_TEMPLATE_NAME "Quickstart Template" 
#define BLYNK_AUTH_TOKEN "KOhYdpV6Oi2NVrq-4XcHpXR7KUFwPqr_" 

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <ESP_Mail_Client.h>

// Define sensor pins
#define SOUND_SENSOR_PIN 34
#define MOTION_SENSOR_PIN 19
#define VIBRATE_SENSOR_PIN 2
#define INFRARED_SENSOR_PIN 23
#define ALARM_SENSOR_PIN 18

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

// SMTP Configuration
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465 // Typically 465 for SSL or 587 for TLS
#define AUTHOR_EMAIL "topten.watthana@gmail.com" // **Ensure this is secure**
#define AUTHOR_PASSWORD "gydy zgqp bgjb pkjk"          // **Ensure this is secure**

// Email Details
#define RECIPIENT_EMAIL "thornthantham@gmail.com"
#define EMAIL_SUBJECT "Alarm Triggered Notification"
#define EMAIL_MESSAGE "An alarm has been triggered by your security system."

// Your WiFi credentials
char ssid[] = "Topten_2.4G";
char pass[] = "0945710162";

BlynkTimer timer;

LiquidCrystal_I2C lcd(0x27, 16, 2); // 16 columns, 2 rows
const int soundThreshold = 3500;    // Threshold for detecting sound
bool motionDetected = false;
bool soundDetected = false;
bool vibrateDetected = false;
bool infraredDetected = false;
bool alarmActive = false;

SMTPSession smtp;

// Variables to handle email timing
unsigned long lastEmailSentTime = -300000;
bool alarmPreviouslyActive = false;
const unsigned long emailInterval = 300000; // 5 minutes in milliseconds

void sendEmailNotification() {
  smtp.debug(1);

  ESP_Mail_Session session;
  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;

  SMTP_Message message;
  message.sender.name = "Alarm System";
  message.sender.email = AUTHOR_EMAIL;
  message.subject = EMAIL_SUBJECT;
  message.text.content = EMAIL_MESSAGE;
  message.addRecipient("Recipient Name", RECIPIENT_EMAIL);

  if (!smtp.connect(&session)) {
    Serial.println("Could not connect to SMTP server.");
    return;
  }

  if (!MailClient.sendMail(&smtp, &message)) {
    Serial.println("Error sending Email, ");
    Serial.println(smtp.errorCode());
  } else {
    Serial.println("Email sent successfully.");
  }

  smtp.closeSession();
}

void emailTask(void * parameter) {
  sendEmailNotification();
  vTaskDelete(NULL); 
}

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
  infraredDetected = !(infraredState == 1); // Assuming active LOW
  Blynk.virtualWrite(V4, infraredDetected);
}

void controlAlarm() {
  
  alarmActive = motionDetected || soundDetected || vibrateDetected || infraredDetected;


  if (alarmActive) {
    digitalWrite(ALARM_SENSOR_PIN, HIGH);
    Blynk.virtualWrite(V5, "Alarm On");

    unsigned long currentTime = millis();

      // Check if 5 minutes have passed since the last email
    if (currentTime - lastEmailSentTime >= emailInterval) {
      // Send another email
      xTaskCreate(emailTask, "Email Task", 8192, NULL, 1, NULL);        
      lastEmailSentTime = currentTime;
    }

    Serial.println("Alarm On");

  } else {
    digitalWrite(ALARM_SENSOR_PIN, LOW);
    Blynk.virtualWrite(V5, "Alarm Off");
    Serial.println("Alarm Off");
  }

}

void setup() {
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
  timer.setInterval(1000L, checkInfrared);
  timer.setInterval(500L, updateLCD);
  timer.setInterval(300L, controlAlarm);

  Serial.println("Setup complete. Awaiting inputs...");
}

void loop() {
  Blynk.run();
  timer.run();
}