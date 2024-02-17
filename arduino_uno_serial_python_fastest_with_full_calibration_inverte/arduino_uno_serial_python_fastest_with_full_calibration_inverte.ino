
int sweepDir = 12;
int sweepStep = 13;
int sweepEnable = 11;
int sweepDelayTime=300; //in micro sec
float sweepStepsPerRev=400;  //1600 is full turn,
float conversionF=0.26; //50 steps give 13 mm of move
#define jmax 57   

int jwait=200;


int CalibrationButtonPin = 7;  // calib start 
//int CollectorEmittor = A0;  // This connects to the photosensor               
 
int OpticalSensorPin = A3;  // This connects to the photosensor               
int VoltageSensorPin = A4;  // This will measure the capacitor voltage, which is the same as the coil voltage. 
int CurrentSensorPin = A5;  // The output put of the ACS712 module. Centered around  512 or so when there is no current.  1A corresponds to 36 units per calibration.

int TriggerButtonPin = 2;  // This will go down to zero as the switch is closed. Will use this to set t=0; will use this to activate the transistor  
int TransistorBasePin = 3;  // This will go down to zero as the switch is closed. Will use this to set t=0; will use this to activate the transistor  
int RelayTransistorBasePin = 4;  // Activate realy; for calibration this is PNP; leave it at HIGH to turn it off
 int centerOV=0;
  int initOV=0;
int StatusLEDPin=8; //whatsup LED
const byte maxData = 100; 

int VoltageSensorValue[maxData];     
int CurrentSensorValue[maxData];     
int OpticalSensorValue[maxData]; 
int optCalValue[jmax];
float optCalStep[jmax];     
long timer[maxData]; 
int triggered=0 ;
int TriggerButtonPinValue=1;

int FullCailbrationDone=1;
int CentralCailbrationDone=0;
int backoff=0;
int thisOV;

void setup() {
  
pinMode(sweepDir, OUTPUT);
pinMode(sweepStep, OUTPUT);
pinMode(sweepEnable, OUTPUT);
digitalWrite(sweepDir, HIGH);     // Set the Wdirection.
digitalWrite(sweepEnable, HIGH); //disable the Wmotor

  pinMode(TriggerButtonPin, INPUT_PULLUP);
  pinMode(StatusLEDPin, OUTPUT);
   pinMode(StatusLEDPin, OUTPUT);
  pinMode(TransistorBasePin, OUTPUT);
     pinMode(RelayTransistorBasePin, OUTPUT);

  analogWrite(TransistorBasePin,255);
  
 

  digitalWrite(RelayTransistorBasePin,HIGH);
   digitalWrite(StatusLEDPin,LOW);


 //digitalWrite(RelayTransistorBasePin,LOW);

 
 //analogWrite(TransistorBasePin,255);
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


Serial.println("Starting... Hold on to your hats!");
delay(100);

};


void loop() {
  digitalWrite(sweepEnable, HIGH); //disable the Wmotor
  analogWrite(TransistorBasePin,255);
   TriggerButtonPinValue = digitalRead(TriggerButtonPin);
   
  if(TriggerButtonPinValue==0){  ;triggered=1;};

    Serial.print("Main loop read voltage:");Serial.print(analogRead(VoltageSensorPin));Serial.print(" converted voltage:");Serial.println(analogRead(VoltageSensorPin)*0.5);
delay(100);


if(CentralCailbrationDone==0){        
        digitalWrite(RelayTransistorBasePin,LOW);
      Serial.println("Will do a center  calibration");

 delay(1000);
    float theC=(analogRead(CurrentSensorPin)-511.0)/38;
    initOV=analogRead(OpticalSensorPin);
     //Serial.print("initOV:");Serial.println(initOV); 
     //Serial.print("; current:");Serial.print(theC);Serial.print("; Voltage:");Serial.println(analogRead(VoltageSensorPin)); 

     digitalWrite(StatusLEDPin,HIGH);
   
     delay(200);
     digitalWrite(StatusLEDPin,LOW);
 analogWrite(TransistorBasePin,0);delay(3000); 
 theC=(analogRead(CurrentSensorPin)-511.0)/38;
centerOV=analogRead(OpticalSensorPin);
     
 //Serial.print("; current:");Serial.print(theC);; Serial.print("; Voltage:");Serial.println(analogRead(VoltageSensorPin)); 
     analogWrite(TransistorBasePin,255);

    Serial.print("initOV:");Serial.println(initOV);  Serial.print("; centerOV:");Serial.print(centerOV); Serial.print(", deltaOV:");Serial.println(initOV-centerOV); 

  digitalWrite(RelayTransistorBasePin,HIGH);
      
       Serial.println("Center  calibration is Done!");
     CentralCailbrationDone=1;
     ;delay(3000); 
};


  
  if(FullCailbrationDone==0){
       Serial.println("Will do a FULL calibration");
delay(2000);

 digitalWrite(sweepEnable, LOW); //enable the motor

int i;
int j;

 digitalWrite(sweepDir, LOW);     // Set the Wdirection.

     for (j = 0; j<backoff; j++) {



for (i = 0; i<sweepStepsPerRev; i++)       // Iterate for 4000 microsteps. 
  {
      digitalWrite(sweepStep, LOW);  // This LOW to HIGH change is what creates the
      digitalWrite(sweepStep, HIGH); // "Rising Edge" so the easydriver knows to when to step.
      delayMicroseconds(sweepDelayTime);      // This delay time is close to top speed for this   // Too fast-->> the motor stalls.    
  }

     };

delay(1000);

 digitalWrite(sweepDir, HIGH);     // Set the Wdirection.

int bottomToCenter=0;
int CenterToTop=0;
int deltaToCenterOV=999;
int thisDelta=999;
    
     for (j = 0; j<jmax; j++) {



for (i = 0; i<sweepStepsPerRev; i++)       // Iterate for 4000 microsteps. 
  {
      digitalWrite(sweepStep, LOW);  // This LOW to HIGH change is what creates the
      digitalWrite(sweepStep, HIGH); // "Rising Edge" so the easydriver knows to when to step.
      delayMicroseconds(sweepDelayTime);      // This delay time is close to top speed for this   // Too fast-->> the motor stalls.    
  }
        delay(jwait);
     thisOV=analogRead(OpticalSensorPin);
     if(abs(thisOV-centerOV)< deltaToCenterOV ) {

       deltaToCenterOV=abs(thisOV-centerOV);bottomToCenter=j;
       
     }
      // Serial.print(" j:");Serial.print(j);   ;Serial.print(";  OPT:");Serial.print( thisOV);  Serial.print("; thisDelta:");Serial.print(abs(thisOV-centerOV));  Serial.print(" deltaToCenterOV :");Serial.print(deltaToCenterOV ); Serial.print(";  bottomToCenter:");Serial.println(bottomToCenter);

};

CenterToTop=jmax-bottomToCenter;
Serial.print(" CenterToTop:");Serial.println(CenterToTop);  
     
       digitalWrite(sweepDir, LOW);     // Set the Wdirection.
float thisP; 
     delay(200);
     Serial.println(" Reversing...");
      for (j = jmax; j>0; j--) {
        thisP= (j-bottomToCenter-1)*conversionF;
          thisOV=analogRead(OpticalSensorPin);

    optCalValue[j]=thisOV;
    optCalStep[j]=thisP;

           
for (i = 0; i<sweepStepsPerRev; i++)       // Iterate for 4000 microsteps. 
  {
      digitalWrite(sweepStep, LOW);  // This LOW to HIGH change is what creates the
      digitalWrite(sweepStep, HIGH); // "Rising Edge" so the easydriver knows to when to step.
      delayMicroseconds(sweepDelayTime);      // This delay time is close to top speed for this   // Too fast-->> the motor stalls.    
  }
    delay(jwait);

 


     //   Serial.print(" j:");Serial.print(j);Serial.print(" OPT:");Serial.print(thisOV);  Serial.print("; thisP:");Serial.println(thisP);  

      }
 FullCailbrationDone=1;analogWrite(TransistorBasePin,255);
 
 
  digitalWrite(sweepDir, HIGH);     // Set the Wdirection.

     for (j = 0; j<backoff; j++) {



for (i = 0; i<sweepStepsPerRev; i++)       // Iterate for 4000 microsteps. 
  {
      digitalWrite(sweepStep, LOW);  // This LOW to HIGH change is what creates the
      digitalWrite(sweepStep, HIGH); // "Rising Edge" so the easydriver knows to when to step.
      delayMicroseconds(sweepDelayTime);      // This delay time is close to top speed for this   // Too fast-->> the motor stalls.    
  }

     };
 
 digitalWrite(sweepEnable, HIGH); //disable the motor


         Serial.println(" Sending the cali data to PC;"); 
           for (int i=1; i<20; i++) { Serial.println( "PIPE CLEANER:");};
            Serial.print("CalibDataHeader,");Serial.print(  "position" );Serial.print(",");Serial.println( "oValue");
            
                     for (int i=1; i<jmax; i++) {
            
  
 Serial.print("CalibData,");Serial.print(  optCalStep[i] );Serial.print(",");Serial.println( optCalValue[i] );
           delay(10);
          };
        

delay(2000);
 };
  



  
   if(triggered==1){ 
 Serial.println(" Sending a pulse now!"); 

        for (int i=0; i<15; i++) {
          delay(50);digitalWrite(StatusLEDPin,LOW);delay(50);digitalWrite(StatusLEDPin,HIGH);
          };
        
       // analogWrite(TransistorBasePin,0);
         int time0=micros();
          // Serial.println( "START MEASURE");
         for (int i=0; i<maxData; i++) {
         
            VoltageSensorValue[i] = analogRead(VoltageSensorPin);
            CurrentSensorValue[i] = analogRead(CurrentSensorPin);
            OpticalSensorValue[i] = analogRead(OpticalSensorPin);
            if(i==1){analogWrite(TransistorBasePin,0);}
            //delayMicroseconds(188);
            delayMicroseconds(200);
            if(i>40){delayMicroseconds(200);}
             if(i>50){delayMicroseconds(500);}
             
            
            
          //delay(1);
            timer[i] = micros()-time0;
        
           // Serial.print( "triggered:"); Serial.print( triggered);   Serial.print( "  MEASURED voltage:");  Serial.print(  VoltageSensorValue[i]);  Serial.print( "at t:"); Serial.println(  timer[i]);
        
        
        
         };
         delay(1000);
        analogWrite(TransistorBasePin,255);
           
        Serial.println(" Sending the data to PC;"); 
           for (int i=0; i<20; i++) {
          Serial.println( "PIPE CLEANER:");
           };
        
          //Serial.print("index"); Serial.print(",");Serial.print("time_us"); Serial.print(",");Serial.print("Voltage"); Serial.print(",");Serial.print("Current"); Serial.print(",");Serial.print("Trigger");; Serial.print(",");Serial.println("optic");//match the Header in Python.
          for (int i=1; i<maxData; i++) {
            
          
           Serial.print(i); 
           Serial.print(",");Serial.print(timer[i]); 
           //Serial.print(",");Serial.print(timer[i]/1000); 
           Serial.print(",");Serial.print( VoltageSensorValue[i]);//
           Serial.print(",");Serial.print( CurrentSensorValue[i]);
           Serial.print(",");Serial.println( OpticalSensorValue[i]);//-centerOV
           //Serial.print(",");Serial.println( CEValue[i]);//-centerOV
           delay(10);
          };
          analogWrite(TransistorBasePin,255);;
          delay(5000);
          //digitalWrite(StatusLEDPin,HIGH);
          //Serial.print( "triggered:"); Serial.print( triggered); Serial.println( "--ALL SENT");
triggered=0;
};
delay(2000);
// thisOV=analogRead(OpticalSensorPin);;Serial.print("AFTER;  OPT:");Serial.println( thisOV);  
  };

 


 //Serial.print(micros()-time0); Serial.print(",");Serial.print(VoltageSensorValue); 
  //Serial.print(",");Serial.print(CurrentSensorValue);; Serial.print(",");
  //Serial.print(",");Serial.println(OpticalSensorValue);
