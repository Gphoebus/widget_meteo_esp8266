/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 by Daniel Eichhorn
 * Copyright (c) 2016 by Fabrice Weinberg
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#include <DNSServer.h>
#include <ESP8266WebServer.h> 

// Use false if you don't like to display Available Pages in Information Page of Config Portal
// Comment out or use true to display Available Pages in Information Page of Config Portal
// Must be placed before #include <ESP_WiFiManager.h> 
#define USE_AVAILABLE_PAGES     true

#include <ESP_WiFiManager.h>              //https://github.com/khoih-prog/ESP_WiFiManager

#include <TimeLib.h>

#include "icons.h"

// Include the correct display library
// For a connection via I2C using Wire include
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"`
//#include "ssd1306_i2c.h"
//#include "Adafruit_SSD1306.h"
//#include "SH1106.h" //alias for `#include "SH1106Wire.h"`
// For a connection via I2C using brzo_i2c (must be installed) include
// #include <brzo_i2c.h> // Only needed for Arduino 1.6.5 and earlier
// #include "SSD1306Brzo.h"
// #include "SH1106Brzo.h"
// For a connection via SPI include
// #include <SPI.h> // Only needed for Arduino 1.6.5 and earlier
// #include "SSD1306Spi.h"
// #include "SH1106SPi.h"

// Include the UI lib
#include "OLEDDisplayUi.h"

// Include custom images
#include "images.h"

#include "WeatherClient.h"

// Use the corresponding display class:

// Initialize the OLED display using SPI
// D5 -> CLK
// D7 -> MOSI (DOUT)
// D0 -> RES
// D2 -> DC
// D8 -> CS
// SSD1306Spi        display(D0, D2, D8);
// or
// SH1106Spi         display(16, 4);

// Initialize the OLED display using brzo_i2c
// D3 -> SDA
// D5 -> SCL
// SSD1306Brzo display(0x3c, D3, D5);
// or
// SH1106Brzo  display(0x3c, D3, D5);

// Initialize the OLED display using Wire library
SSD1306  display(0x3c, 14, 12);
//SH1106 display(0x3c, 5, 4);

OLEDDisplayUi ui ( &display );

int screenW = 128;
int screenH = 64;
int clockCenterX = screenW/2;
int clockCenterY = ((screenH-16)/2)+16;   // top yellow part is 16 px height
float clockRadius = 23.0f;

String Temperature_inter = "0";
String Temperature_exter="0";

// SSID and PW for your Router
String Router_SSID;
String Router_Pass;

String ssid = "Module_meteo";

#ifndef WIFI_CONFIG_H
  #define ssid "phoebus_gaston"

  #define password "phoebus09"
#endif // !WIFI_CONFIG_H

#ifndef fuseau
  #define utcOffsetInSeconds  7200
#endif

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", utcOffsetInSeconds);


unsigned long previousMillis = 0; 

// constants won't change:
const long interval = 600000; 

// utility function for digital clock display: prints leading 0
String twoDigits(int digits){
  if(digits < 10) {
    String i = '0'+String(digits);
    return i;
  }
  else {
    return String(digits);
  }
}

WeatherClient weather;

// flag changed in the ticker function every 10 minutes
bool readyForWeatherUpdate = true;

void clockOverlay(OLEDDisplay *display, OLEDDisplayUiState *state) {

}

