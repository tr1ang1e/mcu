// description
// translate english to morze code using serial port
// only messages smaller than 64 bytes per time are available (serial buffer limit)


// theory
// reference = https://www.arduino.cc/reference/en/language/functions/communication/serial/
// serial port settings = https://en.wikipedia.org/wiki/Serial_port#Settings
// how to process incoming serial data without blocking: http://www.gammon.com.au/serial
// async serial peripheral interface: http://www.gammon.com.au/forum/?id=10894


#define MORZE_PORT (13)    // connected to LED on MCU


// morze intervals        (ms, 1 ms = 0.001 sec)
#define UNIT_TIME         (300)
#define DOT               (UNIT_TIME)
#define DASH              (UNIT_TIME * 3) 
#define IN_LETTER_PAUSE   (UNIT_TIME)
#define IN_WORD_PAUSE     (UNIT_TIME * 3 - UNIT_TIME)     // 3 is correct, but one UNIT_TIME already waited after symbol which was before
#define IN_SENT_PAUSE     (UNIT_TIME * 7 - UNIT_TIME)     // 7 is correct, but one UNIT_TIME already waited after symbol which was before


#define ALPHABET_SIZ (26)
#define ASCII_OFFSET (97)   // 'a' code in ASCII table  ::  is used to get array index 
#define LOWER_OFFSET (32)   // ('A' - 'a') delta in ASCII table  ::  is used to get lower case 
#define SPACE_SYMBOL ' '


const char* ALPHABET[ALPHABET_SIZ] =
{
  [ 0] = ".-",      [ 1] = "-...",    [ 2] = "-.-.",    [ 3] = "-..",       //    A  B  C  D
  [ 4] = ".",       [ 5] = "..-.",    [ 6] = "--.",     [ 7] = "....",      //    E  F  G  H
  [ 8] = "..",      [ 9] = ".---",    [10] = "-.-",     [11] = ".-..",      //    I  J  K  L
  [12] = ".-..",    [13] = "--",      [14] = "-.",      [15] = "---",       //    M  N  O  P
  [16] = ".--.",    [17] = "--.-",    [18] = ".-.",     [19] = "...",       //    Q  R  S  T
  [20] = "-",       [21] = "..-",     [22] = "...-",    [23] = ".--",       //    U  V  W  X    
  [24] = "-.--",    [25] = "--.."                                           //    Y  Z
};


char to_lower(char letter)
{
  if (letter >= 'a' && letter <= 'z')         return letter;
  else if (letter >= 'A' && letter <= 'Z')    return letter + LOWER_OFFSET;
  else                                        return -1;
}


char get_alphabet_index(char letter)
{
  char result = letter - ASCII_OFFSET;
  return (result >= 0 && result < ALPHABET_SIZ) ? result : -1;
}


/* Set digital level on the pin
 * Arguments:
 * 
 * [in] pin (only allowed to output digital signal = 00..19)
 * 
 * [in] signal
 *      - low  :  0 or 'false' or LOW
 *      - high :  1 or 'true' or HIGH
 *
 * */
void signalize(int port, char symbol)
{
  digitalWrite(port, HIGH);
  
  if (symbol == '.')
  {
    delay(DOT);
  }
  else
  {
    delay(DASH);
  }

  digitalWrite(port, LOW);
}


void setup() 
{
  /* Open serial port, leads to MCU reset  
   * Arguments:
   * 
   * [in] long data_rate:
   *      - transaction speed in bounds (bits per second) 
   *      - must be the same in IDE serial port monitor, if it is used
   * 
   * [in] macro config    
   *      - DPS parameters
   *        D = data
   *        P = parity bit (N = no bit, E = even parity, O = odd parity)
   *        S = stop 
   *        In addition, starting bit always presents
   *        So the frame will be:
   *          8N1  :  1 + 8 + 0 (none) + 1 = 10 bits    // default
   *          7E2  :  1 + 7 + 1 (even) + 2 = 11 bits 
   *
   * */
  Serial.begin(9600, SERIAL_8N1);


  // wait for serial port to open
  // doesn't make sence for Arduino without native USB (only with Programming USB)
  // just for example (see docs)
  while(!Serial)
  {
  }
  Serial.println("Serial port is opened");


  /* Configure ports
   * 
   * [in] port number:
   *      00..13            =  digital
   *      14..19 or A0..A5  =  analogue
   *
   * [in] mode:
   *      - INPUT
   *      - INPUT_PULLUP    = ...
   *      - OUTPUT
   *
   * */
   pinMode(MORZE_PORT, OUTPUT);
   Serial.println("GPIO port configured for OUTPUT");

   Serial.println("Initialization is done");
}


// digitalWrite

void loop() 
{
  if (Serial.available())  // check if there are bytes in the serial recieve buffer (buffer size = 64 bytes)
  {
    char letter = to_lower(Serial.read());
    char letter_index = get_alphabet_index(letter);
    
    if (letter != -1 && letter_index != -1)
    {
      const char* letter_code = ALPHABET[(int)letter_index];
      int len = strlen(letter_code);

      for (int i = 0; i < len; ++i)
      {        
        char symbol = letter_code[i];
        Serial.print(symbol);

        signalize(MORZE_PORT, symbol);
      
        // wait between symbols in one letter
        delay(IN_LETTER_PAUSE);
      }
    }
    else
    {
      Serial.print(SPACE_SYMBOL);

      // wait between words in one sentence or if unknown symbol is received
      delay(IN_SENT_PAUSE);
    }

    // wait between letters in one word
    delay(IN_WORD_PAUSE);    
  }


  /* Serial port are available through:
   *   - USB interface
   *   - RX and TX pins
   * USB interface is always corresponded to one pair of RX-TX. So if there is the only RX-TX pair, 
   * serial port is available either through the USB or RX-TX pins at the same time. Uncomment and 
   * use the following command to reopen port if necessary
   * 
   * */
  // Serial.end();
}
