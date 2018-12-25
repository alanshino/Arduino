#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11,10, 5, 4, 3, 2);
byte shape[8][8]={
{B11111,B11111,B11111,B11111,B11111,B11111,B11111,B11111}, //0
{B00000,B00000,B00000,B00000,B00000,B00000,B00000,B00000}, //1
{B11111,B11111,B00000,B00000,B00000,B00000,B00000,B00000}, //2
{B00000,B00000,B00000,B00000,B00000,B00000,B11111,B11111}, //3
{B11111,B11111,B00000,B00000,B00000,B00000,B00000,B11111}, //4
{B11111,B00000,B00000,B00000,B00000,B00000,B11111,B11111}, //5
{B11111,B00000,B00000,B00000,B00000,B00000,B00000,B00000}, //6
{B00000,B00000,B00000,B00000,B00000,B00000,B00000,B11111}}; //7
const char number[10][6]={
{0,2,0,0,3,0},{1,0,1,1,0,1},{4,4,0,0,5,5},{2,4,0,3,5,0},{0,7,0,6,6,0},
{0,4,4,5,5,0},{0,4,4,0,5,0},{2,2,0,1,1,0},{0,4,0,0,5,0},{0,4,0,5,5,0}};
void setup() 
{
  lcd.begin(16,2); 
  for(int i=0;i<8;i++)
     lcd.createChar(i,shape[i]);   
}   
void loop() 
{
  for(int i=0;i<10;i++)
  {
    showNumber(i,6);
    delay(1000);
  }  
}
void showNumber(int value,int colPosition)
{
  int i;
  lcd.setCursor(colPosition,0);
  for(i=0;i<=2;i++)
     lcd.write(byte(number[value][i]));
  lcd.setCursor(colPosition,1);  
  for(i=3;i<=5;i++)
     lcd.write(byte(number[value][i]));
 }
