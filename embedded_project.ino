// #define BLYNK_TEMPLATE_ID "TMPL6if3X-9yu" 
// #define BLYNK_TEMPLATE_NAME "Quickstart Template" 
// #define BLYNK_AUTH_TOKEN "kLJ8g3mNLJxJHHnqhXdLFbpL_bpDPMiB" 

// /* Comment this out to disable prints and save space */
// #define BLYNK_PRINT Serial

// #include <WiFi.h>
// #include <WiFiClient.h>
// #include <LiquidCrystal_I2C.h>
// #include <BlynkSimpleEsp32.h>


// // Your WiFi credentials
// char ssid[] = "Kiang (2)";
// char pass[] = "kiangggg";

// BlynkTimer timer;

// #define LED_PIN 2 // Define LED pin
// #define SOUND_SENSOR_PIN 34 // Sound sensor connected to GPIO 34 (Analog Input)

// LiquidCrystal_I2C lcd(0x27, 16, 2); // 16 columns, 2 rows
// const int soundThreshold = 3200;

// void updateLCD(const char* message) {

//   lcd.clear();
//   lcd.setCursor(0, 0); // Set cursor to the first row
//   lcd.print(message);

//   // Blynk.virtualWrite(V1, "clr");        // Clear LCD screen
//   Blynk.virtualWrite(V1, message); // Display the message on the first row
// }

// void checkSound() {
//   float soundLevel = analogRead(SOUND_SENSOR_PIN); 

//   float percent = (soundLevel / 4095) * 100;
//   Blynk.virtualWrite(V3, percent); // Send the decibel value to Virtual Pin V3

//   if (soundLevel > soundThreshold) { // Adjust `soundThreshold` for dB
//     updateLCD("Sound Detected");
//     digitalWrite(LED_PIN, HIGH); // Turn on the LED to indicate detection
//   } else {
//     updateLCD("No Sound");
//     digitalWrite(LED_PIN, LOW); // Turn off the LED
//   }
// }


// // This function is called every time the Virtual Pin 0 state changes
// BLYNK_WRITE(V0)
// {
//   // Set incoming value from pin V0 to a variable
//   int value = param.asInt();

//   // Update LED state based on the value from V0
//   digitalWrite(LED_PIN, value);

//   // Optionally, send the value to another Virtual Pin (V1) for tracking
//   if (value == 1) {
//     updateLCD("LED ON");
//   } else {
//     updateLCD("LED OFF");
//   }
// }

// // This function sends Arduino's uptime every second to Virtual Pin 2
// // void myTimerEvent()
// // {
// //   // You can send any value at any time.
// //   // Please don't send more than 10 values per second.
// //   Blynk.virtualWrite(V2, millis() / 1000);
// // }

// void setup()
// {
//   Wire.begin(); // Initialize I2C
//   // Debug console
//   Serial.begin(115200);

//   // Set LED pin as output
//   pinMode(LED_PIN, OUTPUT);
//   digitalWrite(LED_PIN, LOW); // Turn off LED initially

//   lcd.init();
//   lcd.backlight(); // Turn on the LCD backlight

//   Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
//   // You can also specify server:
//   //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
//   //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, IPAddress(192,168,1,100), 8080);

//   // Setup a function to be called every second
//   // timer.setInterval(1000L, myTimerEvent);
//   updateLCD("Waiting for input");

//   timer.setInterval(500L, checkSound);
// }

// void loop()
// {
//   Blynk.run();
//   timer.run();
//   // You can inject your own code or combine it with other sketches.
//   // Check other examples on how to communicate with Blynk. Remember
//   // to avoid delay() function!
// }

// #include <Wire.h>
// #include <LiquidCrystal_I2C.h>
// #include <WiFi.h>

// #define LED_PIN 2          // Define LED pin
// #define SOUND_SENSOR_PIN 34 // Sound sensor connected to GPIO 34 (Analog Input)
// #define MOTION_SENSOR_PIN 19
// #define ALARM_SENSOR_PIN 18

// LiquidCrystal_I2C lcd(0x27, 16, 2); // 16 columns, 2 rows
// const int soundThreshold = 3200;    // Threshold for detecting sound
// bool alarmTriggered = false;

// void updateLCD(const char* message) {
//   lcd.clear();
//   lcd.setCursor(0, 0); // Set cursor to the first row
//   lcd.print(message);
// }

// void checkMotion() {
//   int motionState = digitalRead(MOTION_SENSOR_PIN); // Read the state of the motion sensor

//   // Print the motion sensor state to the Serial Monitor
//   Serial.print("Motion Sensor State: ");
//   Serial.println(motionState);

//   // If motion is detected, trigger the alarm and update the LCD
//   if (motionState == 1) {
//     updateLCD("Motion Detected");
//     // digitalWrite(ALARM_SENSOR_PIN, HIGH); // Trigger alarm
//     alarmTriggered = true;
//   } else {
//     // digitalWrite(ALARM_SENSOR_PIN, LOW);  // Turn off alarm
//     alarmTriggered = false;
//   }
// }

// void checkSound() {
//   float soundLevel = analogRead(SOUND_SENSOR_PIN); // Read the analog value from the sound sensor

//   // Calculate the sound level as a percentage of the max ADC value
//   float percent = (soundLevel / 4095) * 100;

//   // Print the sound level and percentage to the Serial Monitor
//   Serial.print("Sound Level (ADC): ");
//   Serial.print(soundLevel);
//   Serial.print(" -> Percent: ");
//   Serial.println(percent);

