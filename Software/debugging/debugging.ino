 
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/********************************************************************************/
//include images 
#include "motivation_quotes.h" 
/********************************************************************************/
//Things to change
//const char * ssid = "MiHotspot";
//const char * password = "1234567890";
const char * ssid = "XXXX";
const char * password = "XXXX";
String GOOGLE_SCRIPT_ID = "XXXXX-sTUp_XP-_T8nXWlmXGj-qvyDlhKx";

const int sendInterval = 500;
const int textdelay = 600000; //10 Minuten Delay
int txt_counter = 1200;  
/********************************************************************************/
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);




void testdrawstyles(String str) {
  // Scroll full screen
  display.clearDisplay();
  drawFrames();
  display.setCursor(0,10);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.println(str.c_str());
  display.display();


  
}

void drawFrames(){
  display.drawLine(0,0,display.width() - 1, 0, WHITE);
  display.drawLine(0,display.height()-1,display.width() - 1, display.height() - 1, WHITE);
 
}

void setup() {
  Serial.begin(115200);
  delay(10);

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }


Serial.println("Ready to go");

//testdrawstyles();
}

void loop() {
  for(int i=0; i<51; i++){
    Serial.print(i);
    Serial.print(": ");
    Serial.println(quote[i]);
    testdrawstyles(quote[i]);
    delay(5000);
  }
}
