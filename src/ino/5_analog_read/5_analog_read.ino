#define ANALOG_PIN A0
#define TIMEOUT (1000)   // 1 sec
#define REF 5.0f         // for Ardiuno Nano VCC = 5V and for analodReference(DEFAULT)
float voltage;

void setup()
{
    Serial.begin(9600, SERIAL_8N1);
    while(!Serial)
    {
    }
    Serial.println("Serial port is opened");

    pinMode(ANALOG_PIN, INPUT);

    // specify supply reference, volt
    //      DEFAULT     equal to VCC voltage
    //      INTERNAL    1.1
    //      EXTERNAL    voltage on AREF pin (range from 0 to VCC is only allowed)
    //
    // [!] always set this property in the right order with 'analogRead()'
    analogReference(DEFAULT);
}

void loop()
{
    // transform analog signal to number of 10 bit = range 0..1023
    //      > to get voltage, use formula: voltage = ((REF / 1023) * value) - REF is supply reference, value is measuring result  
    //      > when start, several measuring shuld be ignored = calibrating
    voltage = (REF / 1023) * analogRead(ANALOG_PIN);
    
    
    Serial.println(voltage);
    delay(TIMEOUT);
}