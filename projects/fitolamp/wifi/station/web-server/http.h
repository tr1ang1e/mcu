#ifndef HTTP_H
#define HTTP_H


/*
    Callbacks for HTTP responces.
    Located here for main source
    file readability purposes.
*/


/* --------------------------------------------------------- */
/*                       I N C L U D E                       */
/* --------------------------------------------------------- */

#include <ESP8266WebServer.h>
#include "pins_arduino.h"


/* --------------------------------------------------------- */
/*                   H E L P   M A C R O S                   */
/* --------------------------------------------------------- */

#define WEB_TEXT      "text/plain"
#define PIN_FITOLAMP  D1 

// all URIs
#define URI_ROOT              "/"
#define URI_STAT              "/stat"
#define URI_LAMP              "/lamp"

// URI_LAMP args
#define URI_LAMP_STATE        "state"


/* --------------------------------------------------------- */
/*                   S T A T I C   D A T A                   */
/* --------------------------------------------------------- */

extern ESP8266WebServer webServer;
extern bool isFitolampOn;

/* --------------------------------------------------------- */
/*                         T Y P E S                         */
/* --------------------------------------------------------- */

typedef enum WEB_RET_CODE
{
    WEB_SUCCESS = 200,
    WEB_BAD_REQUEST = 400,
    WEB_NOT_FOUND = 404
    // ...
    // ...
} WEB_RET_CODE;


/* --------------------------------------------------------- */
/*             P U B L I C   F U N C T I O N S               */
/* --------------------------------------------------------- */

void uri_not_found()
{
    webServer.send(WEB_NOT_FOUND, WEB_TEXT, "404: not found");
}

void uri_root()  // URI_ROOT
{
    webServer.send(WEB_SUCCESS, WEB_TEXT, "root");
}

void uri_stat()  // URI_STAT
{
    char stat[1024] = { 0 };
    size_t offset = 0;

    // TODO:
    // from serial port information

    const char* fitolamp_header = "Fitolamp: ";
    memcpy(stat + offset, fitolamp_header, strlen(fitolamp_header));
    offset += strlen(fitolamp_header);

    const char* fitolamp_state = isFitolampOn ? "on\n" : "off\n";
    memcpy(stat + offset, fitolamp_state, strlen(fitolamp_state));
    offset += strlen(fitolamp_state);

    webServer.send(WEB_SUCCESS, WEB_TEXT, stat);
}

void uri_lamp()  // URI_LAMP
{
    bool hasState = webServer.hasArg(URI_LAMP_STATE);
    
    /* passed arguments validation */
    
    if (!hasState)
    {
        webServer.send(WEB_BAD_REQUEST, WEB_TEXT, "400: use 'state=<state>' to manage fitolamp");
        return;
    }
    
    /* arguments handling */

    if (hasState)
    {
        if (webServer.arg(URI_LAMP_STATE) == "on")
        {
            if (!isFitolampOn)
            {
                digitalWrite(PIN_FITOLAMP, HIGH);
                isFitolampOn = true;  
            }          
        }
        else if (webServer.arg(URI_LAMP_STATE) == "off")
        {
            if (isFitolampOn)
            {
                digitalWrite(PIN_FITOLAMP, LOW);
                isFitolampOn = false;
            }         
        }
        else 
        {
            webServer.send(WEB_SUCCESS, WEB_TEXT, "400: use either 'on' or 'off' value");
            return;
        }

        webServer.send(WEB_SUCCESS, WEB_TEXT, "200: success");
    }
}


#endif // HTTP_H