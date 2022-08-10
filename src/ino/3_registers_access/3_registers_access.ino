
#define SET_BIT(port, bit)      (port | ((byte)1 << bit)) 
#define REVERT_BIT(port, bit)   (port ^ ((byte)1 << bit))


void setup()
{ 
    /* Arduino has 2 possible register sizes:
     *    register of  8 bit
     *    register of 16 bit = consists of two 8-bit real registers: 
     *      H (high, 15..8 bits)
     *      L (low, 7..0)
     *      <regname> consists of <regname>H and <regname>L
     * 
     * 8-bit register might be read and write (if allowed) in direct manner
     * 16-bit register parts must be accessed according to the following order:
     *   > read LOW than HIGH 
     *   > write HIGH than LOW
     * 
     * */
    Serial.begin(9600, SERIAL_8N1);
    Serial.print("TCCR1B, 8-bit = "); Serial.println(TCCR1B);                   //  8-bit register 
    Serial.print("ICR1, 16 bit = "); Serial.println(ICR1L + (ICR1H << 8));      // 16-bit register , LOW register first!

    /* For some bits of registers there are bits names
     * These names are constants that define corresponding bit number inside register, e.g.:
     *     register:    TCCR1B      [  7  ][  6  ][  5  ][  4  ][  3  ][  2  ][  1  ][  0  ]
     *     bits:                     ICNC1  ICES1    -    WGM13  WGM12   CS12   CS11   CS10  
     * */
    Serial.println("TCCR1B bits,    name ... value"); 
    Serial.print(  "                ICNC1 .. "); Serial.println(ICNC1);
    Serial.print(  "                ICES1 .. "); Serial.println(ICES1);
    Serial.print(  "                res .... "); Serial.println(5);
    Serial.print(  "                WGM13 .. "); Serial.println(WGM13);
    Serial.print(  "                WGM12 .. "); Serial.println(WGM12);
    Serial.print(  "                CS12 ... "); Serial.println(CS12);
    Serial.print(  "                CS11 ... "); Serial.println(CS11);
    Serial.print(  "                CS10 ... "); Serial.println(CS10);
    Serial.end();
    

    /* Work with pins might be done through the special registers
     *
     * Registers ('x' is corresponding port):
     *   DDRx   =  data direction register    rw        0 (default) = input, 1 = output
     *   PORTx  =  data register              rw        write: if output = logical value, if input = pullup setting
     *   PINx   =  input pins address         r 
     *
     * */
    SET_BIT(DDRB, 5);  // configure D13 = PB5 for output
}


void loop()
{
    PORTB = REVERT_BIT(PORTB, 5);  // blink with D13
    delay(1000);
}