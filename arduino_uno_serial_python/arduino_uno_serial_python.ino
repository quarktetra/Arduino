
int VoltageSensorPin = A0;  // This will measure the capacitor voltage, which is the same as the coil voltage. 
int CurrentSensorPin = A1;  // The output put of the ACS712 module. Centered around  512 or so when there is no current.  1A corresponds to 36 units per calibration.
int TriggerSensorPin = A2;  // This will go down to zero as the switch is closed. Will use this to set t=0                 
int VoltageSensorValue;     
int CurrentSensorValue;     
int TriggerSensorValue;     

void setup() {
  Serial.begin(250000); // This should match the value in Python if data is read from that side. 250,000 is fast enough to sample once per millisecond.
  Serial.print("time_ms"); Serial.print(",");Serial.print("Voltage"); Serial.print(",");Serial.print("Current"); Serial.print(",");Serial.println("Trigger"); //match the Header in Python.
}

int time0=millis();
void loop() {
  VoltageSensorValue = analogRead(VoltageSensorPin);
  CurrentSensorValue = analogRead(CurrentSensorPin);
  TriggerSensorValue = analogRead(TriggerSensorPin);
  Serial.print(millis()-time0); Serial.print(",");Serial.print(VoltageSensorValue); Serial.print(",");Serial.print(CurrentSensorValue);; Serial.print(",");Serial.println(TriggerSensorValue);
}
