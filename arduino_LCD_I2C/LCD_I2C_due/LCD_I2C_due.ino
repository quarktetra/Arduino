#include <LiquidCrystal_I2C.h>
//get the library here http://www.esp32learning.com/wp-content/uploads/2017/12/LiquidCrystal_I2C-master.zip
LiquidCrystal_I2C lcd(0x27,20,4); // set the LCD address to 0x3F for a 16 chars and 2 line display
// set the LCD address to 0x27 for a 20 chars 4 line display
// Set the pins on the I2C chip used for LCD connections:
//                   addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
void setup()
{
lcd.init(); // initialize the lcd
// Print a message to the LCD.
lcd.backlight();
lcd.setCursor(0,0);//the first argument marks column, the second  marks the row
lcd.print("Deniz hohoho");
lcd.setCursor(0,1);
lcd.print("Mavi");

}
 
void loop()
{
 
}
