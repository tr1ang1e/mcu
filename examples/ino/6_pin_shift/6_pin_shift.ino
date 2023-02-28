// use shift register (e.g. 74HC595) to control several peripheral devices simultaneously  
// read https://arduino.ru/Tutorial/registr_74HC595 for detaied scheme and descriptions

// pins to communicate with shift rgister
#define LATCH_PIN   8       // synchronize data: LOW = allows to recieve data, HIGH = applies recieved data
#define CLOCK_PIN  12       // synchronaze processor taсts while exchanging data 
#define DATA_PIN   11       // pin for data exchange

void setup()
{
    pinMode(LATCH_PIN, OUTPUT);
    pinMode(CLOCK_PIN, OUTPUT);
    pinMode(DATA_PIN, OUTPUT);
}

void loop()
{
    for (byte output = 0; output < 256; ++output)
    {
        digitalWrite(LATCH_PIN, LOW);

        shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, output);            // bits order: MSBFIRST (LSFIRST) = most (least) significant bit first
        // byte input = shiftIn(DATA_PIN, CLOCK_PIN, MSBFIRST);     // replace the above line with this one if data recieving is required

        digitalWrite(LATCH_PIN, HIGH);
        delay(500);
    }
}
