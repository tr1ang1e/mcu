#include "pins_arduino.h"
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


/* --------------------------------------------------------- */
/*                   H E L P   M A C R O S                   */
/* --------------------------------------------------------- */

#define WEB_TEXT  "text/plain"

// all URIs
#define URI_ROOT              "/"
#define URI_LAMP              "/lamp"

// URI_LAMP args
#define URI_LAMP_INFO         "info"
#define URI_LAMP_STATE        "state"


/* --------------------------------------------------------- */
/*                   S T A T I C   D A T A                   */
/* --------------------------------------------------------- */

extern ESP8266WebServer webServer;
extern bool isLampOn;

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

void uri_lamp()  // URI_LAMP
{
    bool hasInfo = webServer.hasArg(URI_LAMP_INFO);
    bool hasState = webServer.hasArg(URI_LAMP_STATE);
    
    /* passed arguments validation */
    
    if (!hasInfo && !hasState)
    {
        webServer.send(WEB_BAD_REQUEST, WEB_TEXT, "400: use 'state=<state>' or 'info' arguments");
        return;
    }
    else if (hasState && hasInfo) 
    {
        webServer.send(WEB_BAD_REQUEST, WEB_TEXT, "400: passed arguments cannot be used simultaneously");
        return;
    }
    
    /* arguments handling */

    if (hasInfo)
    {
        const char* info = isLampOn ? "lamp state: ON" : "lamp state: OFF";
        webServer.send(WEB_SUCCESS, WEB_TEXT, info);
        return;
    }

    if (hasState)
    {
        if (webServer.arg(URI_LAMP_STATE) == "on")
        {
            if (!isLampOn)
            {
                digitalWrite(LED_BUILTIN, LOW);
                isLampOn = true;
            }          
        }
        else if (webServer.arg(URI_LAMP_STATE) == "off")
        {
            if (isLampOn)
            {
                digitalWrite(LED_BUILTIN, HIGH);
                isLampOn = false;
            }         
        }

        webServer.send(WEB_SUCCESS, WEB_TEXT, "200: success");
    }
    
}


#endif // HTTP_H