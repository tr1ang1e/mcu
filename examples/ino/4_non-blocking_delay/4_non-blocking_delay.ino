/* Contact bounce = 
 *   when a contact swithes ON, the switch elements will often bounce, 
 *   even if only briefly, before making final contact. Problem.
 * 
 * To solve the problem there are two approaches:
 *   bad: delay() for time = it stops the programm execution
 *   good: use timer variable value delta as one of condition to check contact state
 * In both cases time (delay or timer delta) must be appropriate to wait contact bounce ending
 *
 * */
#define GOOD_DELAY 50
bool ledState = 0;  // LED is off by default when system starts


#define INPUT_PORT (A0)
#define OUTPUT_PORT (13)
int eventsCounter = 0;


#define BOUNCE_TIME_DELTA 50 
unsigned long lastEvent = 0;
// good approach = don't stop programm execution by delay(), but add timer delta usage
bool is_time_enough()
{
    return (millis() - lastEvent) > BOUNCE_TIME_DELTA;
}


bool contactData;
bool contactCurrent = 1;        // starts with 1 because of pullup     
bool isEvent;   
// construction to avoid getting continuous data from pin and to get only single event on button 
// [in] byRelease:
//      false = notify button is pressed
//      true = notify button is released
bool get_button_event(bool byRelease = true)
{
    isEvent = false;
    contactData = digitalRead(INPUT_PORT);
    
    // button pressed
    // avoid contact bounce by checking if time delta is enough
    if(contactCurrent && !contactData && is_time_enough())         
    {
        contactCurrent = 0;
        
        if(!byRelease)
        {
            isEvent = true;
            lastEvent = millis();

        }
    }

    // button released
    // avoid contact bounce by checking if time delta is enough
    else if (!contactCurrent && contactData && is_time_enough())    
    {
        contactCurrent = 1;

        if(byRelease)
        {
            isEvent = true;
            lastEvent = millis();
        }
    }

    return isEvent;
}


void setup()
{
    Serial.begin(9600, SERIAL_8N1);
    while(!Serial)
    {
    }
    Serial.println("Serial port is opened");
    
    pinMode(INPUT_PORT, INPUT_PULLUP);
    pinMode(OUTPUT_PORT, OUTPUT);
    Serial.println("GPIO ports configured");
    Serial.println("Initialization is done");
}


void loop() 
{
    if (get_button_event())
    {
        ledState = !ledState;
        ++eventsCounter;
        
        digitalWrite(OUTPUT_PORT, ledState);
        Serial.println(eventsCounter);
    }
}
