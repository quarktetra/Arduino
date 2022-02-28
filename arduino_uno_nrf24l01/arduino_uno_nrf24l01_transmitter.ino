// Visit https://tetraquark.netlify.com/  for more projects
//A simple code to transmit joystick x and y values with Arduino Uno and nrf24l01 
// get the librarry here: https://github.com/nRF24/RF24
// will work with the "arduino joystick shield with nrf24l01"
// NRF Pin    Arduino Pin
//   Vcc         3.3V       DO NOT connect to 5V, it will damage the transmittet unit!
//   CS          10        
//   MOSI        11
//   GND         GND
//   CE            9
//   CSN           10
//   SCK         13
//   MISO        12
// Note that CE and CS pins can be changed, if needed.
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#define CE_PIN 9    // these are the default connections for the joystick shield
#define CSN_PIN 10  // these are the default connections for the joystick shield
#define JOYSTICK_X A0     /// Connect Joystick output 1 to A0
#define JOYSTICK_Y A1     /// Connect Joystick output 1 to A1
const uint64_t pipe = 0xE8E8F0F0E1LL;
RF24 radio(CE_PIN, CSN_PIN); 
int joystick[2];// this will store x and y components
void setup()
    {
        Serial.begin(9600);
        radio.begin();
        radio.openWritingPipe(pipe);
    }
void loop()
    {
        joystick[0] = analogRead(JOYSTICK_X);joystick[1] = analogRead(JOYSTICK_Y);// reading the input voltage as an 8 bit number
        radio.write( joystick, sizeof(joystick) );// transmitting the data
        Serial.print("Sending:  X="); Serial.print(joystick[0]); Serial.print("; Y="); Serial.println(joystick[1]);// serial port print for debug purposes
    }
