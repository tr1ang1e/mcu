// programming language: C++ like:
//   classes, references, ... are available
//   be aware of data types sizes


// +---------------------------------------------+
// +           DATA TYPES                        +
// +---------------------------------------------+

                        //    size                                          alias
bool bl = true;         //       1                0 .. 1                    boolean
char c = 'a';           //       1             -128 .. +127   
byte bt = 1;            //       1                0 .. +255
int i = -1;             //       2           -32768 .. +32767
unsigned int ui = 1;    //       2                0 .. +65535               word
long l = -1;            //       4      -2147483648 .. +2147483647
unsigned long ul = 1;   //       4                0 .. +4294967295
float f = 1.0;          //       4      -3.4028235E .. +3.4028235E          double (has same size and same presicion)                                   
 

// +---------------------------------------------+
// +           PROGRAMM STRUCTURE                +
// +---------------------------------------------+

// function to run only once before starting main programm
// prepare controller and peripheral devices to work
// name can't be changed
void setup() 
{
}

// main loop
// will be executing in endless loop
// name can't be changed
void loop() 
{
}
