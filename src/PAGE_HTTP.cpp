#include <Wire.h>
#include <HTTPClient.h>

void PAGE_WEB(String serverName){
    
    unsigned long lastTime = 0;

    HTTPClient http;

    String serverPath = serverName;  
    http.begin(serverPath.c_str());
  
    int httpResponseCode = http.GET();
        
    http.end();
    lastTime = millis();

    delay(1000);
}