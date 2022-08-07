/* By default every pin is configured:
 *   - as an input
 *   - in HIZ mode
 * 
 * HIZ = Hi-Z = high impendance:
 *   pin is not connected neither to VCC nor to GND  >>  just a branch
 *   behaves like an antenna and might be affected by electrical noise 
 * 
 * To avoid electrical noise influence, pin must be:
 *   - pulled up   = connected to VCC through resistor  >>  indicates logical 1  <<  built-in realisation is available 
 *   - pulled down = connected to GND through resistor  >>  indicates logical 0  <<  only manually
 * 
 * Uncomment one of the following lines
 * 
 * */

// uint8_t pin_mode = INPUT;              // HIZ
uint8_t pin_mode = INPUT_PULLUP;       // pullup


/* Instruction
 * 
 * 1. HIZ mode
 *    > uncomment pin_mode for INPUT
 *    > load sketch
 *    > connect wire to the port:
 *      - hold wire in hand = 0 and 1 would appear in a random order
 *      - connect wire to GND = only 0 will appear
 *      - connect wire to VCC = only 1 will appear
 * 
 * 2. pullup mode
 *    > uncomment pin_mode for INPUT_PULLUP
 *    > load sketch
 *    > connect wire to the port:
 *      - hold wire in hand = only 1 would appear because of pullup
 *      - connect wire to GND = only 0 will appear
 *      - connect wire to VCC = only 1 will appear   
 *
 * */


#define INPUT_PORT (A0)

void setup() 
{
   Serial.begin(9600, SERIAL_8N1);
   while(!Serial)
   {
   }
   Serial.println("Serial port is opened");

   pinMode(INPUT_PORT, pin_mode);  // HIZ or pullup
   Serial.println("GPIO port configured for INPUT");
   Serial.print("Mode is (0 for HIZ, 1 for pullup) = ");
   Serial.println(pin_mode);

   Serial.println("Initialization is done");
}


int seconds_left = 0;

void loop() 
{
  bool value = digitalRead(INPUT_PORT);
  Serial.print(seconds_left);
  Serial.print("  ");
  Serial.println(value);
  delay(1000);
  ++seconds_left;
}
