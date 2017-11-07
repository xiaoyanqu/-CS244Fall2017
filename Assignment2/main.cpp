#include "Arduino.h"
#include "ESP8266WiFi.h"
#include "ESP8266HTTPClient.h"
#include <Wire.h>
#include "MAX30105.h"


String deviceName = "CS244";
MAX30105 ir_red_sensor;

// WiFi settings
const char *ssid = "SSID HERE";

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

    WiFi.begin(ssid, "PASSWORD HERE");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
        Serial.print(WiFi.status());
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
    Serial.begin(115200);
    Serial.println("Program started");
    printMacAddress();

    // Initialize sensor
    if (!ir_red_sensor.begin(Wire, I2C_SPEED_FAST)) {
        Serial.println("Senor not found");
        while(true);
    }

    ir_red_sensor.setup();
    
    //0x02 = 0.4mA, 0x1F = 6.4mA, 0x7F = 25.4mA, 0xFF = 50mA
    ir_red_sensor.setPulseAmplitudeRed(0xFF);
    ir_red_sensor.setPulseAmplitudeIR(0xFF);
    ir_red_sensor.setSampleRate(50);
}

void loop()
{
    // You can write your logic here
    // Below are the LED samples from the hands-on
    if (WiFi.status() == WL_CONNECTED) {
        WiFiClient client;
        const int httpPort = 80;
        if (!client.connect("LOCAL IP HERE", httpPort)) {
            Serial.println("connection failed");
            return;
        }
        
        long red = ir_red_sensor.getRed();
        long ir = ir_red_sensor.getIR();

        String url = "GET /data.php?red=" + String(red) + "&ir=" + String(ir) + " HTTP/1.0";
        client.println(url);
        client.println();
        client.stop(); //Stopping client

    } else {
        Serial.println("Error in WiFi connection");
    }
}