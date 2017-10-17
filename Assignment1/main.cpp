#include "Arduino.h"
#include "ESP8266WiFi.h"
#include "ESP8266HTTPClient.h"

String deviceName = "CS244";

// WiFi settings
const char *ssid = "Network Name Here";

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
        } else {
            Serial.println("connection success");
        }
      
        String data = "heartRate=100"; 

        // We now create a URI for the request
        String url = "GET /data.php?" + data + " HTTP/1.0";
        client.println(url);
        client.println();
        client.stop(); //Stopping client

    } else {
        Serial.println("Error in WiFi connection");
    }

    // turn the LED on (HIGH is the voltage level)
    digitalWrite(LED_BUILTIN, HIGH);
    // wait for a second
    delay(1000);
    // turn the LED off by making the voltage LOW
    digitalWrite(LED_BUILTIN, LOW);
    // wait for a second
    delay(1000);

    delay(10000);
}