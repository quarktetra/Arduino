
int VoltageSensorPin = A0;  // This will measure the capacitor voltage, which is the same as the coil voltage. 
int CurrentSensorPin = A1;  // The output put of the ACS712 module. Centered around  512 or so when there is no current.  1A corresponds to 36 units per calibration.
int TriggerSensorPin = A2;  // This will go down to zero as the switch is closed. Will use this to set t=0  
int OpticalSensorPin = A3;  // This connects to the photosensor               
int VoltageSensorValue;     
int CurrentSensorValue;     
int TriggerSensorValue; 
int OpticalSensorValue;     

void setup() {
    ADCSRA = 0;             // clear ADCSRA register
  //ADCSRB = 0;             // clear ADCSRB register
  //ADMUX |= (0 & 0x07);    // set A0 analog input pin
  //ADMUX |= (1 << REFS0);  // set reference voltage
// ADMUX |= (1 << ADLAR);  // left align ADC value to 8 bits from ADCH register

  // sampling rate is [ADC clock] / [prescaler] / [conversion clock cycles]
  // for Arduino Uno ADC clock is 16 MHz and a conversion takes 13 clock cycles
  //ADCSRA |= (1 << ADPS2) | (1 << ADPS0);    // 32 prescaler for 38.5 KHz
 // ADCSRA |= (1 << ADPS2);                     // 16 prescaler for 76.9 KHz
 ADCSRA |= (1 << ADPS1) | (1 << ADPS0);    // 8 prescaler for 153.8 KHz
    ADCSRA |= (1 << ADEN);  // enable ADC
  ADCSRA |= (1 << ADSC);  // start ADC measurements
    TCCR2A = 0;
  TCCR2B = 0;
  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(COM2B0) | _BV(WGM21) | _BV(WGM20); 
  TCCR2B = _BV(CS20);
 
  Serial.begin(2000000); // This should match the value in Python if data is read from that side. 250,000 is fast enough to sample once per microsecond.
  Serial.print("time_ms"); Serial.print(",");Serial.print("Voltage"); Serial.print(",");Serial.print("Current"); Serial.print(",");Serial.println("Trigger"); //match the Header in Python.
}

int time0=micros();
int triggered=0;
void loop() {
  int delta=micros()-time0;
  VoltageSensorValue = analogRead(VoltageSensorPin);
  CurrentSensorValue = analogRead(CurrentSensorPin);
  OpticalSensorValue = analogRead(OpticalSensorPin);
  
  if(triggered==0){TriggerSensorValue = analogRead(TriggerSensorPin);if(TriggerSensorValue<50){triggered=1;};   }; // skip reading this
Serial.print(micros()-time0); Serial.print(",");Serial.print(VoltageSensorValue); Serial.print(",");Serial.print(CurrentSensorValue);; Serial.print(",");Serial.print(TriggerSensorValue);Serial.print(",");Serial.println(OpticalSensorValue);
}
