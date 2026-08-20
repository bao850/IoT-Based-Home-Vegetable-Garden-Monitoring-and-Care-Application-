#include <WiFi.h>
#include "wifi_module.h"

void initWiFi(const char* ssid, const char* pass){
    WiFi.begin(ssid, pass);
    while(WiFi.status()!=WL_CONNECTED) delay(300);
}

void ensureWiFi(){
    if(WiFi.status()==WL_CONNECTED) return;
    WiFi.disconnect();
    WiFi.begin();
}
