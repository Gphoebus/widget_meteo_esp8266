

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

#include "WeatherClient.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

void WeatherClient::lecture()
{


 if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;  //Object of class HTTPClient
    http.begin("http://flal09.free.fr/meteo/data/recup_json.php");
    int httpCode = http.GET();
    //Check the returning code                                                                  
    if (httpCode ==200) {
      // Parsing
      Serial.println("Code de retour 200");
    const size_t capacity = JSON_OBJECT_SIZE(18)+210;
    DynamicJsonDocument root(capacity);
    String lejson = http.getString();
    Serial.println (lejson);
    DeserializationError error = deserializeJson(root,lejson);
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      return;
    }
    else{
  
  
        delay(500);
        Serial.println("Deserialisation en cours");      
        temp_int = root["ti"].as<char*>();// temp inter
        temp_out = root["tout"].as<char*>();// temp exter
        dew_point = root["DP"].as<char*>();// point de rosée
        rhi = root["RHi"].as<char*>();// humidité inter 
        rho = root["RHo"].as<char*>();// humidité exter
        ws = root["WS"].as<char*>(); // vitesse du vent
        dir_vent = root["DIRtext"].as<char*>();// Direction du vent
        pression = root["RP"].as<char*>();// pression atmospherique
        wsMax = root["WSMax"].as<char*>(); // vitesse du vent Maxi
        wsMoy = root["WSAvg"].as<char*>(); // vitesse du vent Moyenne
        TiMax = root["timax"].as<char*>(); // Temp inter Maxi
        Timin = root["timin"].as<char*>(); // Temp inter Maxi
        ToMax = root["toutmax"].as<char*>(); // Temp inter Maxi
        Tomin = root["toutmin"].as<char*>(); // Temp inter Maxi        
       
        Serial.println("Aprés lecture");      
        Serial.print("len Temp int ");
        Serial.println(temp_int);
  
        Serial.print("Temp ext ");
        Serial.println(temp_out);
  
        Serial.print("vent ");
        Serial.println(ws);
  /*
        Serial.print("Pression atmospherique ");
        Serial.println(pression);
        */
        }
    }
    else
    {
      
        Serial.println("Erreur de lecture json");
      
      }
    }
    
  
}
  



/*
 * extrait la balise xml
 * texte = le texte dans lequel est la balise
 * bal = la balise a extraire
 */
String WeatherClient::extract (String texte, String bal)
{
    String balise_debut = "<"+bal;
   String retour = "false";
    if (texte.indexOf(balise_debut)!=-1){
      int debut = texte.indexOf(balise_debut);
    
      String balise_fin = "/>";
  
      int fin = texte.indexOf(balise_fin);
    retour = texte.substring(debut+(balise_debut.length()),fin);
  }else
  {
    retour = "false";
  
  }
   return retour;
  }
  


/*
 * Retourne l attribur
 * Texte est le texte complet
 * lattribut est celui a recherche
 * bla dans quel balise (noeud)
 */
String WeatherClient::attribut (String texte,String lattribut, String bal)
{
String labalise = extract(texte,bal);
String retour = "";
if (labalise!="false"){
  Serial.print("Balise = ");
  Serial.println(labalise);
  int debut_attribut = labalise.indexOf(lattribut+"=");
  int fin_attribut = labalise.indexOf((char)34,debut_attribut+(lattribut.length())+2);
  if (debut_attribut==0)
  {
    retour="";
  }
  else
  {
   retour = labalise.substring(debut_attribut+(lattribut.length())+2,fin_attribut); 
  }
}
return retour;
}

void WeatherClient::setUnits(String units) {
   myUnits = units; 
}

String WeatherClient::getKey(String line) {
  int separatorPosition = line.indexOf("=");
  if (separatorPosition == -1) {
    return "";
  }  
  return line.substring(0, separatorPosition);
}

String WeatherClient::getValue(String line) {
  int separatorPosition = line.indexOf("=");
  if (separatorPosition == -1) {
    return "";
  }  
  return line.substring(separatorPosition + 1);
}


String WeatherClient::getCurrentTemp_int(void) {
  //Serial.println(temp_int);
      Serial.print("Temp int get ");
      Serial.println(temp_int);
  return String(temp_int);
}
String WeatherClient::getCurrentTemp_ext(void) {
  //Serial.println(temp_int);
      Serial.print("Temp out get ");
      Serial.println(temp_out);
  return String(temp_out);
}

int WeatherClient::getCurrentHumidity(void) {
  return currentHumidity;
}
String WeatherClient::getCurrentIcon(void) {
  return currentIcon;
}
String WeatherClient::getCurrentSummary(void) {
  return currentSummary;
}
String WeatherClient::getIconToday(void) {
  return iconToday;
}
String WeatherClient::getMaxTempToday(void) {
  return maxTempToday;
}
String WeatherClient::getMinTempToday(void) {
  return minTempToday;
}
String WeatherClient::getSummaryToday(void) {
  return summaryToday;
}
int WeatherClient::getMaxTempTomorrow(void) {
  return maxTempTomorrow;
}
int WeatherClient::getMinTempTomorrow(void) {
  return minTempTomorrow;
}
String WeatherClient::getIconTomorrow(void) {
  return iconTomorrow;
}
String WeatherClient::getSummaryTomorrow(void) {
  return summaryTomorrow;
}
String WeatherClient::getvitessevent(void){
  Serial.print("vent ");
  Serial.println(ws);
  return String (ws);
}
String WeatherClient::getpression(void){
  return pression;

}
String WeatherClient::getventmax(void){
  return String (wsMax);
}
String WeatherClient::getventmoy(void){
  return String (wsMoy);
}
String WeatherClient::getToMax(void){
  return String (ToMax);
}
String WeatherClient::getToMin(void){
  return String (Tomin);
}
String WeatherClient::getTiMax(void){
  return String (TiMax);
}
String WeatherClient::getTiMin(void){
  return String (Timin);
}