void analogClockFrame(OLEDDisplay *display, OLEDDisplayUiState *state,  int16_t x,  int16_t y) {
//  ui.disableIndicator();

  // Draw the clock face
//  display->drawCircle(clockCenterX + x, clockCenterY + y, clockRadius);
  display->drawCircle(clockCenterX + x, clockCenterY + y, 2);
  //
  //hour ticks
  for( float z=0; z < 360.0f;z= z + 30.0f ){
  //Begin at 0° and stop at 360°
    float angle = z ;
    angle = ( angle / 57.29577951f ) ; //Convert degrees to radians
    int x2 = ( clockCenterX + ( sin(angle) * clockRadius ) );
    int y2 = ( clockCenterY - ( cos(angle) * clockRadius ) );
    int x3 = ( clockCenterX + ( sin(angle) * ( clockRadius - ( clockRadius / 8 ) ) ) );
    int y3 = ( clockCenterY - ( cos(angle) * ( clockRadius - ( clockRadius / 8 ) ) ) );
    display->drawLine( x2 + x , y2 + y , x3 + x , y3 + y);
  }

  // display second hand
  float angle = second() * 6.0f ;
  angle = ( angle / 57.29577951 ) ; //Convert degrees to radians
  int x3 = ( clockCenterX + (sin(angle) * ( clockRadius - ( clockRadius / 5 ) ) ) );
  int y3 = ( clockCenterY - (cos(angle) * ( clockRadius - ( clockRadius / 5 ) ) ) );
  display->drawLine( clockCenterX + x , clockCenterY + y , x3 + x , y3 + y);
  //
  // display minute hand
  angle = minute() * 6 ;
  angle = ( angle / 57.29577951 ) ; //Convert degrees to radians
  x3 = ( clockCenterX + ( sin(angle) * ( clockRadius - ( clockRadius / 4 ) ) ) );
  y3 = ( clockCenterY - ( cos(angle) * ( clockRadius - ( clockRadius / 4 ) ) ) );
  display->drawLine( clockCenterX + x , clockCenterY + y , x3 + x , y3 + y);
  //
  // display hour hand
  angle = hour() * 30 + int( ( minute() / 12 ) * 6 )   ;
  angle = ( angle / 57.29577951 ) ; //Convert degrees to radians
  x3 = ( clockCenterX + ( sin(angle) * ( clockRadius - ( clockRadius / 2 ) ) ) );
  y3 = ( clockCenterY - ( cos(angle) * ( clockRadius - ( clockRadius / 2 ) ) ) );
  display->drawLine( clockCenterX + x , clockCenterY + y , x3 + x , y3 + y);
}

void digitalClockFrame(OLEDDisplay *display, OLEDDisplayUiState* state,  int16_t x,  int16_t y) {
  
  String timenow = String(hour())+":"+twoDigits(minute())+":"+twoDigits(second());
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_24);
  display->drawString(clockCenterX + x , clockCenterY + y-10, timenow );
}


void temperatures(OLEDDisplay *display, OLEDDisplayUiState *state,  int16_t x, int16_t y){
  
   //display->setFontScale2x2(true);
   display->setFont(ArialMT_Plain_16);
   display->drawString(60 + x, 16 + y, "T Inter");
  // display->drawXbm(x,y, 60, 60, xbmtemp);
 // display.setFontScale2x2(true)
   //display->setFontScale2x2(true);
   display->drawString(clockCenterX + x-10, clockCenterY + y, Temperature_inter + " °C");
   //display->setFontScale2x2(false);


}

void temperatures_int(OLEDDisplay *display, OLEDDisplayUiState *state,  int16_t x, int16_t y){
  
   //display->setFontScale2x2(true);
   display->setFont(ArialMT_Plain_16);
   display->drawString(60 + x, 16 + y, "T Exter");
  // display->drawXbm(x,y, 60, 60, xbmtemp);
 // display.setFontScale2x2(true)
   //display->setFontScale2x2(true);
   display->drawString(clockCenterX + x-10, clockCenterY + y, Temperature_exter + " °C");
   //display->setFontScale2x2(false);


}

// This array keeps function pointers to all frames
// frames are the single views that slide in
FrameCallback frames[] = { temperatures,temperatures_int, digitalClockFrame };

// how many frames are there?
int frameCount = 3;

// Overlays are statically drawn on top of a frame eg. a clock
OverlayCallback overlays[] = { clockOverlay };
int overlaysCount = 1;

