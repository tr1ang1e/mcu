
/* handling external hardware interrupts
 * see datasheets to know how to deal with them:
 *      AVR datasheet (e.g. Atmega328P):
 *          p. 1 = pins configuration (INT0, INT1, PCINT23..0)  // PCINT = pin change interrupt
 *          p. 15 = 6.7 Reset and Interrupt Handling (base information)
 *          p. 49 = 11. Interrupts (detailed information, registers description)
 *      Arduino datasheet (e.g. Nano):
 *          p. 8 = 5 Connector Pinouts
 * 
 * in current example only external INTx interrupts are used
 * PCINTx interrpts require special approach (see datasheets,
 * library example: https://github.com/NicoHood/PinChangeInterrupt)
 * 
 * [!] be aware of:
 *      - interrupts priority
 *      - nested interrupts
 *      - pin working
 * 
 * functions to deal with interrupts
 *      > interrupts()      = enable interrupts (enabled by default)
 *      > noInterrupts()    = disable interrupts (be aware becase PWM and Clock functions will not work after thi function call)
 *      > attachInterupt()  = specify interrupt handler for particular interrupt 
 *      > detachInterrupt   = disable interrupt on particular pin (not all interrupt like noInterrupts() does)
 * 
 * */

#define OUTPUT_PIN (13)
volatile bool indicate;
volatile int debounce;

/* interrupt handler ruquirements:
 *    return value = void
 *    args = no
 *    body =
 *      delay() - doesn't work
 *      millis() and micros() timers dont work
 *      Serial.print() might work not correct
 * 
 * if any global variable is used inside interrupt handler,
 * is must be declared as 'volatile' to avoid compiler optimizations  
 *  
 */
void interrupt_handler()
{
    if ((millis() - debounce) >= 200)  // protection from bounce
    {
        digitalWrite(OUTPUT_PIN, HIGH);
        indicate = true;
        debounce = millis();    // millis() result doesn't change while interrupt handler processes, but millis() function itself works 
    }
}

void setup()
{
    pinMode(OUTPUT_PIN, OUTPUT);
    indicate = false;
    debounce = 0;

    // see docs to get description
    attachInterrupt(INT0, interrupt_handler, RISING);
}

// when interrupted, LED works for 1 second
void loop()
{   
    if (indicate)
    {
        delay(1000);
        digitalWrite(OUTPUT_PIN, LOW);
        indicate = false;
    }
}
