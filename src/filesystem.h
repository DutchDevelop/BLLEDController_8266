#ifndef _FILESYSTEM
#define _FILESYSTEM

const char *configPath = "/blledconfig.json";

#include <Arduino.h>
#include <LittleFS.h>
#include "types.h"

bool loadFilesystem(){
    File configFile;
    int attempts = 0;
    while (attempts < 2) {
        configFile = LittleFS.open(configPath, "r");
        if (configFile) {
            break;
        }
        attempts++;
        Serial.println(F("Failed to open Filesystem, retrying.."));
        delay(3000);
    }

    if (!configFile) {
        Serial.println(F("Failed to open Filesystem after 2 retries"));
        return false;
    }

    size_t size = configFile.size();

    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);

    StaticJsonDocument<512> json;

    auto deserializeError = deserializeJson(json, buf.get());

    if (deserializeError) {
        Serial.println(F("Failed to load Filesystem"));
        return false;
    }

    strcpy(globalVariables.printerIP, json["printerIp"]);
    strcpy(globalVariables.accessCode, json["accessCode"]);
    strcpy(globalVariables.serialNumber, json["serialNumber"]);

    strcpy(globalVariables.SSID, json["ssid"]);
    strcpy(globalVariables.APPW, json["appw"]);

    Serial.println(F("Loaded Filesystem"));

    configFile.close();

    return true;
}

bool saveFilesystem(){
    Serial.println(F("Saving Filesystem"));

    StaticJsonDocument<512> json;
    json["printerIp"] = globalVariables.printerIP;
    json["accessCode"] = globalVariables.accessCode;
    json["serialNumber"] = globalVariables.serialNumber;

    json["ssid"] = globalVariables.SSID;
    json["appw"] = globalVariables.APPW;

    File configFile = LittleFS.open(configPath, "w");

    if (!configFile) {
        Serial.println(F("Failed to save Filesystem"));
        return false;
    }

    serializeJson(json, configFile);
    configFile.close();
    Serial.println(F("Filesystem Saved"));

    return true;
}

void setupFilesystem(){
    Serial.println(F("Mounting LittleFS"));
    if (!LittleFS.begin()) {
        Serial.println(F("Failed to mount LittleFS"));
        LittleFS.format();
        Serial.println(F("Formatting LittleFS"));
        ESP.restart();
    }
    Serial.println(F("Mounted LittleFS"));

    Serial.println(F("Loading Filesystem"));
    if (!loadFilesystem()){
        LittleFS.remove(configPath);
        saveFilesystem();
    }
}

#endif