void setup() {
  Serial.begin(115200);
  Serial.println("---");
  unsigned long startedAt = millis();
  // Initialising the UI will init the display too.
  ESP_WiFiManager ESP_wifiManager("ConfigOnStartup");
  ESP_wifiManager.setMinimumSignalQuality(-1);

  // Have to create a new function to store in EEPROM/SPIFFS for this purpose  
  Router_SSID = ESP_wifiManager.WiFi_SSID();
  Router_Pass = ESP_wifiManager.WiFi_Pass();  

  display.init();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 52, "Connection au wifi");

  display.drawXbm(38,3, 48, 50, phm_bits);

  display.display();

  if (Router_SSID != "")
  {
    ESP_wifiManager.setConfigPortalTimeout(60); //If no access point name has been previously entered disable timeout.
    Serial.println("Timeout 60s");
    display.clear();
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0, 52, "Attente de connection");
    display.drawXbm(38,3, 48, 50, phm_bits);
    display.display();
  }
  else
  {
    Serial.println("No timeout");
    display.clear();
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0, 52, "Configurer AP");
    display.drawXbm(38,3, 48, 50, phm_bits);
    display.display();
  }

  // SSID to uppercase 
  //ssid.toUpperCase();  

  //it starts an access point 
  //and goes into a blocking loop awaiting configuration
  Serial.println("temtative de demarrage ap"); 
  if (!ESP_wifiManager.startConfigPortal((const char *) ssid, password)) 
  {
    Serial.println("Not connected to WiFi but continuing anyway.");
    display.clear();
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0, 0, "Non connecté mais continu");
    display.display(); 
  }
  else 
  {
    Serial.println("WiFi connected...yeey :)");
    display.clear();
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0, 0, "Connecté");
    display.display();
  }

  
  // For some unknown reason webserver can only be started once per boot up 
  // so webserver can not be used again in the sketch.
  #define WIFI_CONNECT_TIMEOUT        30000L
  #define WHILE_LOOP_DELAY            200L
  #define WHILE_LOOP_STEPS            (WIFI_CONNECT_TIMEOUT / ( 3 * WHILE_LOOP_DELAY ))
  
  //WiFi.begin(ssid, password);

  startedAt = millis();
  
  while ( (WiFi.status() != WL_CONNECTED) && (millis() - startedAt < WIFI_CONNECT_TIMEOUT ) )
  {
 int i = 0;
    while((!WiFi.status() || WiFi.status() >= WL_DISCONNECTED) && i++ < WHILE_LOOP_STEPS)
    {
      delay(WHILE_LOOP_DELAY);
    }    

  }


     display.clear();
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0, 0, "Connecté");
    display.display();
    delay ( 500 );
       Serial.println ();
       Serial.println ("Connecté");

  timeClient.begin();

	// The ESP is capable of rendering 60fps in 80Mhz mode
	// but that won't give you much time for anything else
	// run it in 160Mhz mode or just set it to 30 fps
  ui.setTargetFPS(60);

	// Customize the active and inactive symbol
  ui.setActiveSymbol(activeSymbol);
  ui.setInactiveSymbol(inactiveSymbol);

  // You can change this to
  // TOP, LEFT, BOTTOM, RIGHT
  ui.setIndicatorPosition(TOP);

  // Defines where the first frame is located in the bar.
  ui.setIndicatorDirection(LEFT_RIGHT);

  // You can change the transition that is used
  // SLIDE_LEFT, SLIDE_RIGHT, SLIDE_UP, SLIDE_DOWN
  ui.setFrameAnimation(SLIDE_LEFT);

  // Add frames
  ui.setFrames(frames, frameCount);

  // Add overlays
  ui.setOverlays(overlays, overlaysCount);

  // Initialising the UI will init the display too.
  ui.init();

  //display.flipScreenVertically();

  unsigned long secsSinceStart = millis();
  // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
  const unsigned long seventyYears = 2208988800UL;
  // subtract seventy years:
  unsigned long epoch = secsSinceStart - seventyYears * SECS_PER_HOUR;
  //setTime(epoch);

    display.clear();
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0, 0, "Interrogation Horloge");
    display.display();
   timeClient.update();
   setTime(timeClient.getEpochTime());   
  //Serial.printf("Clock updated: %02d, %02d (%lu  %lu)\n", timeClient.getHours(), timeClient.getMinutes(),timeClient.getEpochTime());
    display.clear();
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0, 0, "Lecture meteo");
    display.display();
  weather.lecture();
  delay(1000);
  Temperature_inter = weather.getCurrentTemp_int();
  Temperature_exter = weather.getCurrentTemp_ext();

  Serial.println("Lecture init");
  Serial.print("Temp int ");
  Serial.println(Temperature_inter);

  Serial.print("Temp ext ");
   Serial.println(Temperature_exter);
}


void loop() {

  unsigned long currentMillis = millis();
  int remainingTimeBudget = ui.update();

  if (remainingTimeBudget > 0) {
    // You can do some work here
    // Don't do stuff if you are below your
    // time budget.
      if (currentMillis - previousMillis >= interval) {
        timeClient.update();
        previousMillis = currentMillis;
        weather.lecture();
        delay(1000);
        Temperature_inter =weather.getCurrentTemp_int();
        Temperature_exter = weather.getCurrentTemp_ext();
        Serial.println("Lecture Mise a jour");
        Serial.print("Temp int ");
        Serial.println(Temperature_inter);
        Serial.print("Temp ext ");
        Serial.println(Temperature_exter);

        Serial.print("loop ");
        Serial.println(remainingTimeBudget);
      }
    delay(remainingTimeBudget);

  }


}
