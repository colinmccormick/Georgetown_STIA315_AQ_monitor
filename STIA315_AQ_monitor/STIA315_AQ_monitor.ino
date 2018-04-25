// Air quality monitor
// STIA 315: International Air Quality Lab
// Georgetown University, Spring 2018
// 
// This code is for a low-cost air-quality monitor built and tested in Spring 2018 by students
// in Georgetown University's STIA 315: International Air Quality Lab class.
//
// The device measures temperature, humidity, and PM2.5 concentrations and sends the data
// via WiFi to ThingSpeak.com, a cloud-based IoT platform. The measurement cycle time is 
// set by the variable sampleTime. The monitor can be paused and resumed by pressing the A and C
// buttons on the display. The monitor is battery-operated with solar charging.
//
// The monitor uses the following components:
//
// - Adafruit Feather HUZZAH microcontroller
// - Adafruit SSD1306 display
// - DHT11 temperature and humidity sensor
// - PPD42NS dust sensor for PM2.5 measurements
// - WiFi connection for data transfer
// - ThingSpeak.com for cloud-based data storage and visualization
// - Adafruit LiPo battery, USB/DC/Solar LiPo charger (v2), and 6V/2W solar panel
// - Large plastic enclosure
//
// Files for 3D printing the monitor case and associated items are available at:
// https://www.thingiverse.com/thing:2857819
//
// For additional information, see the README.

#include <SPI.h>
#include <DHT.h>
#include <DHT_U.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// WiFi settings
#define WIFI_SSID   "SSID HERE"
#define WIFI_PASS   "PASSWORD HERE"

// ThingSpeak settings
int channelID = 00000;    // set to your channel ID
String writeAPIKey = "YOUR API WRITE KEY HERE";  // set to your API key
String server = "api.thingspeak.com";
WiFiClient client;

// DHT-11 settings
#define DHTPIN 2
#define DHTTYPE DHT11
float humidity;
float temperature;
DHT dht(DHTPIN, DHTTYPE);

// PPD42NS settings
#define PPDPIN 16
unsigned long stopwatchDuration;
unsigned long totalStopwatch = 0;
unsigned long startTime;
unsigned long sampleTime =  15 * 60 * 1000;   // specified in milliseconds
float ratio = 0;
float concentration = 0;
float maxConcentration = 2000.0; // max value we'll accept from the PPD42NS sensor

// SSD1306 settings
#define BUTTON_A 0
#define BUTTON_C 2
Adafruit_SSD1306 display = Adafruit_SSD1306();
String s;

// Utility function to display text on screen
void display_text(String text_to_display) {
  display.clearDisplay();
  display.setCursor(0,0);
  display.print(text_to_display);
  display.display();
}

// Main setup function
void setup() {
  pinMode(DHTPIN, INPUT);
  pinMode(PPDPIN, INPUT);
  Serial.begin(115200);

  // Display setup
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setRotation(2);       // turn display upside down b/c of case design
  display_text("WiFi: " + String(WIFI_SSID) + " ... ");
  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);

  // WiFi setup
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) { // connect to wifi
    delay(1000);
  }
  display_text("Connected!\nStarting monitor...\nTTM: "+String(sampleTime/60/1000)+" mins");
  dht.begin();
  startTime = millis();
}

void loop() {
  // Check for button A push to pause; resume with button C
  if ( !digitalRead(BUTTON_A) ) {         
    display_text("PAUSED...\nPress C to resume.");
    while ( digitalRead(BUTTON_C) ) { delay(500); } // wait for button C
    display_text("Resuming...\nTTM: "+String(sampleTime/60/1000)+" mins");
    totalStopwatch = 0;     // reset after pause
    startTime = millis();   // reset after pause
  }

  // Monitor pulse length from PPD42NS
  stopwatchDuration = pulseIn(PPDPIN, LOW);   // duration of LOW pulse (microseconds)
  totalStopwatch = totalStopwatch + stopwatchDuration;
  if ((millis() - startTime) >= sampleTime) { // wait until we've hit cycle time
    // Calculate PM2.5 concentration
    ratio = totalStopwatch / (sampleTime * 10.0); // percentage (0 to 100)
    concentration = 1.1 * pow(ratio, 3) - 3.8 * pow(ratio, 2) + 520 * ratio + 0.62; // from manual
    if (concentration > maxConcentration) {   // clip concentration above maximum value
      concentration = maxConcentration;
    }
    // Read humidity and temperature
    humidity = dht.readHumidity();
    temperature = dht.readTemperature();
    
    // Print values on display
    s = "Air quality readings:\n";
    s = s + "PM: " + String(concentration) + " pcs/0.01cf\n";
    s = s + "T : " + String(temperature) + "C\n";
    s = s + "RH: " + String(humidity);
    display_text(s);

    // Connect to ThingSpeak server and send data from this measurement
    if (client.connect(server, 80)) { // connect to ThingSpeak server
      String body = "field1=" + String(temperature) + "&field2="
                    + String(humidity) + "&field3=" + String(concentration);
      client.print("POST /update HTTP/1.1\n");
      client.print("Host: api.thingspeak.com\n");
      client.print("Connection: close\n");
      client.print("X-THINGSPEAKAPIKEY: " + writeAPIKey + "\n");
      client.print("Content-Type: application/x-www-form-urlencoded\n");
      client.print("Content-Length: ");
      client.print(body.length());
      client.print("\n\n");
      client.print(body);
      client.print("\n\n");
      Serial.println("Sent data to ThingSpeak...");
    }
    client.stop();    // close connection to ThingSpeak server
    
    // Reset stopwatch and start time
    totalStopwatch = 0;
    startTime = millis();
  }
}

