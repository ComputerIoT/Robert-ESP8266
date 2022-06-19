/**********
   Thubakgale Mabalane Samuel 
   
**********/
//::::::::: Libraries
//---- Wifi 
    #include <Arduino.h> 
    #include <ESP8266WiFi.h>
    #include <ESP8266WiFiMulti.h> 
    #include <ESP8266HTTPClient.h> 
//---- Json-Conversion 
    #include <Arduino_JSON.h>
//:::::::::: Variables     
//---- Wifi 
    ESP8266WiFiMulti WiFiMulti; 
//:::::::::: Set up
void setup() 
{  
     Serial.begin(9600);
  // Wifi Login Credentials
     WiFi.mode(WIFI_STA); 
     const char* ssid = "MAF006";
     const char* pass = "MAF006PrivateNetwork"; 
     WiFiMulti.addAP(ssid, pass);
  // ----------------------
}
//:::::::::: Loop 
void loop() 
{ 
   if(Serial.available())
   {
      String mss = Serial.readString(); 
      
      int a = mss.indexOf(",");
      int b = mss.indexOf(',', a+1 );
      int c = mss.indexOf(',', b+1 );
 
      String device = mss.substring(0, a);
      String voltage = mss.substring(a+1, b);  
      String current = mss.substring(b+1, c); 
      String power = mss.substring(c+1);
      String r = "";
 
      // Interpret Reading
         if(device == "grid")
         {
              r = "https://api.thingspeak.com/update?api_key=VPX9QS6K3EVX4843&field1=" + voltage +"&field2="+current+"&field3="+power;
         } 
         if(device == "roller")
         {
             r = "https://api.thingspeak.com/update?api_key=S4E9XWWLJDL9BEUL&field1="+ voltage +"&field2="+current+"&field3="+power;
         } 
 
      // wait for WiFi connection
         if ((WiFiMulti.run() == WL_CONNECTED) && r != "") 
         {  
            // https client  
               CRUD_Read(r);
            // 
         }
    } 
}

void CRUD_Read(String url)
{ 
       std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure); 
       client->setInsecure();  
       HTTPClient https; 
       
       if (https.begin(*client, url)) 
       {     
           int httpCode = https.GET();  
           if (httpCode > 0) 
           { 
              if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) 
              {
                 String payload = https.getString();
                 Serial.println(payload);
              }
              else
              { 
                 Serial.println(httpCode );
              }
            } 
            else 
            {
               Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
            }

            https.end();
        } 
        else 
        {
           Serial.printf("[HTTPS] Unable to connect\n");
        }
}
