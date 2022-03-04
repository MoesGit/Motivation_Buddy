/***************************************************************************************/
/*                                                   
                            WELCOME TO THE CODE 
                            
            - is this code perfect?            -> def. not 
            - does it work?                    -> kinda for what i use it for
            - can it be done way more efficent -> YES! 
            - am I to lazy to change it?       -> maybe 
         
            - Feel free to change it and contact me! 
            have fun 
*/          
/***************************************************************************************/




#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/********************************************************************************/

//include images and motivation quotes
#include "heart_map.h"
#include "paw_map.h"
#include "igel_map.h"
#include "wahl_map.h"
#include "motivation_quotes.h"
#include "settings.h"

/********************************************************************************/

// Constants to set Time intervalls
const int sendInterval = 500;
const int textdelay = 600000; // ~1h
int txt_counter = 1200;

/********************************************************************************/

// Constants and Defines for display and wifi
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
ESP8266WiFiMulti WiFiMulti;
std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);


void oled_info(int errorcode) {
  // 0 all good
  // 1 error

  if (errorcode == 1) {
    // ERROR
     Serial.println("In ERROR OLED INFO Function " );
    display.setCursor(8, 50); // set cursor to botom
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.println("[ *** NO WIFI *** ]");
    display.display();
  }


}

void print_image(int img_id) {
  display.clearDisplay(); //for Clearing the display
  switch (img_id) {
    case 1:
      display.drawBitmap(0, 0, heart_map, 128, 64, WHITE);
      break;
    case 2:
      display.drawBitmap(0, 0, paw_map, 128, 64, WHITE);
      break;
    case 3:
      display.drawBitmap(0, 0, wahl_map, 128, 64, WHITE);
      break;
    case 4:
      display.drawBitmap(0, 0, igel_map, 128, 64, WHITE);
      break;
    default:
      display.drawBitmap(0, 0, igel_map, 128, 64, WHITE);
      break; // Wird nicht benötigt, wenn Statement(s) vorhanden sind
  }


  display.display();
  delay(sendInterval);
}

void testdrawstyles(String str) {
  // Scroll full screen
  display.clearDisplay();
  drawFrames();
  display.setCursor(0, 10);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.println(str.c_str());
  display.display();

}

void drawFrames() {
  display.drawLine(0, 0, display.width() - 1, 0, WHITE);
  display.drawLine(0, display.height() - 1, display.width() - 1, display.height() - 1, WHITE);

}

void print_quote(){
    int rand_number = random(51);
    Serial.println(quote[rand_number]);
    testdrawstyles(quote[rand_number]);
}




void setup() {
  Serial.begin(115200);
  delay(10);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }


  WiFi.mode(WIFI_STA);
  //WiFi.begin(ssid, password);
  WiFiMulti.addAP(ssid, password);

  Serial.println("Started");
  Serial.print("Connecting");
  //while (WiFi.status() != WL_CONNECTED) {
  //  delay(500);
  // Serial.print(".");
  //}

  Serial.println("Ready to go");

  //testdrawstyles();
}

void loop() {

  if ((WiFiMulti.run() == WL_CONNECTED)) { //does not work? 
    update_display();
    delay(sendInterval);
  } else { //TODO Fix error msg
    display.clearDisplay();
    display.println("No Internet Connection");
  }

}

void update_display(void) {
  HTTPClient http;
  client->setInsecure();
  String url = "https://script.google.com/macros/s/" + GOOGLE_SCRIPT_ID + "/exec?read";
  Serial.print("Making a request ");
  Serial.println(url.c_str());
  http.begin(*client, url.c_str()); //Specify the URL and certificate
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  int httpCode = http.GET();
  String payload;
  Serial.println(httpCode);
  if (httpCode > 0 ) { //Check for the returning code 
    payload = http.getString();

    if (payload.length() > 200){ // Error in payload 
          Serial.println("Error in Payload");
          Serial.println("Automation mode");
          if (txt_counter < 1200) { //BUFFER
            txt_counter = txt_counter + 1;
          } else {
            print_quote();
            oled_info(1);
            txt_counter = 0;
          }
       
    }else{ //everything is fine DO CODE
 
      // Displaying the different things 
      if (payload.length() < 2) { //Print image
        if (payload.toInt() == 0) { //get random motivation quote
          if (txt_counter < 1200) {
            txt_counter = txt_counter + 1;
          } else {
            print_quote();
            txt_counter = 0;
          }
        } else { //Print image
          print_image(payload.toInt());
          txt_counter = 1200;
        }
      } else { //Print Text
        Serial.print("Http Code: ");
        Serial.println(httpCode);
        Serial.println(payload);
        testdrawstyles(payload);
        txt_counter = 1200;
      }
      Serial.print("Text Count: ");
      Serial.print(txt_counter);
      Serial.println(" | Quote changes at 1200");

    }

    
  } else { // No internet connection or error in request 
    Serial.println("Error on HTTP request");
    Serial.println("Automation mode");
    if (txt_counter < 1200) { //BUFFER
      txt_counter = txt_counter + 1;
    } else {
      print_quote();
      oled_info(1);
      txt_counter = 0;
    }
  }
  http.end();
}
