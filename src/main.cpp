// #include <iostream>
#include "Arduino.h"
#include "ESP8266WiFi.h"
#include "SoftwareSerial.h"
#include <ESP8266HTTPClient.h>

String deviceName = "CS244";

// WiFi settings
const char *ssid = "UCInet Mobile Access";

void printMacAddress()
{
    byte mac[6];
    WiFi.macAddress(mac);

    char MAC_char[18]="";
    for (int i = 0; i < sizeof(mac); ++i)
    {
        sprintf(MAC_char, "%s%02x:", MAC_char, mac[i]);
    }

    Serial.print("Mac address : ");
    Serial.print(MAC_char);

    WiFi.begin(ssid); //, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    // Print the IP address
    Serial.println(WiFi.localIP());
}

void setup()
{
  // initialize LED digital pin as an output.
    pinMode(LED_BUILTIN, OUTPUT);

    // Serial.begin(115200);
    // Serial.println("Program started");
    // printMacAddress();

    Serial.begin(115200);
    delay(500);

    Serial.println();
    Serial.print("MAC: ");
    Serial.println(WiFi.macAddress());
}

void loop()
{
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
}