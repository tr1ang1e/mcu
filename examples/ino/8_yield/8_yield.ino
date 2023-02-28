#define OUTPUT_PIN (13)
uint8_t level; 

void setup()
{
    Serial.begin(9600, SERIAL_8N1);
    while(!Serial)
    {
    }
    Serial.println("Serial port is opened");
    
    pinMode(OUTPUT_PIN, OUTPUT);
    level = HIGH;
}

void loop()
{
    Serial.print("Level is: ");
    Serial.println(level);
    
    delay(1000);    // yield() will be called inside in a loop
    level ^= (uint8_t)0x1;
}

/* special function
 *  
 *  > is called every time when delay() is called : see delay() implementation 
 *  > might be already defined in standard or any other library, so:
 *    - might be redefined in user's source code and this realizations is prior, but
 *    - be aware of built-in or another library realization corrupting
 * 
 **/
void yield()
{
    // not very good example, 
    // because chip will write 'level' value to pin in a loop until delay() is done
    digitalWrite(OUTPUT_PIN, level);         
}
