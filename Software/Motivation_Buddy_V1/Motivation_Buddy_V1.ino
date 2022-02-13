#include <WiFi.h>
#include <HTTPClient.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/********************************************************************************/
//Things to change
const char * ssid = "XXXXX";
const char * password = "XXXXX";
String GOOGLE_SCRIPT_ID = "XXXXXXX-C9-bLxbjNEBNpYrKnKlxk2lorXW9MG2KteE6T1snD1xPL6ihRqamd-LKqh28-";


const int sendInterval = 5000; 
/********************************************************************************/
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16


WiFiClientSecure client;

/* Original
void testdrawstyles(String str) {
  display.clearDisplay();
  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F(str.c_str()));
  display.display();
  delay(2000);
}
*/






void testdrawstyles(String str) {
  // Scroll full screen
  display.clearDisplay();
  drawFrames();
  display.setCursor(0,10);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.println(F(str.c_str()));
  display.display();
  //display.startscrollleft(0x00, 0x0F);
  //delay(5000);
  //display.stopscroll();

  
}

void drawFrames(){

  //display.drawLine(0,0,display.width() - 1, display.height() - 1, WHITE);
  display.drawLine(0,0,display.width() - 1, 0, WHITE);
  //display.drawLine(0,0,0,display.height() - 1, WHITE);
  //display.drawLine(display.width() - 1,0,display.width() - 1, display.height() - 1, WHITE);
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


  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.println("Started");
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
Serial.println("Ready to go");
//testdrawstyles();
}

void loop() {
  spreadsheet_comm();
  delay(sendInterval);
}

void spreadsheet_comm(void) {
   HTTPClient http;
   String url="https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?read";
  Serial.print("Making a request");
  http.begin(url.c_str()); //Specify the URL and certificate
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  int httpCode = http.GET();
  String payload;
    if (httpCode > 0) { //Check for the returning code
        payload = http.getString();
        Serial.println(httpCode);
        Serial.println(payload);
        testdrawstyles(payload);
      }
    else {
      Serial.println("Error on HTTP request");
    }
  http.end();
}
