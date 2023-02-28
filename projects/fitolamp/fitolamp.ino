/*
    Turn on or off builtin led depending on 
    actions with sensor button: to change state
    button should be pressed for 1 second
*/


#include <Arduino.h>
#include <Esp.h>


#define HOLD_TIME_MS    1000    // 1 sec, safety for ESP because is used in non-blocking mode 
#define BUTTON_PIN      D5      // default logical level depends on button module


// global
bool ledOn = false;
bool currentState = false;
bool previousState = false;
unsigned long pressStart = 0;
unsigned long currTime = 0;


void setup() 
{
  // set serial port communication
  Serial.begin(9600);
  while(!Serial)
  {
  }
  Serial.println("Serial port is opened");

  // configure pins
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
}


void loop() 
{
  currentState = digitalRead(BUTTON_PIN);
  if (!currentState)
  {
    previousState = false;
    pressStart = 0;
  }
  else
  {
    currTime = millis();
    if (!previousState)
    {
      previousState = true;
      pressStart = currTime;
    }
    else
    {
      if ((currTime - pressStart) >= HOLD_TIME_MS)
      {
        ledOn = !ledOn;
        digitalWrite(LED_BUILTIN, !ledOn);    // working level on builtin led is inverted
        previousState = false;                // prevent too continuous button holding
      }
    }
  }
}
