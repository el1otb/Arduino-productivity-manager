#include <WiFiS3.h>  // WiFi library for Arduino UNO R4 WiFi
#include "ThingSpeak.h"  // ThingSpeak library

// WiFi Credentials
const char* ssid = "wifi-ssid";  
const char* password = "wifi-password";

// ThingSpeak API Details
unsigned long myChannelNumber = "ThingSpeak Channel ID";  
const char* myWriteAPIKey = "ThingSpeak Write API Key";  

WiFiClient client;

#define PIR_PIN 2        // Motion sensor on D2 Digital
#define SOUND_SENSOR 3   // Sound sensor on Digital input 3

void setup() {
    Serial.begin(9600);
    
    pinMode(PIR_PIN, INPUT);  
    pinMode(SOUND_SENSOR, INPUT);

    Serial.print("Connecting to WiFi...");
    WiFi.begin(ssid, password);

    unsigned long startAttemptTime = millis();
    int timeoutSeconds = 30;  

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");

        if (millis() - startAttemptTime >= timeoutSeconds * 1000) {
            Serial.println("\nWiFi connection failed. Please check credentials and restart.");
            return;
        }
    }

    Serial.println("\nConnected to WiFi.");

    ThingSpeak.begin(client);  // Initialize ThingSpeak

    //  **Print CSV Header After WiFi Connects**
    Serial.println("Timestamp, Motion, Sound Level"); 
}

void loop() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi disconnected. Reconnecting...");
        WiFi.disconnect();
        delay(1000);
        WiFi.begin(ssid, password);
        return;
    }

    // **Read sensor values**
    int motion = digitalRead(PIR_PIN);
    int soundLevel = digitalRead(SOUND_SENSOR); 

    // **Generate Timestamp**
    unsigned long timeNow = millis() / 1000;  // Time in seconds

    // **Print Data in CSV Format**
    Serial.print(timeNow);
    Serial.print(", ");
    Serial.print(motion);
    Serial.print(", ");
    Serial.println(soundLevel); 

    // **Send Data to ThingSpeak**
    ThingSpeak.setField(1, motion);
    ThingSpeak.setField(2, soundLevel);
    ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

    delay(2000);  // Send every 2 seconds
}