//   // Update LCD and LED based on sound level
//   if (soundLevel > soundThreshold) {
//     updateLCD("Sound Detected");
//     digitalWrite(LED_PIN, HIGH); // Turn on the LED
//     // digitalWrite(ALARM_SENSOR_PIN, HIGH);
//     alarmTriggered = true;
//   } else {
//     updateLCD("No Sound");
//     digitalWrite(LED_PIN, LOW); // Turn off the LED
//     // digitalWrite(ALARM_SENSOR_PIN, LOW);
//     alarmTriggered = false;
//   }
// }

// // This function is called when a button press changes the LED state
// void handleButtonPress(bool isLedOn) {
//   if (isLedOn) {
//     digitalWrite(LED_PIN, HIGH);
//     updateLCD("LED ON");
//   } else {
//     digitalWrite(LED_PIN, LOW);
//     updateLCD("LED OFF");
//   }
// }

// void controlAlarm() {
//   if (alarmTriggered) {
//     digitalWrite(ALARM_SENSOR_PIN, HIGH); // Trigger alarm
//     alarmTriggered = false; // Reset the flag after triggering
//   } else {
//     digitalWrite(ALARM_SENSOR_PIN, LOW);  // Turn off alarm
//   }
// }

// void setup() {
//   Wire.begin();           // Initialize I2C
//   Serial.begin(115200);   // Debug console

//   // Set up LED pin
//   pinMode(ALARM_SENSOR_PIN, OUTPUT);
//   pinMode(MOTION_SENSOR_PIN, INPUT);
//   digitalWrite(ALARM_SENSOR_PIN, LOW);
//   pinMode(LED_PIN, OUTPUT);
//   digitalWrite(LED_PIN, LOW); // Turn off the LED initially

//   // Initialize the LCD
//   lcd.init();
//   lcd.backlight(); // Turn on the LCD backlight
//   updateLCD("Waiting for input");

//   // Print initialization message
//   Serial.println("Setup complete. Waiting for sound...");
// }

// void loop() {
//   checkSound(); // Continuously check for sound
//   checkMotion();
//   controlAlarm();
//   delay(500);   // Add a delay to avoid overwhelming the loop
// }


#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>

#define LED_PIN 2           // Define LED pin
#define SOUND_SENSOR_PIN 34 // Sound sensor connected to GPIO 34 (Analog Input)
#define MOTION_SENSOR_PIN 19
#define ALARM_SENSOR_PIN 18

LiquidCrystal_I2C lcd(0x27, 16, 2); // 16 columns, 2 rows
const int soundThreshold = 3200;    // Threshold for detecting sound
bool motionDetected = false;        // Track motion detection
bool soundDetected = false;         // Track sound detection

void updateLCD(const char* message) {
  lcd.clear();
  lcd.setCursor(0, 0); // Set cursor to the first row
  lcd.print(message);
}

void checkMotion() {
  int motionState = digitalRead(MOTION_SENSOR_PIN); // Read the state of the motion sensor

  // Print the motion sensor state to the Serial Monitor
  Serial.print("Motion Sensor State: ");
  Serial.println(motionState);

  // Update motion detection status
  if (motionState == 1) {
    motionDetected = true;  // Motion detected
    updateLCD("Motion Detected");
  } else {
    motionDetected = false; // No motion
    updateLCD("Wating for input");
  }
}

void checkSound() {
  float soundLevel = analogRead(SOUND_SENSOR_PIN); // Read the analog value from the sound sensor

  // Calculate the sound level as a percentage of the max ADC value
  float percent = (soundLevel / 4095) * 100;

  // Print the sound level and percentage to the Serial Monitor
  Serial.print("Sound Level (ADC): ");
  Serial.print(soundLevel);
  Serial.print(" -> Percent: ");
  Serial.println(percent);

  // Update sound detection status
  if (soundLevel > soundThreshold) {
    soundDetected = true;  // Sound detected
    updateLCD("Sound Detected");
    digitalWrite(LED_PIN, HIGH); // Turn on the LED
  } else {
    soundDetected = false; // No sound
    updateLCD("Wating for input");
    digitalWrite(LED_PIN, LOW); // Turn off the LED
  }
}

void controlAlarm() {
  // Turn on the alarm if either motion or sound is detected
  if (motionDetected || soundDetected) {
    digitalWrite(ALARM_SENSOR_PIN, HIGH); // Trigger alarm
    Serial.println("Alarm Triggered!");
  } else {
    digitalWrite(ALARM_SENSOR_PIN, LOW);  // Turn off alarm
    Serial.println("Alarm Off.");
  }
}

void setup() {
  Wire.begin();           // Initialize I2C
  Serial.begin(115200);   // Debug console

  // Set up pins
  pinMode(ALARM_SENSOR_PIN, OUTPUT);
  pinMode(MOTION_SENSOR_PIN, INPUT);
  pinMode(SOUND_SENSOR_PIN, INPUT)
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(ALARM_SENSOR_PIN, LOW); // Ensure alarm is off initially
  digitalWrite(LED_PIN, LOW);          // Ensure LED is off initially

  // Initialize the LCD
  lcd.init();
  lcd.backlight(); // Turn on the LCD backlight
  updateLCD("Waiting for input");

  // Print initialization message
  Serial.println("Setup complete. Waiting for sound and motion...");
}

void loop() {
  checkSound();  // Continuously check for sound
  // checkMotion(); // Continuously check for motion
  controlAlarm(); // Control the alarm based on detections
  delay(500);     // Add a delay to avoid overwhelming the loop
}

