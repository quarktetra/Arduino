
int sweepDir = 8;
int sweepStep = 9;
int sweepEnable = 10;



int sweepDelayTime=500; //in micro sec


float sweepStepsPerRev=1600;




void setup() {
Serial.begin(9600);


pinMode(sweepDir, OUTPUT);
pinMode(sweepStep, OUTPUT);
pinMode(sweepEnable, OUTPUT);
digitalWrite(sweepDir, HIGH);     // Set the Wdirection.
digitalWrite(sweepEnable, HIGH); //disable the Wmotor

}

void loop()
{

int i;
 digitalWrite(sweepDir, HIGH);     // Set the Wdirection.
     digitalWrite(sweepEnable, LOW); //enable the motor
for (i = 0; i<sweepStepsPerRev; i++)       // Iterate for 4000 microsteps. 
  {
      digitalWrite(sweepStep, LOW);  // This LOW to HIGH change is what creates the
      digitalWrite(sweepStep, HIGH); // "Rising Edge" so the easydriver knows to when to step.
      delayMicroseconds(sweepDelayTime);      // This delay time is close to top speed for this   // Too fast-->> the motor stalls.    
  }

 
 
}
