#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11,10, 5, 4, 3, 2);
byte yearChar[8]={B10000,B11111,B00010,B01111,B01010,B11111,B00010,B00000};
byte moonChar[8]={B01111,B01001,B01111,B01001,B01111,B01001,B11101,B00000};
byte dayChar[8]={B01111,B01001,B01001,B01111,B01001,B01001,B01111,B00000};
int years=2013;
byte moons=8;
byte days=5;
void setup() 
{
  lcd.begin(16,2); 
  lcd.createChar(0,yearChar);
  lcd.createChar(1,moonChar);
  lcd.createChar(2,dayChar);  
}   
void loop() 
{
  lcd.setCursor(0,0);
  lcd.print(years);
  lcd.write(byte(0)); 
  lcd.print(moons);
  lcd.write(byte(1));
  lcd.print(days);
  lcd.write(byte(2));
}
