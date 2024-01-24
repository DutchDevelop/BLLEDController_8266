#ifndef _WIFIMANAGER
#define _WIFIMANAGER

#include <Arduino.h>
#include <ArduinoJson.h> 
#include <ESP8266WiFi.h>

#include "types.h"

int connectionAttempts = 0;
const int maxConnectionAttempts = 10;

bool setupWifi(){
    Serial.println(F("-------------------------------------"));

    if (strlen(globalVariables.SSID) == 0 || strlen(globalVariables.APPW) == 0) {
        Serial.println(F("SSID or password is missing. Please configure both by going to: https://dutchdevelop.com/blled-configuration-setup/"));
        return false;
    }

    while (connectionAttempts < maxConnectionAttempts) {
        if (WiFi.status() == WL_CONNECTED)
            break;
        
        WiFi.begin(globalVariables.SSID, globalVariables.APPW);
        delay(1000);
        
        Serial.print(F("Connecting to WIFI.. "));
        Serial.println(globalVariables.SSID);
        Serial.println(globalVariables.APPW);
        delay(8000);
        connectionAttempts++;
    }

    int signalStrength = WiFi.RSSI();
    Serial.println(F("Connected To Wifi:"));

    if (WiFi.status() != WL_CONNECTED){
        Serial.println(F("Failed to connect to wifi."));
        return false;
    }
    
    return true;
}

#endif