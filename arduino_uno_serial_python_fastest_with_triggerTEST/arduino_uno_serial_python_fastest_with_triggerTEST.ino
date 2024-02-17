

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
int TriggerSensorValue[maxData]; 
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
 Serial.println("HELLOO");

};


void loop() {
  if(calibrate==1){
    float theC=(analogRead(CurrentSensorPin)-511.0)/38;
     Serial.print("Sensor before:");Serial.print(analogRead(OpticalSensorPin)); Serial.print("; current:");Serial.print(theC);; 
     Serial.print("; Voltage:");Serial.println(analogRead(VoltageSensorPin)); 

     digitalWrite(StatusLEDPin,HIGH);
       digitalWrite(RelayTransistorBasePin,LOW);
     delay(1000);
      
        digitalWrite(RelayTransistorBasePin,HIGH);
      delay(2000);
      digitalWrite(StatusLEDPin,LOW);
 analogWrite(TransistorBasePin,255);
 delay(2000);
 delay(200);
  analogWrite(TransistorBasePin,0);
  theC=(analogRead(CurrentSensorPin)-511.0)/38;
 Serial.print("Sensor aFter:");Serial.print(analogRead(OpticalSensorPin));  Serial.print("; current:");Serial.print(theC);; 
     Serial.print("; Voltage:");Serial.println(analogRead(VoltageSensorPin)); 

      delay(20000);
      
    }
  analogWrite(TransistorBasePin,0);
  int TriggerButtonPinValue = digitalRead(TriggerButtonPin);
 //delay(500);Serial.print("; TriggerButtonPinValue:");Serial.println(TriggerButtonPinValue);
  if(TriggerButtonPinValue==0){  ;triggerred=1;

 
  };

 

}

 //Serial.print(micros()-time0); Serial.print(",");Serial.print(VoltageSensorValue); 
  //Serial.print(",");Serial.print(CurrentSensorValue);; Serial.print(",");Serial.print(TriggerSensorValue);
  //Serial.print(",");Serial.println(OpticalSensorValue);
