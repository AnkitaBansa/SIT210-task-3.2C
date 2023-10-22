#include <WiFiNINA.h>
#include <BH1750.h>
#include <Wire.h>

// Please enter your sensitive data in the Secret tab
char ssid[] = "Get lost";
char pass[] = "bansalankita27";

WiFiClient client;
BH1750 lightMeter;

char HOST_NAME[] = "maker.ifttt.com";
String PATH_NAME = "/trigger/Light/with/key/sjanjdnundUENFONDI";

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Connect to Wi-Fi
  connectToWiFi();
}

void loop() {
  // Read light data
  float lightLevel = readLightLevel();

  // Send data to IFTTT
  sendToIFTTT(lightLevel);

  // Wait for some time before reading and sending again
  delay(60000); // Delay for 1 minute
}

void connectToWiFi() {
  // Connect to Wi-Fi network
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.println("Attempting to connect to WiFi...");
    delay(1000);
  }

  Serial.println("Connected to WiFi");
}

float readLightLevel() {
  lightMeter.begin();
  float lightLevel = lightMeter.readLightLevel();
  return lightLevel;
}

void sendToIFTTT(float lightLevel) {
  // Create the URL for the IFTTT webhook
  String url = "http://" + String(HOST_NAME) + PATH_NAME + "?value1=" + String(lightLevel);

  // Make a GET request to the IFTTT webhook
  if (client.connect(HOST_NAME, 80)) {
    client.print("GET " + url + " HTTP/1.1\r\n");
    client.print("Host: " + String(HOST_NAME) + "\r\n");
    client.println("Connection: close\r\n");
    client.println();
    client.stop();
    Serial.println("Sent data to IFTTT");
  } else {
    Serial.println("Failed to connect to IFTTT");
  }
}