
int VoltageSensorPin = A0;  // This will measure the capacitor voltage, which is the same as the coil voltage. 
int CurrentSensorPin = A1;  // The output put of the ACS712 module. Centered around   
int TriggerSensorPin = A2;  // The potentiometer on pin 1                  
int VoltageSensorValue;     
int CurrentSensorValue;     
int TriggerSensorValue;     

void setup() {
  Serial.begin(250000); //115200 This should match the value in Python if data is read from that side.
  Serial.print("time_ms"); Serial.print(",");Serial.print("Voltage"); Serial.print(",");Serial.print("Current"); Serial.print(",");Serial.println("Trigger");
}

int time0=millis();
void loop() {
  VoltageSensorValue = analogRead(VoltageSensorPin);
  CurrentSensorValue = analogRead(CurrentSensorPin);
  TriggerSensorValue = analogRead(TriggerSensorPin);
  Serial.print(millis()-time0); Serial.print(",");Serial.print(VoltageSensorValue); Serial.print(",");Serial.print(CurrentSensorValue);; Serial.print(",");Serial.println(TriggerSensorValue);
}
