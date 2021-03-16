//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Find details: https://tetraquarkbeta.netlify.app/post/wheelchair/wheelchair/
// A simple code to transmit joystick x and y values with Arduino Uno and nrf24l01 
// get the librarry here: https://github.com/nRF24/RF24
// will work with the "arduino joystick shield with nrf24l01" You can find it at Ebay
// NRF Pin    Arduino Pin
//   Vcc         3.3V       DO NOT connect to 5V, it may damage the radio unit!
//   CS          10        
//   MOSI        11
//   GND         GND
//   CE           9
//   CSN         10
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
int joystick[2];
void setup()
{
Serial.begin(9600);
radio.begin();
radio.openWritingPipe(pipe);
}
void loop()
{
joystick[0] = analogRead(JOYSTICK_X);joystick[1] = analogRead(JOYSTICK_Y);
radio.write( joystick, sizeof(joystick) );
Serial.print("Sending:  X="); Serial.print(joystick[0]); Serial.print("; Y="); Serial.println(joystick[1]);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
