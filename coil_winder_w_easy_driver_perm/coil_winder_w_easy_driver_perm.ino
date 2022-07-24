
// coil winder with Arduino uno, 4-terminal stepper and easy drive module
const byte opticalSignal = 2; //connect the optical signal here. Digital output of the coupler
const byte startStopSignal=3 ;
int counterV=0;
int lasttime=millis();

int sweepDir = 12;
int sweepStep = 13;
int sweepEnable = 11;


int windingDir = 8;
int windingStep = 9;
int windingEnable = 10;

int interWait=5000;
int sweepDelayTime=300; //in micro sec
int windingDelayTime=300; //in micro sec

float cHeigth=11.8; //mm inner height
float wDiameter=0.435;   //
float nTurnsPerLayer=floor(cHeigth/wDiameter);  //12/0.43 will fit 28 turns per layer
float sweepStepsPerRev=wDiameter*1.15*400;// spreader motor
float overshoot=6;  
int numLayers=2;
int layerIndex;
float windingStepsPerRev=2.11*1600; // 2*1600 gives a full rev
int redCount=0;
int totalturns=0;
int stopped=1;
 static unsigned long last_start_stop_time = 0;
void setup() {
  pinMode(opticalSignal, INPUT_PULLUP);
  pinMode(startStopSignal, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(opticalSignal), my_interrupt_handler, FALLING);
  attachInterrupt(digitalPinToInterrupt(startStopSignal), startStop, FALLING);
  
  Serial.begin(115200);

  pinMode(sweepDir, OUTPUT);
  pinMode(sweepStep, OUTPUT);
  pinMode(sweepEnable, OUTPUT);
  digitalWrite(sweepDir, HIGH);     // Set the Wdirection.
  digitalWrite(sweepEnable, HIGH); //disable the Wmotor
  pinMode(windingDir, OUTPUT);
  pinMode(windingStep, OUTPUT);
  pinMode(windingEnable, OUTPUT);
  digitalWrite(windingDir, LOW);     // Set the Wdirection.
  digitalWrite(windingEnable, HIGH); //disable the Wmotor
  Serial.print("nTurnsPerLayer");Serial.print(nTurnsPerLayer);Serial.print("; sweepStepsPerRev:");Serial.println(sweepStepsPerRev); 
  Serial.println("Sending the header info");   

  Serial.print("header:");Serial.print(",Layer");Serial.print(",TurnsInThisLayer"); Serial.print(",TotalTurns"); Serial.print(",HalfRotations");; Serial.print(",cHeigth");; Serial.println(",wDiameter");   
}

