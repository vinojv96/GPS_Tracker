//***********success 08/11/2020 12:05AM
#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include <WiFi.h>
#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <BlynkSimpleEsp32.h>
#define OLED_SDA 4
#define OLED_SCL 15 
#define OLED_RST 16
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
float latitude , longitude;
String  lat_str , lng_str;
const char *ssid =  "Vino";     // Enter your WiFi Name
const char *pass =  "vinoth@1996"; // Enter your WiFi Password
char auth[] = "FpxjhiGZ-lS7Hk7eweAOdVzt6FmVfVWn"; 
WidgetMap myMap(V0); 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);
WiFiClient client;
TinyGPSPlus gps;
HardwareSerial SerialGPS(1);
void setup()
{
  Serial.begin(115200);
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");              // print ... till not connected
  }
  Serial.println("");
  Serial.println("WiFi connected");
   Wire.begin(OLED_SDA, OLED_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.println(F("hello"));
  delay(1000);
  SerialGPS.begin(9600, SERIAL_8N1, 12, 13);
  Blynk.begin(auth, ssid, pass);
  Blynk.virtualWrite(V0, "clr"); 
}
void loop()
{
  while (SerialGPS.available() > 0) {
    if (gps.encode(SerialGPS.read()))
    {
      if (gps.location.isValid())
      {
        latitude = gps.location.lat();
        lat_str = String(latitude , 6);
        longitude = gps.location.lng();
        lng_str = String(longitude , 6);
        Serial.print("Latitude = ");
        Serial.println(lat_str);
        Serial.print("Longitude = ");
        Serial.println(lng_str);
        display.clearDisplay();
        display.setCursor(0,0);
        display.println(F("Lat:"));
        display.println(lat_str);
        display.println(F("Lng:"));
        display.println(lng_str);
        Blynk.virtualWrite(V0, 1, latitude, longitude, "Location");
        display.display();
      }
     delay(1000);
     Serial.println();  
    }
  }  
  Blynk.run();
}
