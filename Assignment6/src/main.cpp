#include "Arduino.h"
#include "ESP8266WiFi.h"
#include "ESP8266HTTPClient.h"
#include "Wire.h"
#include "SPI.h"
#include "SparkFunLIS3DH.h"
#include "MAX30105.h"


String deviceName = "CS244";
LIS3DH acc_sensor(SPI_MODE, 4);
MAX30105 hr_sensor;

// WiFi settings
// const char *ssid = "NETGEAR89";
const char *ssid = "UCInet Mobile Access";

void connectToWiFi()
{
    WiFi.begin(ssid);
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
    // Wire.pins(0, 4);
    // Wire.begin();
    Serial.begin(9600);
    connectToWiFi();
    delay(1000); //relax...
    Serial.println("Start moving.\n");
    // Serial.println("Start sensing.\n");

    if (!hr_sensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed: begin(Wire, I2C_SPEED_FAST)
    {
        Serial.println("MAX30105 was not found. Please check wiring/power. ");
        while (1);
    }

    hr_sensor.setup();

    // Initialize sensor
    acc_sensor.begin();
}

void loop()
{
    // Serial.print(" R[");
    // Serial.print(hr_sensor.getRed());
    // Serial.print("] IR[");
    // Serial.print(hr_sensor.getIR());
    // Serial.print("] G[");
    // Serial.print(hr_sensor.getGreen());
    // Serial.print("]");
    // delay(1000);
    // Serial.println();
    String ir = String(hr_sensor.getIR());
    String red = String(hr_sensor.getRed());

    Serial.print("IR = " + ir);
    Serial.print(" R = " + red);

    String x = String(acc_sensor.readFloatAccelX());
    String y = String(acc_sensor.readFloatAccelY());
    String z = String(acc_sensor.readFloatAccelZ());

    Serial.print(" X = " + x);
    Serial.print(" Y = " + y);
    Serial.println(" Z = " + z);
    
    String reading = "ir=" + ir + "&red=" + red + "&x=" + x + "&y=" + y + "&z=" + z;
    
    if (WiFi.status() == WL_CONNECTED) {
        WiFiClient client;
        const int httpPort = 80;
                
        if (!client.connect("169.234.64.169", httpPort)) {
            Serial.println("Server connection failed");
            return;
        }

        String url = "GET /data.php?" + reading + " HTTP/1.0";
        client.println(url);
        client.println();
        client.stop(); //Stopping client
    } else {
        Serial.println("Error in WiFi connection");
    }
    
}