void loop()
{

     delay(10000);
     digitalWrite(sweepEnable, LOW); //enable the motor
     digitalWrite(windingEnable, LOW); //enable the motor
     digitalWrite(sweepDir, LOW);     

 int j;int i; int k;

    for (k = 0; k<overshoot; k++) {
        for (i = 0; i<sweepStepsPerRev; i++)       // Iterate for 4000 microsteps. 
       {
                {
                    digitalWrite(sweepStep, LOW);  // This LOW to HIGH change is what creates the
                    digitalWrite(sweepStep, HIGH); // "Rising Edge" so the easydriver knows to when to step.
                    delayMicroseconds(sweepDelayTime);      // This delay time is close to top speed for this   // Too fast-->> the motor stalls.    
                }
              };
     };


digitalWrite(sweepDir, HIGH);   
 for (layerIndex = 0; layerIndex<numLayers; layerIndex++)      {
   digitalWrite(sweepEnable, LOW); //enable the motor
   digitalWrite(windingEnable, LOW); //enable the motor
  if(layerIndex>2 ){overshoot=4;};
  if(layerIndex>1 ){redCount=layerIndex% 2;};
  redCount=0;
    //if(layerIndex>3 ){redCount=2;};
        if(layerIndex% 2 == 0)
                digitalWrite(sweepDir, HIGH);     // Set the Wdirection.;
            else
                digitalWrite(sweepDir, LOW);     // Set the Wdirection.
         
          for (j = 0; j<(nTurnsPerLayer+redCount); j++)      {

            pausenow();
          totalturns=totalturns+1;

          Serial.print("stopped:");Serial.println(stopped);
          //Serial.print(",Layer");Serial.print(", TurnsInThisLayer:"); Serial.print(",TotalTurns"); Serial.print(",HalfRotations:");  

          
    
         for (i = 0; i<windingStepsPerRev; i++)       // Iterate for 4000 microsteps. 
          {
              digitalWrite(windingStep, LOW);  // This LOW to HIGH change is what creates the
              digitalWrite(windingStep, HIGH); // 
              delayMicroseconds(windingDelayTime);      // This delay time is close to top speed for this   // Too fast-->> the motor stalls.    
          } 
          for (i = 0; i<sweepStepsPerRev; i++)       // Iterate for 4000 microsteps. 
          {
              digitalWrite(sweepStep, LOW);  // This LOW to HIGH change is what creates the
              digitalWrite(sweepStep, HIGH); // 
              delayMicroseconds(sweepDelayTime);      // This delay time is close to top speed for this   // Too fast-->> the motor stalls.    
          } 



          Serial.print(layerIndex+1);Serial.print(","); 
          Serial.print(j+1); ;Serial.print(",");
          Serial.print(totalturns); ;Serial.print(",");
          Serial.print(counterV);;Serial.print(",");
          Serial.print(cHeigth);;Serial.print(",");
          Serial.println(wDiameter);
            
        };


        
        
             delay(1000);
if(layerIndex<numLayers-1 ){
                    for (k = 0; k<2*overshoot; k++) {
            for (i = 0; i<sweepStepsPerRev; i++)       // Iterate for 4000 microsteps. 
           {
                    {
                        digitalWrite(sweepStep, LOW);  // This LOW to HIGH change is what creates the
                        digitalWrite(sweepStep, HIGH); // "Rising Edge" so the easydriver knows to when to step.
                        delayMicroseconds(sweepDelayTime);      // This delay time is close to top speed for this   // Too fast-->> the motor stalls.    
                    }
                  };
         };
};

if(layerIndex==numLayers-1 ){
                    for (k = 0; k<overshoot; k++) {
            for (i = 0; i<sweepStepsPerRev; i++)       // Iterate for 4000 microsteps. 
           {
                    {
                        digitalWrite(sweepStep, LOW);  // This LOW to HIGH change is what creates the
                        digitalWrite(sweepStep, HIGH); // "Rising Edge" so the easydriver knows to when to step.
                        delayMicroseconds(sweepDelayTime);      // This delay time is close to top speed for this   // Too fast-->> the motor stalls.    
                    }
                  };
         };
};

 digitalWrite(sweepEnable, HIGH); //disable the motor
     digitalWrite(windingEnable, HIGH); //disable the motor
          if(layerIndex<numLayers-1 ){delay(interWait);delay(interWait);delay(interWait);delay(interWait);delay(interWait);};

 };
   
                            
 digitalWrite(sweepEnable, HIGH); //disable the motor
 digitalWrite(windingEnable, HIGH); //disable the motor  
 Serial.print("terminate");
 delay(1000000);
  exit(0);
 
}


void startStop()
{
//stopped;

  if (millis() - last_start_stop_time> 300) // debouncing noise.
  {   last_start_stop_time=millis();
      //Serial.print("count:"); Serial.println(counterV);   
      ;
      stopped=(stopped+1)% 2;
  }

}
void pausenow()
{
  if(stopped==1){
    delay(50);
    pausenow();
    }
  };

void my_interrupt_handler()
{
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > 300) // debouncing noise.
  {    counterV=counterV+1;
      last_interrupt_time = interrupt_time;
      //Serial.print("count:"); Serial.println(counterV);   
  }

}
