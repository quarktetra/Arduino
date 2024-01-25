
int sweepDir = 11;
int sweepStep = 12;
int sweepEnable = 13;

int windingDir = 8;
int windingStep = 9;
int windingEnable = 10;


int sweepDelayTime=300; //in micro sec
int windingDelayTime=300; //in micro sec

float cHeigth=12; //mm
float wDiameter=0.48;
float sweepRots=cHeigth/wDiameter;
  
float sweepStepsPerRev=wDiameter*1.01*1600;



float windingStepsPerRev=2.09*1600; // 2*1600 gives a full rev

void setup() {
Serial.begin(9600);


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
}

void loop()
{


     delay(5000);
     digitalWrite(sweepEnable, LOW); //enable the motor
     digitalWrite(windingEnable, LOW); //enable the motor
     digitalWrite(sweepDir, HIGH);     

 int j;int i;

 
  for (j = 0; j<sweepRots; j++)      {
Serial.print("LAYER1; Turn");Serial.print(j);Serial.print(" out of ");Serial.println(sweepRots);
      for (i = 0; i<windingStepsPerRev; i++)       // Iterate for 4000 microsteps. 
  {
      digitalWrite(windingStep, LOW);  // This LOW to HIGH change is what creates the
      digitalWrite(windingStep, HIGH); // "Rising Edge" so the easydriver knows to when to step.
      delayMicroseconds(windingDelayTime);      // This delay time is close to top speed for this   // Too fast-->> the motor stalls.    
  } 
  for (i = 0; i<sweepStepsPerRev; i++)       // Iterate for 4000 microsteps. 
  {
      digitalWrite(sweepStep, LOW);  // This LOW to HIGH change is what creates the
      digitalWrite(sweepStep, HIGH); // "Rising Edge" so the easydriver knows to when to step.
      delayMicroseconds(sweepDelayTime);      // This delay time is close to top speed for this   // Too fast-->> the motor stalls.    
  } 
    
};

     delay(1000);
     digitalWrite(sweepDir, LOW);     // Set the Wdirection.

  for (j = 0; j<sweepRots; j++)      {

Serial.print("LAYER2; Turn");Serial.print(j);Serial.print(" out of ");Serial.println(sweepRots);
      for (i = 0; i<windingStepsPerRev; i++)       // Iterate for 4000 microsteps. 
  {
      digitalWrite(windingStep, LOW);  // This LOW to HIGH change is what creates the
      digitalWrite(windingStep, HIGH); // "Rising Edge" so the easydriver knows to when to step.
      delayMicroseconds(windingDelayTime);      // This delay time is close to top speed for this   // Too fast-->> the motor stalls.    
  }
  
  for (i = 0; i<sweepStepsPerRev; i++)       // Iterate for 4000 microsteps. 
  {
      digitalWrite(sweepStep, LOW);  // This LOW to HIGH change is what creates the
      digitalWrite(sweepStep, HIGH); // "Rising Edge" so the easydriver knows to when to step.
      delayMicroseconds(sweepDelayTime);      // This delay time is close to top speed for this   // Too fast-->> the motor stalls.    
  } 
   delay(100);


   
                            
};



 

 digitalWrite(sweepEnable, HIGH); //disable the motor
    digitalWrite(windingEnable, HIGH); //disable the motor  

 delay(100000);
 
 
}
