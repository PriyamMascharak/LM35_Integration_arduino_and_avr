/*Change the REF VCC based on the measured 
value if you want accurate sensor reading */
#define REF_VCC 5.053   
// Clock frequency and prescaler
#define CLOCK_FREQ 1000000UL  // 1 MHz
#define PRESCALER 1024

// To calculate compare value
// OCR1A = (time(s) * clock Freq.)/prescaler
// Calculate Timer compare frequency (TCF) and period (TCP)
#define TCF (CLOCK_FREQ / PRESCALER)
#define TCP (1.0 / TCF)

// Define timing options in milliseconds
#define BLINK_INTERVAL_250MS (250 / TCP)
#define BLINK_INTERVAL_500MS (500 / TCP)


#define LED 13
const int sensor=A1; // Assigning analog pin A1 to variable 'sensor'
float tempc; //variable to store temperature in degree Celsius
float tempf; //variable to store temperature in Fahreinheit
float vout; //temporary variable to hold sensor reading
boolean ledState = false;          //determines current state of led
void setup() 
{
    pinMode(sensor,INPUT);           // Configuring sensor pin as input
    Serial.begin(9600);

    pinMode(LED, OUTPUT);
    TCCR1A = 0;                      //Reset Timer1 control Registor A

    bitClear(TCCR1B, WGM13);         //Set CTC mode
    bitSet(TCCR1B, WGM12);


    bitSet(TCCR1B, CS12);            //Set prescaler to 1024
    bitClear(TCCR1B, CS11);
    bitSet(TCCR1B, CS10);

    //Reset Timer1
    TCNT1 = 0;


    bitSet(TIMSK1, OCIE1A);           // Enable Timer1 compare interrupt
    sei();                            //Enable Global inturrupt
}

void loop() 
{

    vout=analogRead(sensor); //Reading the value from sensor
    vout=(vout*500)/1024;
    
    
    tempc=vout; // Storing value in Degree Celsius
    
    
    tempf=(vout*1.8)+32; // Converting to Fahrenheit

    if(tempc<30)
    {

      OCR1A = BLINK_INTERVAL_250MS;       
      /*Setting the Output Compare Register A value so that the interval is  250ms*/
    }
    else
    {
      OCR1A = BLINK_INTERVAL_500MS;
      /*Setting the Output Compare Register A value so that the interval is  500ms*/
    }
    
    Serial.print("in DegreeC="); 
    Serial.print("\t");
    Serial.print(tempc);
    Serial.print(" ");  
    Serial.print("in Fahrenheit=");
    Serial.print("\t");
    Serial.print(tempf);  
    Serial.println();
}

ISR(TIMER1_COMPA_vect)
{
  if (ledState)                // check if ledstate is true then convert to false
  {
    ledState = false;
  }
  else                         // if ledstate is false then this make it true
  {
    ledState = true;
  }
  digitalWrite(LED, ledState); //Set LED pin state as per ledState 
}
