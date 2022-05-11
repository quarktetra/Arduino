
// read analog inputs --> save to internal memory  --> send to PC
int VoltageSensorPin = A0;  // This will measure the capacitor voltage, which is the same as the coil voltage. 
int CurrentSensorPin = A1;  // The output put of the ACS712 module. Centered around  512 or so when there is no current.  1A corresponds to 36 units per calibration.
int TriggerSensorPin = A2;  // This will go down to zero as the switch is closed. Will use this to set t=0  
int OpticalSensorPin = A3;  // This connects to the photosensor               
 

const byte maxData = 400; 
int VoltageSensorValue[maxData];     
int CurrentSensorValue[maxData];     
int TriggerSensorValue[maxData]; 
int OpticalSensorValue[maxData];      
long timer[maxData]; 
int triggerred=0 ;
void setup() {
    /*  The bit of code below was taken from:
   *  http://yaab-arduino.blogspot.it/p/oscope.html
   */
  ADCSRA = 0;             // clear ADCSRA register
  //ADCSRB = 0;             // clear ADCSRB register
  //ADMUX |= (0 & 0x07);    // set A0 analog input pin
  //ADMUX |= (1 << REFS0);  // set reference voltage
// ADMUX |= (1 << ADLAR);  // left align ADC value to 8 bits from ADCH register

  // sampling rate is [ADC clock] / [prescaler] / [conversion clock cycles]
  // for Arduino Uno ADC clock is 16 MHz and a conversion takes 13 clock cycles
  //ADCSRA |= (1 << ADPS2) | (1 << ADPS0);    // 32 prescaler for 38.5 KHz
  ADCSRA |= (1 << ADPS2);                     // 16 prescaler for 76.9 KHz
 // ADCSRA |= (1 << ADPS1) | (1 << ADPS0);    // 8 prescaler for 153.8 KHz
    ADCSRA |= (1 << ADEN);  // enable ADC
  ADCSRA |= (1 << ADSC);  // start ADC measurements
    TCCR2A = 0;
  TCCR2B = 0;
  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(COM2B0) | _BV(WGM21) | _BV(WGM20); 
  TCCR2B = _BV(CS20);
 

      Serial.begin(115200); // This should match the value in Python if data is read from that side. 250,000 is fast enough to sample once per microsecond.


};


void loop() {
  //delay(100);
 // Serial.print("TRIGGER VALUE:");Serial.print( analogRead(TriggerSensorPin));Serial.print(",");Serial.print(millis()); Serial.print("; Voltage:");Serial.print(99);  Serial.print(", Current:");Serial.print(analogRead(CurrentSensorPin)); 
  //Serial.print(", triggerred:");Serial.println( triggerred);
 // Serial.print("; Optic:");Serial.println(analogRead(OpticalSensorPin));
  if(analogRead(TriggerSensorPin)<50){triggerred=1; };

//  

if(triggerred==1){
  int time0=micros();
  // Serial.println( "START MEASURE");
 for (int i=0; i<maxData; i++) {
  
    VoltageSensorValue[i] = analogRead(VoltageSensorPin);
    CurrentSensorValue[i] = analogRead(CurrentSensorPin);
    TriggerSensorValue[i] = analogRead(TriggerSensorPin);
    OpticalSensorValue[i] = analogRead(OpticalSensorPin);
    //delayMicroseconds(188);
    delayMicroseconds(100); // change this number to up/down sample
  //delay(1);
    timer[i] = micros()-time0;

   // Serial.print( "triggerred:"); Serial.print( triggerred);   Serial.print( "  MEASURED voltage:");  Serial.print(  VoltageSensorValue[i]);  Serial.print( "at t:"); Serial.println(  timer[i]);



 };

   for (int i=0; i<20; i++) {
  Serial.print( "PIPE CLEANER:");Serial.println( VoltageSensorValue[0]);
   };

  //Serial.print("index"); Serial.print(",");Serial.print("time_us"); Serial.print(",");Serial.print("Voltage"); Serial.print(",");Serial.print("Current"); Serial.print(",");Serial.print("Trigger");; Serial.print(",");Serial.println("optic");//match the Header in Python.
  for (int i=0; i<maxData; i++) {
    
   //Serial.print("SENDING--");  
   Serial.print(i); 
   Serial.print(",");Serial.print(timer[i]); 
   //Serial.print(",");Serial.print(timer[i]/1000); 
   Serial.print(",");Serial.print( VoltageSensorValue[i]);//
   Serial.print(",");Serial.print( CurrentSensorValue[i]);
   Serial.print(",");Serial.print( TriggerSensorValue[i]);
   Serial.print(",");Serial.println( OpticalSensorValue[i]);
   delay(10);
  };
  triggerred=2;
  delay(100);
  //Serial.print( "triggerred:"); Serial.print( triggerred); Serial.println( "--ALL SENT");
  delay(9999999999999);
  exit(0);
  Serial.println( "NOOOOOOOOOOO");
}
}

 //Serial.print(micros()-time0); Serial.print(",");Serial.print(VoltageSensorValue); 
  //Serial.print(",");Serial.print(CurrentSensorValue);; Serial.print(",");Serial.print(TriggerSensorValue);
  //Serial.print(",");Serial.println(OpticalSensorValue);
