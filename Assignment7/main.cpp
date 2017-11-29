#include "Arduino.h"
#include "ArduinoJson.h"
#include "ESP8266WiFi.h"
#include "ESP8266HTTPClient.h"
#include "Wire.h"
#include "SPI.h"
#include "SparkFunLIS3DH.h"
#include "MAX30105.h"

String deviceName = "CS244";
LIS3DH acc_sensor(SPI_MODE, 4);
MAX30105 ppg_sensor;

// WiFi settings
const char *ssid = "UCInet Mobile Access";

void connectToWiFi()
{
    WiFi.begin(ssid);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
}

void setup()
{
    Serial.begin(9600);
    connectToWiFi();

    //Use default I2C port, 400kHz speed: begin(Wire, I2C_SPEED_FAST)
    if (!ppg_sensor.begin(Wire, I2C_SPEED_FAST))
    {
        Serial.println("MAX30105 was not found.");
        while (true);
    }
    ppg_sensor.setup();

    acc_sensor.begin();
    
    Serial.println("Start moving.\n");
}

void loop() {
    
    // Post every 0.5 second
    const int BATCH_SIZE = 50;
    
    //How to determine buffer size: https://goo.gl/s14vUA
    const int BUFFER_SIZE = JSON_OBJECT_SIZE(6) + 6 * JSON_ARRAY_SIZE(BATCH_SIZE);
    //StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;
    DynamicJsonBuffer jsonBuffer(4000);
    JsonObject& root = jsonBuffer.createObject();
    JsonArray& Time = root.createNestedArray("Time");
    JsonArray& Red = root.createNestedArray("Red");
    JsonArray& IR = root.createNestedArray("IR");
    JsonArray& X = root.createNestedArray("X");
    JsonArray& Y = root.createNestedArray("Y");
    JsonArray& Z = root.createNestedArray("Z");
    
    for (int i = 0; i < BATCH_SIZE; i++) {
        Time.add(millis());
        Red.add(ppg_sensor.getRed());
        IR.add(ppg_sensor.getIR());
        X.add(acc_sensor.readFloatAccelX());
        Y.add(acc_sensor.readFloatAccelY());
        Z.add(acc_sensor.readFloatAccelZ());
    }

    // Serialize the object to a JSON string
    String jsonStr;
    root.printTo(jsonStr);
    //Serial.println(jsonStr);

    HTTPClient http;
    http.begin("http://169.234.217.123/data.php");
    http.addHeader("Content-Type", "application/json");

    // Post request, receive http response and print
    int httpCode = http.POST(jsonStr);
    String payload = http.getString();
    Serial.println(httpCode);
    Serial.println(payload);

    // End session
    http.end();
}