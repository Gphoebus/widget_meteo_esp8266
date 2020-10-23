

/**The MIT License (MIT)

Copyright (c) 2015 by Daniel Eichhorn

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

See more at http://blog.squix.ch
*/

#include <Arduino.h>
#include <ArduinoJson.h>

class WeatherClient {
  private:
    const char* vitesse_vent;
    const char* pression;
    const char* temp_int;
    const char* temp_out;
    const char* dew_point;
    const char* rhi;
    const char* rho;
    const char* ws;
    const char* wsMax;
    const char* wsMoy;
    const char* dir_vent;
    const char* TiMax;    
    const char* Timin;
    const char* ToMax;    
    const char* Tomin;      

    String currentTemp;
    int currentHumidity;
    String currentIcon;
    String currentSummary;
    String iconToday;
    String maxTempToday;
    String minTempToday;
    String summaryToday;
    int maxTempTomorrow;
    int minTempTomorrow;
    String iconTomorrow;
    String summaryTomorrow;
    String apiKey;
    String myUnits = "auto";
    String myLanguage;
    
    String getValue(String line);
    String getKey(String line);
    String extract (String texte, String bal);
    String attribut (String texte,String lattribut, String bal);

    
  
  public:
  void lecture();
    void updateWeatherData(String apiKey, double lat, double lon); 
    void setUnits(String units);
    String getCurrentTemp_int(void);
    String getCurrentTemp_ext(void);    
    int getCurrentHumidity(void);
    String getCurrentIcon(void);
    String getCurrentSummary(void);
    String getIconToday(void);
    String getMaxTempToday(void);
    String getMinTempToday(void);
    String getSummaryToday(void);
    int getMaxTempTomorrow(void);
    int getMinTempTomorrow(void);
    String getIconTomorrow(void);
    String getSummaryTomorrow(void);
    String getvitessevent(void);
    String getpression(void);
    String getventmoy(void);
    String getventmax(void);
    String getToMax(void);
    String getToMin(void);
    String getTiMax(void);
    String getTiMin(void);
};
