#include "Arduino.h"
#include "ESP8266WiFi.h"
#include "ESP8266HTTPClient.h"
#include "Wire.h"
#include "SPI.h"
#include "SparkFunLIS3DH.h"

String deviceName = "CS244";
LIS3DH acc_sensor;

// WiFi settings
const char *ssid = "WIFI NAME";

void connectToWiFi()
{
    WiFi.begin(ssid, "PASSWORD");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
        Serial.print(WiFi.status());
    }
    Serial.println("");
    Serial.println("WiFi connected");
}

void setup()
{
    Serial.begin(9600);
    connectToWiFi();
    delay(1000); //relax...
    Serial.println("Start moving.\n");
    // Initialize sensor
    acc_sensor.begin();
}

void loop()
{
    if (WiFi.status() == WL_CONNECTED) {
        WiFiClient client;
        const int httpPort = 80;
                
        if (!client.connect("LOCAL IP", httpPort)) {
            Serial.println("connection failed");
            return;
        }

        String x = String(acc_sensor.readFloatAccelX());
        String y = String(acc_sensor.readFloatAccelY());
        String z = String(acc_sensor.readFloatAccelZ());
    
        String reading = "x=" + x + "&y=" + y + "&z=" +  z;
        String url = "GET /data.php?" + reading + " HTTP/1.0";
        client.println(url);
        client.println();
        client.stop(); //Stopping client
    } else {
        Serial.println("Error in WiFi connection");
    }
}

