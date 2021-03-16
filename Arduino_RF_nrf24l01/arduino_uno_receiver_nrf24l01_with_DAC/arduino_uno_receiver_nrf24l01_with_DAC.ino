// Find details: https://tetraquarkbeta.netlify.app/post/wheelchair/wheelchair/
//A simple code to receive joystick x and y values with Arduino Uno and nrf24l01
// and to convert the values to Voltages using PWM (2.2K resistor and 33uF capacitor as a low pass filter will do the trick)
// note the CS and CE pin connections
// NRF Pin    Arduino Pin   
//   Vcc         3.3V       DO NOT connect to 5V, it will damage the transmittet unit!
//   CS          8          
//   MOSI        11
//   GND         GND
//   CE          7 
//   SCK         13
//   MISO        12
// Note that CE and CS pins can be changed, if needed.
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#define CE_PIN 7
#define CSN_PIN 8
const uint64_t pipe = 0xE8E8F0F0E1LL;
RF24 radio(CE_PIN, CSN_PIN);
int joystick[2];// will store joystick doublet: first entry is the X value, second is the Y value
int lout= 3;// Will light up an LED when signal is received-very useful for debugging
int yout= 5; //PWM output for X value
int xout= 6;//PWM output for Y value



float central_valuex =  511.0; // this should be 512 theoretically but it may deviate from it a bit
float central_valuey =  502.0; // this should be 512 theoretically but it may deviate from it a bit

/// Joystick values are 10 bit numbers ranging from 0-1023
/// We will map these numbers a range so that when converted to voltage we get Vmin and Vmax
 //Vmax=4V for this example. We also assume center voltage is 2.5. The voltage will range from 1V to 4V in this example, i.e 1.5V swing
float Vmax=4.00;
float mindelta=10;// incoming joystick values may fluctuate a bit and more importantly as you try move the
// joystick along one axis only it may move on the other axis a bit as well. We will put a threshold for
// minimum deviation to clean out unintentional movement


int debugmode=1; /// use this to turn serial commumication on and off

int joyx;//the number to send to the PWM output. Min value is dictated by Vmin, max is by Vmax
int joyy;
void setup()
{
  pinMode(lout, OUTPUT);
     analogWrite(lout,0); ;
  Serial.begin(9600);
  delay(100);
  Serial.println("Nrf24L01 Receiver Starting");
  radio.begin();
  radio.openReadingPipe(1,pipe);
  radio.startListening();
}

void loop()
{
  if(radio.available())
    {   
         analogWrite(lout,255); 
      radio.read( joystick, sizeof(joystick) );
      /// calculate joyx and joyy
      if (abs(joystick[0]-central_valuex)<mindelta)
      {joyx=128;}// if the joystickx did not deviate enough from mid point set output to 128, which is 2.5V
      else{joyx=128+127*(joystick[0]-central_valuex)*((Vmax-2.5)/2.5)/(1023-central_valuex);}
      
      if (abs(joystick[1]-central_valuey)<mindelta)
      {joyy=128;}// if the joysticky did not deviate enough from mid point set output to 128, which is 2.5V
          else{joyy=128+127*(joystick[1]-central_valuey)*((Vmax-2.5)/2.5)/(1023-central_valuey);}
      
      analogWrite(xout, joyx);analogWrite(yout, joyy); // send the final results to the ports
      if(debugmode==1){
      //Serial.print("Received: X=");Serial.print(joystick[0]); Serial.print("; Y=");Serial.print(joystick[1]);
      Serial.print("; Converted to: Xpwm=");Serial.print(joyx);Serial.print("; Ypwm=");Serial.println(joyy);
      }
    } 
    else {delay(300);Serial.println("No radio");   analogWrite(lout,0); ;analogWrite(yout, 128); analogWrite(xout, 128);}//wait 0.2 sec before deciding there is no radio
}
