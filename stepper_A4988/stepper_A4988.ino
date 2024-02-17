/*     Simple Stepper Motor Control Exaple Code
 *      
 *  by Dejan Nedelkovski, www.HowToMechatronics.com
 *  
 */

// defines pins numbers
const int stepPin = 3; 
const int dirPin = 4; 
const int span=2000;
 
void setup() {
  // Sets the two pins as Outputs
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
}
void loop() {
  //digitalWrite(dirPin,HIGH); // Enables the motor to move in a particular direction
  digitalWrite(dirPin,LOW);
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < span; x++) {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(5000); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(5000); 
  }
  //delay(200); // One second delay
  
  //digitalWrite(dirPin,LOW); //Changes the rotations direction
  // Makes 400 pulses for making two full cycle rotation


   // delay(20); // One second delay
}
