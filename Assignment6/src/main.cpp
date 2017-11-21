#include "Arduino.h"
#include "ESP8266WiFi.h"
#include "ESP8266HTTPClient.h"
#include "Wire.h"
#include "SPI.h"
#include "SparkFunLIS3DH.h"
#include "MAX30105.h"
// #include "heartRate.h"



String deviceName = "CS244";
LIS3DH acc_sensor(SPI_MODE, 4);
MAX30105 particleSensor;

const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred

float beatsPerMinute;
int beatAvg;

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

    if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed: begin(Wire, I2C_SPEED_FAST)
    {
        Serial.println("MAX30105 was not found. Please check wiring/power. ");
        while (1);
    }

    particleSensor.setup();
    // particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
    // particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED

    // Initialize sensor
    acc_sensor.begin();
}

void loop()
{
    String ir = String(particleSensor.getIR());
    String red = String(particleSensor.getRed());

    Serial.print("IR = " + ir);
    Serial.print(" R = " + red);

    String x = String(acc_sensor.readFloatAccelX());
    String y = String(acc_sensor.readFloatAccelY());
    String z = String(acc_sensor.readFloatAccelZ());

    Serial.print(" X = " + x);
    Serial.print(" Y = " + y);
    Serial.println(" Z = " + z);

    // if (checkForBeat(irValue) == true)
    // {
    //     //We sensed a beat!
    //     long delta = millis() - lastBeat;
    //     lastBeat = millis();

    //     beatsPerMinute = 60 / (delta / 1000.0);

    //     if (beatsPerMinute < 255 && beatsPerMinute > 20)
    //     {
    //         rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
    //         rateSpot %= RATE_SIZE; //Wrap variable

    //         //Take average of readings
    //         beatAvg = 0;
    //         for (byte x = 0 ; x < RATE_SIZE ; x++)
    //         beatAvg += rates[x];
    //         beatAvg /= RATE_SIZE;
    //     }
    // }

    // Serial.print(", BPM=");
    // Serial.print(beatsPerMinute);
    // Serial.print(", Avg BPM=");
    // Serial.print(beatAvg);

    // if (irValue < 50000)
    // Serial.print(" No finger?");

    // Serial.println();
    
    String reading = "&ir=" + ir + "&red=" + red + "&x=" + x + "&y=" + y + "&z=" + z;
    
    if (WiFi.status() == WL_CONNECTED) {
        WiFiClient client;
        const int httpPort = 80;
                
        if (!client.connect("169.234.51.114", httpPort)) {
            Serial.println("Server connection failed");
            return;
        }
        String curTime = String(millis());
        Serial.println("Time : " + curTime);
        reading = "time=" + curTime + reading;
        String url = "GET /data.php?" + reading + " HTTP/1.0";
        client.println(url);
        client.println();
        client.stop(); //Stopping client
    } else {
        Serial.println("Error in WiFi connection");
    }
    
}
