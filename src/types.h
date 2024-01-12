#ifndef _TYPES
#define _TYPES

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct PrinterVaraiblesStruct{
        String parsedHMS = "";
        String gcodeState = "IDLE";
        int stage = 0;
        bool ledstate = true;
        bool hmsstate = false;
        bool online = false;
        bool finished = false;
        unsigned long finishstartms;

    } PrinterVariables;

    PrinterVariables printerVariables;

    typedef struct GlobalVariablesStruct{
        char SSID[32];
        char APPW[32];
        char printerIP[16];
        char accessCode[9];
        char serialNumber[16];
    } GlobalVariables;

    GlobalVariables globalVariables;

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif