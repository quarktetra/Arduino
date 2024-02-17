
// coil winder with Arduino uno, 4-terminal stepper and easy drive module
int opticalSignal = 2; //connect the optical signal here. Digital output of the coupler
int windNowButton=3 ;
int changeLayerButton=4 ;

int changeDir=0;
int ledPin = 5;   // the onboard LED

int alldonePin = 7;
int windingDir = 8;
int windingStep = 9;
int windingEnable = 10;
int sweepEnable = 11;
int sweepDir = 12;
int sweepStep = 13;



int counterV=0;
float wLength=0;
int turnInThisLayer=0;
int layerV=1;
int lasttime=millis();
int theDir=1;

int interWait=2000;
int sweepDelayTime=300; //in micro sec
int windingDelayTime=300; //in micro sec

float cHeigth=10;//11.8; //mm inner height
float wDiameter=0.373;   //
int nTurnsPerLayer=round(cHeigth/wDiameter);  //12/0.43 will fit 28 turns per layer
float sweepStepsPerRev=wDiameter*1.03*400;// spreader motor
int overshoot=3;  
int numLayers=20;
int layerIndex;
float windingStepsPerRev=2.11*1600; // 2*1600 gives a full rev
int redCount=0;
int totalturns=0;
int stopped=1;
static unsigned long last_start_stop_time = 0;
static unsigned long last_change_layer_time = 0;
void setup() {
  pinMode(opticalSignal, INPUT_PULLUP);pinMode(windNowButton, INPUT_PULLUP);pinMode(changeLayerButton, INPUT_PULLUP);pinMode(alldonePin, INPUT_PULLUP); 
  
  attachInterrupt(digitalPinToInterrupt(opticalSignal), optical_interrupt_handler, FALLING);
  attachInterrupt(digitalPinToInterrupt(windNowButton), startStop, FALLING);
  
  
  Serial.begin(115200);

  pinMode(sweepDir, OUTPUT);pinMode(sweepStep, OUTPUT);  pinMode(sweepEnable, OUTPUT);
  pinMode(windingDir, OUTPUT); pinMode(windingStep, OUTPUT);pinMode(windingEnable, OUTPUT);
  
  digitalWrite(sweepDir, HIGH); digitalWrite(sweepEnable, HIGH); 
 digitalWrite(windingDir, LOW); digitalWrite(windingEnable, HIGH); 
  //Serial.print("nTurnsPerLayer");Serial.println(nTurnsPerLayer);//Serial.print("; sweepStepsPerRev:");Serial.println(sweepStepsPerRev); 
  Serial.print("Sending the header info. nTurnsPerLayer:");  Serial.println(nTurnsPerLayer);   
  Serial.print("header:");Serial.print(",Layer");Serial.print(",TurnsInThisLayer"); Serial.print(",TotalTurns"); Serial.print(",HalfRotations");Serial.print(",Length");; Serial.print(",cHeigth"); Serial.print(",wDiameter");  
  Serial.print(",numLayers"); Serial.println(",nTurnsPerLayer");     


//pausenow();





   int i; for (i = 0; i<overshoot; i++)  {sweepnow();};
theDir=0;
}

void loop()
{

     delay(200);
     //Serial.print("alldonePin:"); Serial.println(digitalRead(alldonePin));
      if(digitalRead(alldonePin)==0){Serial.println("TerminateTransmission");}


    if(digitalRead(changeLayerButton)==0){
         if (millis() - last_change_layer_time> 5000) // debouncing noise.
            {   last_change_layer_time=millis();
             layerV=layerV+1;
            
             int i; for (i = 0; i<overshoot; i++)  {sweepnow();}; Serial.println("reversing the direction");
              theDir=(layerV+1) % 2;
             turnInThisLayer=0;
                 
                  //Serial.print("counterV:");Serial.print(counterV);Serial.print("; layerV:");Serial.print(layerV);Serial.print(";   theDir:");Serial.println(theDir);

            }
     };


 
}


void startStop()
{

  if (millis() - last_start_stop_time> 300) // debouncing noise.
  {   last_start_stop_time=millis();
      //Serial.print("count:"); Serial.println(counterV);   
      ;
      stopped=(stopped+1)% 2;
         windnow(windingStepsPerRev);// sweepnow(overshoot);
         sweepnow();
         totalturns=totalturns+1;
         turnInThisLayer=turnInThisLayer+1;
         
            if(theDir==0){    digitalWrite(ledPin,HIGH);}else{digitalWrite(ledPin,LOW); }


    // Serial.print("counterV:");Serial.print(counterV);Serial.print("; layerV:");Serial.print(layerV);Serial.print(";   theDir:");Serial.println(theDir);
wLength=float(counterV*3.14159*0.0304/2);

          Serial.print("ToBeLogged,");
          Serial.print(layerV);Serial.print(","); 
          Serial.print(turnInThisLayer); ;Serial.print(",");
          Serial.print(totalturns); ;Serial.print(",");
          Serial.print(counterV);;Serial.print(",");
          Serial.print(wLength);;Serial.print(","); 
          Serial.print(cHeigth);;Serial.print(",");
          Serial.print(wDiameter);Serial.print(",");
          Serial.print(numLayers);Serial.print(",");
          Serial.println(nTurnsPerLayer);

          
   
  }

};




void sweepnow()
{
   // Serial.println("sweep");
      digitalWrite(sweepEnable, LOW); //disable the motor
   if(theDir==0){    digitalWrite(sweepDir, HIGH); }else{ digitalWrite(sweepDir, LOW); }

   int i;  
 
        for (i = 0; i<sweepStepsPerRev; i++)       // Iterate for 4000 microsteps. 
       {
                {
                    digitalWrite(sweepStep, LOW);  // This LOW to HIGH change is what creates the
                    digitalWrite(sweepStep, HIGH); // "Rising Edge" so the easydriver knows to when to step.
                    delayMicroseconds(sweepDelayTime);      // This delay time is close to top speed for this   // Too fast-->> the motor stalls.    
                }
              };
 
  digitalWrite(sweepEnable, HIGH); //disable the motor


};


void windnow(int thesteps)
{
 // Serial.println("windnow");
       digitalWrite(windingEnable, LOW); //enable the motor
   int i; 
    
         for (i = 0; i<thesteps ; i++)       // Iterate for 4000 microsteps. 
          {
              digitalWrite(windingStep, LOW);  // This LOW to HIGH change is what creates the
              digitalWrite(windingStep, HIGH); // 
              delayMicroseconds(windingDelayTime);      // This delay time is close to top speed for this   // Too fast-->> the motor stalls.    
          }
               digitalWrite(windingEnable, HIGH); //disable the motor 
              
               
};

void pausenow()
{
  if(stopped==1){
    delay(50);
 
     digitalWrite(sweepEnable, HIGH); //HIGH: disable, LOW:enable
     digitalWrite(windingEnable, HIGH); //HIGH: disable, LOW:enable
     Serial.print("stopped:");Serial.println(stopped);
     pausenow();
    }else{
         digitalWrite(sweepEnable, LOW); //HIGH: disable, LOW:enable
         digitalWrite(windingEnable, LOW); //HIGH: disable, LOW:enable
      }
  };

void optical_interrupt_handler()
{
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > 300) // debouncing noise.
  {    counterV=counterV+1;
      last_interrupt_time = interrupt_time;
      //Serial.print("count:"); Serial.println(counterV);   
  }

}
