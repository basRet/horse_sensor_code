/*
  ESP-NOW Remote Sensor - Transmitter (Multiple Version)
  esp-now-xmit-multiple.ino
  Sends Temperature & Humidity data to other ESP32 via ESP-NOW
  Uses DHT22
  Multiple Transmitter modification
  
  DroneBot Workshop 2022
  https://dronebotworkshop.com
*/

// Include required libraries
#include <WiFi.h>
#include <esp_now.h>

// Define DHT22 parameters
int sensorPin = A0;
float sensorData = 0;
float sensorCalibrationValue = 1000;
bool leftLeg = true;

// Responder MAC Address (Replace with your responders MAC Address)
uint8_t broadcastAddress[] = { 0x84, 0xFC, 0xE6, 0x84, 0x3E, 0xA8 };
uint8_t broadcastAddressLaptop[] = { 0x84, 0xFC, 0xE6, 0x84, 0x32, 0x0C };

// Define data structure
typedef struct struct_message {
  short pressValue;
  bool leftLeg;
} struct_message;

// Create structured data object
struct_message myData;

// Register peer
esp_now_peer_info_t peerInfo;

// Sent data callback function
void OnDataSent(const uint8_t *macAddr, esp_now_send_status_t status) {
  // Serial.print("Last Packet Send Status: ");
  // Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {

  // Setup Serial monitor
  //Serial.begin(115200)
  delay(100);
  pinMode(sensorPin, INPUT);

  // Set ESP32 WiFi mode to Station temporarly
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != 0) {
    // Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Define callback
  esp_now_register_send_cb(OnDataSent);

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    // Serial.println("Failed to add peer");
    return;
  }

  memcpy(peerInfo.peer_addr, broadcastAddressLaptop, 6);
  
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    // Serial.println("Failed to add peer");
    return;
  }

  myData.leftLeg = leftLeg;  //tells hat which leg this info is
}

void loop() {
  sensorData = (short)(sensorCalibrationValue / (1.0 - analogRead(sensorPin) / 4095.0) - sensorCalibrationValue);

  // Add sensorData to structured data object
  myData.pressValue = sensorData;

  // Send data
  esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));
  esp_now_send(broadcastAddressLaptop, (uint8_t *)&myData, sizeof(myData));

  delay(100);
}