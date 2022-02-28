const byte interruptPin = 2; //connect the optical signal here.
int counterV=0;
int lasttime=millis();

void setup()
{

  Serial.begin(9600);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), my_interrupt_handler, FALLING);
    delay(1000);  
   Serial.println("Ready.");

  }

void loop() {}



void my_interrupt_handler()
{
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > 100) // debouncing noise.
  {
      counterV=counterV+1;
 
     last_interrupt_time = interrupt_time;
    Serial.print("count:"); Serial.println(counterV); 
    
        
  }

}
