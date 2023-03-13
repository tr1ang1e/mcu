/*
    Use ESP8266 in a station mode  >>  use as a WEB-server
    Available URI's:
        <esp>/                          = root URI
        <esp>/stat                      = current chip statistics
        <esp>/lamp?state=<state>        = <state> values: on, off 
*/

#define SKETCH "station, web-server: fitolamp"


/* --------------------------------------------------------- */
/*                       I N C L U D E                       */
/* --------------------------------------------------------- */

#include <Arduino.h>
#include <Esp.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>

#include "http.h"


/* --------------------------------------------------------- */
/*                   H E L P   M A C R O S                   */
/* --------------------------------------------------------- */

// ADC_MODE(ADC_VCC)   // ESP.getVcc();
#define __PINFO(_1, _2)  Serial.print(_1); Serial.println(_2);


/* --------------------------------------------------------- */
/*                   S T A T I C   D A T A                   */
/* --------------------------------------------------------- */

// access points preferences >> the strongest one will be chosen
// ESP8266WiFiMulti::addAP(ssid, password):
//    ssid = service set identifier
//    password = access point password
ESP8266WiFiMulti wifiMulti;

// station network settings
IPAddress ip(192, 168, 1, 101);             // local IP address
IPAddress gateway(192, 168, 1, 1);          // see local network gateway
IPAddress mask(255, 255, 255, 0);           // subnet mask

// fitolamp state state
bool isFitolampOn = false;

// MDNS settings
const char* responder = "esp8266";

// HTTP server
ESP8266WebServer webServer(80);


/* --------------------------------------------------------- */
/*             S T A T I C   F U N C T I O N S               */
/* --------------------------------------------------------- */

// setup functions
static void set_access_points_preferences();
static void set_serial_communication(int baudrate);
static bool set_mdns_responder(const char* responder);
static void set_web_server_callbacks();

// utilities
static void blink(bool startLevel, int delayMs);
static bool connect_access_point(int attempts, int attemptDelay);         // @attempts: negative for endless tries


/* --------------------------------------------------------- */
/*                        M A I N                            */
/* --------------------------------------------------------- */

void setup() 
{
    bool result;
    set_serial_communication(9600);

    // configure pin:
    //    LED_BUILTIN indicates work
    //    PIN_FITOLAMP might be accessed throug web-interface
    pinMode(LED_BUILTIN, OUTPUT);     digitalWrite(LED_BUILTIN, LOW);
    pinMode(PIN_FITOLAMP, OUTPUT);    digitalWrite(PIN_FITOLAMP, LOW);

    // set esp network settings
    result = WiFi.config(ip, gateway, mask);
    if (!result)
    {
        __PINFO("Failed to set custom ESP station network properties. ", "Defaults are used");
        goto end;
    }

    // connect to existent access point
    set_access_points_preferences();
    result = connect_access_point(-1 , 200);    // led is on after processing
    if (!result)
    {
        __PINFO("Failed to connect to access points. ", "");
        digitalWrite(LED_BUILTIN, HIGH);
        goto end;
    }

    // MDNS settings
    result = set_mdns_responder(responder);
    if (!result)
    {
        __PINFO("Failed to set MDNS responder: ", responder);      
        goto end;
    }

    // HTTP server
    set_web_server_callbacks();
    webServer.begin();

    // Information
    __PINFO("Sketch = ", SKETCH)
    __PINFO("Connected to = ", WiFi.SSID());
    __PINFO("ESP info", " = ");
    __PINFO("  MAC address   : ", WiFi.macAddress());
    __PINFO("  local IP      : ", WiFi.localIP());
    __PINFO("  mDNS (.local) : ", responder);
    __PINFO("Remote hardware", " = ");
    __PINFO("  fitolamp pin  : ", PIN_FITOLAMP);

end:
    ;   // required by label
}

void loop() 
{
    // must be called in every 'loop' to run the mDNS processing
    MDNS.update();  

    // HTTP requests from clients listener
    webServer.handleClient(); 
}


/* --------------------------------------------------------- */
/*             S T A T I C   F U N C T I O N S               */
/* --------------------------------------------------------- */

void set_access_points_preferences()
{
    wifiMulti.addAP("Keenetic-4349", "UDWMpKf6" );
    wifiMulti.addAP("MDC_5GHz", "ypswa5WCJ");
}

void set_serial_communication(int baudrate)
{
    Serial.begin(baudrate);
    while(!Serial)
    {
    }
    Serial.println("Serial port is opened");
}

bool set_mdns_responder(const char* responder)
{
    bool result = MDNS.begin(String(responder));
    return result;
}

void set_web_server_callbacks()
{
    // the only callback for URIs weren't defined
    webServer.onNotFound(uri_not_found);    
    
    // callbacks for existent URIs
    webServer.on(URI_ROOT, uri_root);
    webServer.on(URI_STAT, uri_stat);
    webServer.on(URI_LAMP, uri_lamp);
    // ...
}

void blink(bool startLevel, int delayMs)
{
    digitalWrite(LED_BUILTIN, startLevel);
    delay(delayMs);
    digitalWrite(LED_BUILTIN, !startLevel);
    delay(delayMs);
}

bool connect_access_point(int attempts, int attemptDelay)
{
    bool result = false;
    wl_status_t status;
    
    bool endless = (attempts < 0);
    while (endless || attempts--)
    {
        status = wifiMulti.run();
        if (status == WL_CONNECTED)
        {
          result = true;
          break;
        }
        blink(HIGH, attemptDelay / 2);
    }

    return result;
}