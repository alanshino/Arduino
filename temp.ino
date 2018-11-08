#include <LiquidCrystal.h>       //調用Arduino自帶的LiquidCrystal庫
#include <OneWire.h>      //調用Arduino單總線庫
#include <DallasTemperature.h>  //調用ds18b20溫度傳感器庫

//#define ONE_WIRE_BUS 2    //設置單總線數據I/O口為2號引腳
#define ONE_WIRE_BUS 7

OneWire temp(ONE_WIRE_BUS);  //初始化一個單總線類，以2號引腳作為輸出口
DallasTemperature sensors(&temp); //初始化一個溫度傳感器類。
//LiquidCrystal lcd(12, 11, 9, 8, 7, 6);  //初始化一個LiquidCrystal，設置相應的接口，其中RS為12號引腳，E為11號引腳，9~6為數據輸入引腳
LiquidCrystal lcd(12, 11,10, 5, 4, 3, 2);

float temperature = 0;      //設置temperature為浮點變量
float setTmp = 27;
const byte swUp = 14;
void setup()
{
  Serial.begin(9600);       //初始化串口
  sensors.begin();        //初始化溫度傳感器
  lcd.begin(16, 2);       //初始化LCD
  pinMode(swUp,INPUT_PULLUP);
}
 
void loop ()                     
{ 
  if(digitalRead(swUp)==LOW){
    delay(20);
    while(digitalRead(swUp)==LOW)
      ;
    setTmp+=0.5;
  }
  sensors.requestTemperatures();  //對傳感器進行一個溫度請求
  temperature=sensors.getTempCByIndex(0);  //讀取傳感器輸出的數據，以攝氏度為單位賦值給temperature變量。
  delay(100);
  lcd.clear();          //清屏
  lcd.print("ST:");   //使屏幕顯示文字
  lcd.print( setTmp,1);  //顯示溫度小數點後一位
  lcd.print((char)223);      //顯示o符號
  lcd.print("C");      //顯示字母C
  
  lcd.setCursor(0, 1) ;
  lcd.print("CT:");   //使屏幕顯示文字
  lcd.print( temperature,1);  //顯示溫度小數點後一位
  lcd.print((char)223);      //顯示o符號
  lcd.print("C");      //顯示字母C
  
  
  if(temperature>setTmp+1){
    analogWrite(8,255);
    analogWrite(9,0);
    lcd.print(" Cool");
  }
  else if(temperature<setTmp){
    analogWrite(8,0);
    analogWrite(9,255);
    lcd.print(" Heat");
  }
  else{
    analogWrite(8,0);
    analogWrite(9,0);
    lcd.print("      ");
  }  
  //delay(1000);           //延時1秒，刷新速度。
}

