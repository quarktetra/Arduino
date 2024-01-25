
// coil winder with Arduino uno, 4-terminal stepper and easy drive module
int opticalSignal = 2; //connect the optical signal here. Digital output of the coupler
int startStopSignal=3 ; // use this to pause the winding
int layerTerminate=4 ; // use this to terminate the layer and move to the next
int counterV=0;
int lasttime=millis();

int sweepDir = 12;
int sweepStep = 13;
int sweepEnable = 11;

int paused=1;
int windingDir = 8;
int windingStep = 9;
int windingEnable = 10;

int interWait=10000;
int sweepDelayTime=300; //in micro sec
int windingDelayTime=300; //in micro sec
float theWireDiameter=0.43;

float cHeigth=15;//12.4; //mm inner height
float wDiameter=0.43;   //
float nTurnsPerLayer=floor(cHeigth/wDiameter);  //12/0.43 will fit 28 turns per layer
float sweepStepsBase=17;// spreader motor
float tiltDistance=3;  // in millimeters
float stepsPerMM= 24;
int numLayers=10;
int layerIndex=1;
float windingStepsPerRev=2.11*1600; // 2*1600 gives a full rev
int redCount=0;
int totalturns=0;
int  TurnsInThisLayerIndex= 0;
int  TurnsInThisLayerIndexR= 0;
int layerTerminateV;
void setup() {
  Serial.begin(115200);
  pinMode(startStopSignal, INPUT_PULLUP);
   pinMode(layerTerminate, INPUT_PULLUP);
  
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
  attachInterrupt(digitalPinToInterrupt(startStopSignal), startStop, FALLING); // pause the motors
  attachInterrupt(digitalPinToInterrupt(opticalSignal), opticalCounter, FALLING);

  Serial.print("nTurnsPerLayer=");Serial.println(nTurnsPerLayer);  //Serial.println("Sending the header info");   

  Serial.print("header:");Serial.print(",Layer");Serial.print(",TurnsInThisLayer"); Serial.print(",TotalTurns"); Serial.print(",HalfRotations");; Serial.print(",cHeigth");; Serial.println(",wDiameter");  
// Python code will look for a serial commumnication starting with "header:", and grab the rest of the items as the table headers.
   delay(1000);

   
   moveSweeper(tiltDistance, 0); //back off

};

void loop()
{
 // Serial.print("paused:");Serial.print(paused);Serial.print("; count:"); Serial.println(counterV);   
  while  (layerIndex<=numLayers){ 
    delay(100);
    
      //if(layerIndex>1 ){redCount=layerIndex% 2;};
      while (TurnsInThisLayerIndex < nTurnsPerLayer-redCount) {
        layerTerminateV=digitalRead(layerTerminate);
        delay(100);  
        if(layerTerminateV==0){layerIndex++; TurnsInThisLayerIndex=0; Serial.println("Terminated this layer!!!");delay(5000); }
        if(paused==0){ 
          Serial.print("layerIndex:");Serial.print(layerIndex);Serial.print("; nTurnsPerLayer:");Serial.print(nTurnsPerLayer); Serial.print(",redCount: "); Serial.print(redCount);
          Serial.print("turns in this layer:");Serial.println(nTurnsPerLayer-redCount);
          TurnsInThisLayerIndex++;moveSweeper(theWireDiameter, layerIndex% 2);delay(200); moveWinder(1);totalturns++; 
          //Serial.print("layerTerminateV:");Serial.print(layerTerminateV);  Serial.print(",");
          Serial.print("ToBeLogged");Serial.print(","); 
          Serial.print(layerIndex);Serial.print(","); 
          Serial.print(TurnsInThisLayerIndex);Serial.print(","); 
          Serial.print(totalturns); ;Serial.print(",");
          Serial.print(counterV);;Serial.print(",");
          Serial.print(cHeigth);;Serial.print(",");
          Serial.println(wDiameter);

        };
          //totalturns=totalturns+1;
      };
   if(paused==0){  
    
     if(layerIndex<numLayers-1 ){moveSweeper(2*tiltDistance, layerIndex% 2); };
      if(layerIndex==numLayers-1 ){moveSweeper(tiltDistance, layerIndex% 2); };
    layerIndex++; TurnsInThisLayerIndex=0;
   
  
   delay(20000);
  }
    
    

    
    
  }
    // Serial.print("layerIndex:");Serial.print(layerIndex); Serial.print(",numLayers:");Serial.println(numLayers); 
if  (layerIndex>=numLayers){ 
 Serial.print("TerminateTransmission");
 delay(1000000);
 exit(0);
};


};



void  moveSweeper(float dist, int dir)
{
  int j;int i; int k;
  digitalWrite(sweepEnable, LOW); //LOW -->enable; HIGH -->DISABLE
  digitalWrite(sweepDir, dir);
    for (k = 0; k<dist*stepsPerMM; k++) {
        for (i = 0; i<sweepStepsBase; i++)       // Iterate for 4000 microsteps. 
               {                       
                    digitalWrite(sweepStep, LOW);  // This LOW to HIGH change is what creates the
                    digitalWrite(sweepStep, HIGH); // "Rising Edge" so the easydriver knows to when to step.
                    delayMicroseconds(sweepDelayTime);      // This delay time is close to top speed for this   // Too fast-->> the motor stalls.    
                
                      };
     };
 digitalWrite(sweepEnable, HIGH); //LOW -->enable; HIGH -->DISABLE

};

void  moveWinder(float fraction)
{
int i;
  digitalWrite(windingEnable, LOW); //LOW -->enable; HIGH -->DISABLE
  

        for (i = 0; i<fraction*windingStepsPerRev; i++)       // Iterate for 4000 microsteps. 
               {                       
                    digitalWrite(windingStep, LOW);  // This LOW to HIGH change is what creates the
                    digitalWrite(windingStep, HIGH); // "Rising Edge" so the easydriver knows to when to step.
                    delayMicroseconds(windingDelayTime);      // This delay time is close to top speed for this   // Too fast-->> the motor stalls.    
                
                      };
 digitalWrite(windingEnable, HIGH); //LOW -->enable; HIGH -->DISABLE

};

void startStop()
{
  static unsigned long last_startStop_time = 0; unsigned long startStop_time = millis();
  if (startStop_time - last_startStop_time > 300) {paused=!paused;last_startStop_time = startStop_time;}// debouncing noise.
};



void opticalCounter()
{
  static unsigned long last_interrupt_time = 0; unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > 200) {counterV++;last_interrupt_time = interrupt_time;}
}
