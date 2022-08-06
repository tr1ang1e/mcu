#define INPUT_PORT (A0)


void setup() 
{
   Serial.begin(9600, SERIAL_8N1);
   while(!Serial)
   {
   }
   Serial.println("Serial port is opened");


   pinMode(INPUT_PORT, INPUT_PULLUP);
   Serial.println("GPIO port configured for INPUT");

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
