
int CalibrationButtonPin = 7;  // calib start  
int OpticalSensorPin = A3;  // This connects to the photosensor               
int VoltageSensorPin = A4;  // This will measure the capacitor voltage, which is the same as the coil voltage. 
int CurrentSensorPin = A5;  // The output put of the ACS712 module. Centered around  512 or so when there is no current.  1A corresponds to 36 units per calibration.

int TriggerButtonPin = 2;  // This will go down to zero as the switch is closed. Will use this to set t=0; will use this to activate the transistor  
int TransistorBasePin = 3;  // This will go down to zero as the switch is closed. Will use this to set t=0; will use this to activate the transistor  
int RelayTransistorBasePin = 4;  // Activate realy; for calibration this is PNP; leave it at HIGH to turn it off

int StatusLEDPin=8; //whatsup LED
const byte maxData = 400; 
int VoltageSensorValue[maxData];     
int CurrentSensorValue[maxData];     
int OpticalSensorValue[maxData];      
long timer[maxData]; 
int triggerred=0 ;
int calibrate=1;
void setup() {
  pinMode(TriggerButtonPin, INPUT);
  pinMode(StatusLEDPin, OUTPUT);
   pinMode(StatusLEDPin, OUTPUT);
  pinMode(TransistorBasePin, OUTPUT);
     pinMode(RelayTransistorBasePin, OUTPUT);

  analogWrite(TransistorBasePin,0);
  
 

  digitalWrite(RelayTransistorBasePin,HIGH);
   digitalWrite(StatusLEDPin,LOW);


 //digitalWrite(RelayTransistorBasePin,LOW);

 
 //analogWrite(TransistorBasePin,0);
  digitalWrite(StatusLEDPin,LOW);
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
analogWrite(TransistorBasePin,0);
  if(calibrate==1){
     digitalWrite(StatusLEDPin,HIGH);
            Serial.print("Sensor BEFORE:");Serial.println(analogRead(OpticalSensorPin));

        digitalWrite(RelayTransistorBasePin,LOW);
      delay(2000);
      digitalWrite(StatusLEDPin,LOW);
 analogWrite(TransistorBasePin,255);
 delay(5000);
 Serial.print("Sensor AFTER:");Serial.println(analogRead(OpticalSensorPin));
 delay(100);
  analogWrite(TransistorBasePin,0);

      delay(3000);
      
    }
  analogWrite(TransistorBasePin,0);
  int TriggerButtonPinValue = digitalRead(TriggerButtonPin);
 //delay(500);Serial.print("; TriggerButtonPinValue:");Serial.println(TriggerButtonPinValue);
  if(TriggerButtonPinValue==0){  ;triggerred=1;

 
  };

//  

if(triggerred==1){
  digitalWrite(StatusLEDPin,LOW);
  //analogWrite(TransistorBasePin,255);
    int time0=micros();

  // Serial.println( "START MEASURE");
 for (int i=0; i<maxData; i++) {
 
    VoltageSensorValue[i] = analogRead(VoltageSensorPin);
    CurrentSensorValue[i] = analogRead(CurrentSensorPin);
    OpticalSensorValue[i] = analogRead(OpticalSensorPin);
    //delayMicroseconds(188);
    delayMicroseconds(300);
    
  //delay(1);
    timer[i] = micros()-time0;

   // Serial.print( "triggerred:"); Serial.print( triggerred);   Serial.print( "  MEASURED voltage:");  Serial.print(  VoltageSensorValue[i]);  Serial.print( "at t:"); Serial.println(  timer[i]);



 };

   

   for (int i=0; i<20; i++) {
  Serial.print( "PIPE CLEANER:");Serial.println( VoltageSensorValue[0]);
   };

  //Serial.print("index"); Serial.print(",");Serial.print("time_us"); Serial.print(",");Serial.print("Voltage"); Serial.print(",");Serial.print("Current"); Serial.print(",");Serial.print("Trigger");; Serial.print(",");Serial.println("optic");//match the Header in Python.
  for (int i=1; i<maxData; i++) {
    
   //Serial.print("SENDING--");  
   Serial.print(i); 
   Serial.print(",");Serial.print(timer[i]); 
   //Serial.print(",");Serial.print(timer[i]/1000); 
   Serial.print(",");Serial.print( VoltageSensorValue[i]);//
   Serial.print(",");Serial.print( CurrentSensorValue[i]);
   Serial.print(",");Serial.println( OpticalSensorValue[i]);
   delay(10);
  };
  triggerred=2;
  analogWrite(TransistorBasePin,0);;
  delay(5000);
  //digitalWrite(StatusLEDPin,HIGH);
  //Serial.print( "triggerred:"); Serial.print( triggerred); Serial.println( "--ALL SENT");

}
}

 //Serial.print(micros()-time0); Serial.print(",");Serial.print(VoltageSensorValue); 
  //Serial.print(",");Serial.print(CurrentSensorValue);; Serial.print(",");Serial.print(TriggerSensorValue);
  //Serial.print(",");Serial.println(OpticalSensorValue);
