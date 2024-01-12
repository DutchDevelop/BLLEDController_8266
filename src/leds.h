#ifndef _LED
#define _LED

#include <Arduino.h>

const int redPin = 5;
const int greenPin = 14;
const int bluePin = 4;
const int warmPin = 2;
const int coldPin = 0;

int currentRed = 0;
int currentGreen = 0;
int currentBlue = 0;
int currentWarm = 0;
int currentCold = 0;

void tweenToColor(int targetRed, int targetGreen, int targetBlue, int targetWarm, int targetCold, int duration) {

    float brightness = (float)printerConfig.brightness/100.0;

    int brightenedRed = round(targetRed * brightness);
    int brightenedGreen = round(targetGreen * brightness);
    int brightenedBlue = round(targetBlue * brightness);
    int brightenedWarm = round(targetWarm * brightness);
    int brightenedCold = round(targetCold * brightness);

    if (brightenedRed == currentRed && brightenedGreen == currentGreen && brightenedBlue == currentBlue && brightenedWarm == currentWarm && brightenedCold == currentCold){
        if (printerConfig.debuging){
            Serial.println(F("LEDS Trying to change to the same color."));
        };
        return; // already at that color
    }
    float stepTime = (float)duration / 255.0;
    int redStep = (brightenedRed - currentRed) / 255;
    int greenStep = (brightenedGreen - currentGreen) / 255;
    int blueStep = (brightenedBlue - currentBlue) / 255;
    int warmStep = (brightenedWarm - currentWarm) / 255;
    int coldStep = (brightenedCold - currentCold) / 255;

     for (int i = 0; i < 256; i++) {
        currentRed += redStep;
        currentGreen += greenStep;
        currentBlue += blueStep;
        currentWarm += warmStep;
        currentCold += coldStep;

        analogWrite(redPin, currentRed);
        analogWrite(greenPin, currentGreen);
        analogWrite(bluePin, currentBlue);
        analogWrite(warmPin, currentWarm);
        analogWrite(coldPin, currentCold);
        delay(stepTime);
    }

    currentRed = brightenedRed;
    currentGreen = brightenedGreen;
    currentBlue = brightenedBlue;
    currentWarm = brightenedWarm;
    currentCold = brightenedCold;

    analogWrite(redPin, currentRed);
    analogWrite(greenPin, currentGreen);
    analogWrite(bluePin, currentBlue);
    analogWrite(warmPin, currentWarm);
    analogWrite(coldPin, currentCold);
}

void updateleds(){
    Serial.println(F("Updating leds"));

    Serial.println(printerVariables.stage);
    Serial.println(printerVariables.gcodeState);
    Serial.println(printerVariables.ledstate);
    Serial.println(printerVariables.hmsstate);
    Serial.println(printerVariables.parsedHMS);
    //OFF

    if (printerConfig.turbo == true){
        return;
    }

    if (printerVariables.online == false){ //printer offline
        tweenToColor(0,0,0,0,0,500); //OFF
        Serial.println(F("Printer offline, Turning Leds off"));
        return;
    }

    if (printerVariables.ledstate == false){ // replicate printer behaviour
        tweenToColor(0,0,0,0,0,500); //OFF
        Serial.println(F("Chamber light off, Turning Leds off"));
        return;
    }

     if (printerVariables.stage == 14){ //Cleaning nozzle
        tweenToColor(0,0,0,0,0,500); //OFF
        Serial.println(F("Cleaning nozzle, Turning Leds off"));
        return;
    }

    if (printerVariables.stage == 9){ //Scaning surface
        tweenToColor(0,0,0,0,0,500); //OFF
        Serial.println(F("Scanning Surface, Turning Leds off"));
        return;
    }
    
     //RED

    if (printerVariables.parsedHMS == "Serious"){
        
        Serial.println(F("Serious problem, Turning Leds red"));
        return;
    }

    if (printerVariables.parsedHMS == "Fatal"){
        tweenToColor(255,0,0,0,0,500); //RED
        Serial.println(F("Fatal problem, Turning Leds red"));
        return;
    }

    if (printerVariables.stage == 6){ //Fillament runout
        tweenToColor(255,0,0,0,0,500); //RED
        Serial.println(F("Fillament runout, Turning Leds red"));
        return;
    }

    if (printerVariables.stage == 17){ //Front Cover Removed
        tweenToColor(255,0,0,0,0,500); //RED
        Serial.println(F("Front Cover Removed, Turning Leds red"));
        return;
    }

    if (printerVariables.stage == 20){ //Nozzle Temp fail
        tweenToColor(255,0,0,0,0,500); //RED
        Serial.println(F("Nozzle Temp fail, Turning Leds red"));
        return;
    }

    if (printerVariables.stage == 21){ //Bed Temp Fail
        tweenToColor(255,0,0,0,0,500); //RED
        Serial.println(F("Bed Temp fail, Turning Leds red"));
        return;
    }

    //GREEN

    if ((millis() - printerVariables.finishstartms) <= 300000 && printerVariables.gcodeState == "FINISH"){
        tweenToColor(0,255,0,0,0,500); //ON
        if (printerConfig.debuging){
            Serial.println(F("Finished print, Turning Leds green"));
            Serial.println(F("Leds should stay on for: "));
            Serial.print((millis() - printerVariables.finishstartms));
            Serial.print(F(" MS"));
        };
        return;
    }

    //ON

    if (printerVariables.stage == 0){ //Printing
        tweenToColor(0,0,0,255,255,500); //ON
        Serial.println(F("Printing, Turning Leds On"));
        return;
    }

    if (printerVariables.stage == -1){ // Idle
        tweenToColor(0,0,0,255,255,500); //ON
        Serial.println(F("Idle, Turning Leds On"));
        return;
    }

    if (printerVariables.stage == -2){ //Offline
        tweenToColor(0,0,0,255,255,500); //ON
        Serial.println(F("Stage -2, Turning Leds On"));
        return;
    }
}

void setupLeds() {
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
    pinMode(coldPin, OUTPUT);
    pinMode(warmPin, OUTPUT);
    updateleds();
}

void ledsloop(){
    RGBCycle();
    if((millis() - printerVariables.finishstartms) >= 300000 && printerVariables.gcodeState == "FINISH"){
        printerVariables.gcodeState == "IDLE";
        updateleds();
    }
}

#endif