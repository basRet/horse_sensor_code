/*
  ESP-NOW Remote Sensor - Receiver (Multiple Version)
  esp-now-rcv.ino
  Receives Temperature & Humidity data from other ESP32 via ESP-NOW
  
  DroneBot Workshop 2022
  https://dronebotworkshop.com
*/

//yellow red blue
//first one always on (white), 3Y, 3R, 3B

// Include required libraries
#include <WiFi.h>
#include <esp_now.h>
#include <Adafruit_NeoPixel.h>

#define LED_COUNT 10
int leftLEDPin = D1;
int rightLEDPin = D2;
int rightLegPressure = 0;
int leftLegPressure = 0;
unsigned long previousMillis = 0;
unsigned long interval = 20;
bool connectionLiveLeft = false;
bool connectionLiveRight = false;
unsigned long connectionMadeMillisLeft = 0;
unsigned long connectionMadeMillisRight = 0;


Adafruit_NeoPixel leftStrip(LED_COUNT, leftLEDPin, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel rightStrip(LED_COUNT, rightLEDPin, NEO_GRB + NEO_KHZ800);

// Define data structure
typedef struct struct_message {
  short pressValue;
  bool leftLeg;
} struct_message;

// Create structured data object
struct_message myData;

// Callback function
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  // Get incoming data
  memcpy(&myData, incomingData, sizeof(myData));

  //mydata.leftleg tells us which leg sent this info.
  if (myData.leftLeg) {
    leftLegPressure = myData.pressValue;
    connectionMadeMillisLeft = millis();   //start timer to check connection
  } else {
    rightLegPressure = myData.pressValue;
    connectionMadeMillisRight = millis();   //start timer to check connection
  }
}

void setup() {


  //LED strip setup
  rightStrip.begin();  // INITIALIZE NeoPixel strip object (REQUIRED)
  leftStrip.begin();
  rightStrip.show();  // Turn OFF all pixels
  leftStrip.show();
  rightStrip.setBrightness(250);  // Set BRIGHTNESS to about 1/5 (max = 255)
  leftStrip.setBrightness(250);

  // Start ESP32 in Station mode
  WiFi.mode(WIFI_STA);

  // Initalize ESP-NOW
  if (esp_now_init() != 0) {
    // Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register callback function
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  //change 2000 to proper values
  unsigned long currentMillis = millis();
  connectionLiveLeft = !(connectionMadeMillisLeft + 200 < currentMillis);  //if it hasn't created a connection for more than 100 ms, turn leds white
    connectionLiveRight = !(connectionMadeMillisRight + 200 < currentMillis);


  if (currentMillis - previousMillis > interval) {  //update LED's 50 times a second
    previousMillis = currentMillis;
    changeLEDStrips(true, map(leftLegPressure, 0, 2000, 0, 10));
    changeLEDStrips(false, map(rightLegPressure, 0, 2000, 0, 10));
  }
}

void changeLEDStrips(bool leftStripBoolean, byte intensity) {  //intensity ranges from 1-10, leftstrip true means change left strip, false means change right strip
  if (leftStripBoolean) {
    leftStrip.clear();

    if (connectionLiveLeft) {  //set status LED's
      leftStrip.setPixelColor(0, 0, 255, 0);
    } else {
      leftStrip.setPixelColor(0, 255, 255, 255);
    }

    for (byte i = 1; i < intensity; i++) {
      // Serial.println(i);
      if (i < 4) {
        leftStrip.setPixelColor(i, 255, 255, 0);
      } else if (i < 7) {
        leftStrip.setPixelColor(i, 255, 0, 0);
      } else if (i < 11) {
        leftStrip.setPixelColor(i, 0, 0, 255);
      } else {
        // Serial.println("led strip overreach error");
      }
    }
    leftStrip.show();  // Update strip with new contents
  } else {
    rightStrip.clear();

    if (connectionLiveRight) {  //set status LED's
      rightStrip.setPixelColor(0, 0, 255, 0);
    } else {
      rightStrip.setPixelColor(0, 255, 255, 255);
    }

    for (byte i = 1; i < intensity; i++) {
      if (i < 4) {
        rightStrip.setPixelColor(i, 255, 255, 0);
      } else if (i < 7) {
        rightStrip.setPixelColor(i, 255, 0, 0);
      } else if (i < 11) {
        rightStrip.setPixelColor(i, 0, 0, 255);
      } else {
        // Serial.println("led strip overreach error");
      }
    }
    rightStrip.show();  // Update strip with new contents
  }
}

void showCaseLEDS() {
  changeLEDStrips(true, 1);
  delay(100);
  changeLEDStrips(true, 2);
  delay(100);
  changeLEDStrips(true, 3);
  delay(100);
  changeLEDStrips(true, 4);
  delay(100);
  changeLEDStrips(true, 5);
  delay(100);
  changeLEDStrips(true, 6);
  delay(100);
  changeLEDStrips(true, 7);
  delay(100);
  changeLEDStrips(true, 8);
  delay(100);
  changeLEDStrips(true, 9);
  delay(100);
  changeLEDStrips(true, 10);
  delay(200);

  changeLEDStrips(false, 1);
  delay(100);
  changeLEDStrips(false, 2);
  delay(100);
  changeLEDStrips(false, 3);
  delay(100);
  changeLEDStrips(false, 4);
  delay(100);
  changeLEDStrips(false, 5);
  delay(100);
  changeLEDStrips(false, 6);
  delay(100);
  changeLEDStrips(false, 7);
  delay(100);
  changeLEDStrips(false, 8);
  delay(100);
  changeLEDStrips(false, 9);
  delay(100);
  changeLEDStrips(false, 10);
  delay(200);

  changeLEDStrips(false, 10);
  delay(100);
  changeLEDStrips(false, 9);
  delay(100);
  changeLEDStrips(false, 8);
  delay(100);
  changeLEDStrips(false, 7);
  delay(100);
  changeLEDStrips(false, 6);
  delay(100);
  changeLEDStrips(false, 5);
  delay(100);
  changeLEDStrips(false, 4);
  delay(100);
  changeLEDStrips(false, 3);
  delay(100);
  changeLEDStrips(false, 2);
  delay(100);
  changeLEDStrips(false, 1);
  delay(200);


  changeLEDStrips(true, 10);
  delay(100);
  changeLEDStrips(true, 9);
  delay(100);
  changeLEDStrips(true, 8);
  delay(100);
  changeLEDStrips(true, 7);
  delay(100);
  changeLEDStrips(true, 6);
  delay(100);
  changeLEDStrips(true, 5);
  delay(100);
  changeLEDStrips(true, 4);
  delay(100);
  changeLEDStrips(true, 3);
  delay(100);
  changeLEDStrips(true, 2);
  delay(100);
  changeLEDStrips(true, 1);
  delay(200);
}