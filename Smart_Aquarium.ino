// 匯入程式庫標頭檔
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 2  // Arduino數位腳位2接到1-Wire裝置
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
#include <Wire.h>
#include <LiquidCrystal_I2C.h>           // 序列式 LCD 介面程式庫
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // 設定 LCD I2C 位址
#include <SoftwareSerial.h>
SoftwareSerial BT(8, 7); // 接收, 傳送
float temp1 = 0;           //變數宣告
int tmp;         //變數宣告
const byte led = 3;                     //腳位定義
const byte het = 11;                     //腳位定義
const byte fan = 10;                     //腳位定義
const byte SETT = 4;      //腳位宣告
const byte SETU = 5;      //腳位宣告
const byte SETD = 6;      //腳位宣告
const byte mfed = 12;                     //腳位宣告
const byte bzr = 9;                     //腳位宣告
int timer[] = {10, 0, 0, 8, 16, 30};
int ik,  ss = 0, mm = 0, hh = 0;
int lii, lii2;
String msg = "";
int si = 1, sk = 0, it, fed = 0, lli = 0, sok;
void setup(void)
{
  pinMode(led, OUTPUT);   //設定腳位方向
  pinMode(het, OUTPUT);   //設定腳位方向
  pinMode(fan, OUTPUT);   //設定腳位方向
  pinMode(SETT, INPUT);       //設定腳位方向
  pinMode(SETU, INPUT);       //設定腳位方向
  pinMode(SETD, INPUT);       //設定腳位方向
  pinMode(mfed, OUTPUT);   //設定腳位方向
  pinMode(bzr, OUTPUT);   //設定腳位方向

  digitalWrite(led, LOW);  //led關
  digitalWrite(het, HIGH);  //加熱關
  digitalWrite(fan, LOW);   //風扇關
  digitalWrite(SETT, HIGH);   //設HIGH
  digitalWrite(SETU, HIGH);   //設HIGH
  digitalWrite(SETD, HIGH);   //設HIGH
  digitalWrite(mfed, LOW);   //餵食馬達停
  digitalWrite(bzr, HIGH);  //蜂鳴器停

  delay(1000);
  BEE();
  BT.begin(9600);             //RS232通訊鮑率
  lcd.begin(16, 2);           // 初始化 LCD
  lcd.setCursor(0, 0);        //lcd顯示在第1行第1字
  lcd.print("Wifi connecting ");
  BT.print("test"); delay(300);  //設定wifi帳號及密碼
  BT.print("12345678");
  sok = 0;
  while (sok == 0)            //等待wifi連線
  {
    if ( BT.available()) {           //收到資料時
      if ( BT.find("WIFI CONNECTED")) {   //WIFI模組傳來WIFI CONNECTED時，表示已連到WIFI分享器
        sok = 1;                       //sok設1
      }
    }
  }
  lcd.setCursor(0, 0);    //lcd顯示在第1行第1字
  lcd.print("  :  :          ");
  lcd.setCursor(0, 1);    //lcd顯示在第2行第1字
  lcd.print("Temp:      C    ");
  lcd.setCursor(10, 1);   //lcd顯示在第2行第11字
  lcd.write(0XDF);        //顯示度的符號
  BT.setTimeout(100);
  BT.print("rfisb04time");            //讀取firebase fisb-time欄位
  while (BT.available() == 0);      //等待資料傳進來
  msg = BT.readString();            //讀出字串給msg
  msg.trim();                       //消去字串的0d 0a
  if (msg.substring(1, 2) == "T" || msg.substring(1, 2) == "t")               //收到的字串第2字是T或t時
  {
    timer[0] =  msg.substring(2, 4).toInt();  //依序取出 目前時間 時時、分分、秒秒、定時1時時 定時2時時  溫度
    timer[1] =  msg.substring(4, 6).toInt();
    timer[2] =  msg.substring(6, 8).toInt();
    timer[3] =  msg.substring(8, 10).toInt();
    timer[4] =  msg.substring(10, 12).toInt();
    timer[5] =  msg.substring(12, 14).toInt();
    showt();      //顯示設定值在lcd上
  }
  // 初始化
  sensors.begin();          //開啟DS8B20溫度感測功能
  delay(500);
  TCCR1A = 0x00;                   // Normal mode, just as a Timer
  TCCR1B |= _BV(CS12);             // prescaler = CPU clock/1024
  TCCR1B &= ~_BV(CS11);
  TCCR1B |= _BV(CS10);
  TIMSK1 |= _BV(TOIE1);            // enable timer overflow interrupt
  TCNT1 = -15625;                  // Ticks for 1 second @16 MHz,prescale=1024
}
void showt()      //顯示設定值
{
  if (timer[0] > 23)timer[0] = 0; if (timer[1] > 59)timer[1] = 0; if (timer[2] > 59)timer[2] = 0;
  if (timer[0] < 0)timer[0] = 23; if (timer[1] < 0)timer[1] = 59; if (timer[2] < 0)timer[2] = 59;
  lcd.setCursor(0, 0);    //lcd顯示在第1行第1字
  lcd.write(timer[0] / 10 + 0x30);
  lcd.write((timer[0] % 10) + 0x30);
  lcd.setCursor(3, 0);    //lcd顯示在第1行第4字
  lcd.write(timer[1] / 10 + 0x30);
  lcd.write((timer[1] % 10) + 0x30);
  lcd.setCursor(6, 0);    //lcd顯示在第1行第7字
  lcd.write(timer[2] / 10 + 0x30);
  lcd.write((timer[2] % 10) + 0x30);

  if (timer[3] > 23)timer[3] = 0; if (timer[4] > 23)timer[4] = 0;
  if (timer[3] < 0)timer[3] = 23; if (timer[4] < 0)timer[4] = 23;
  lcd.setCursor(10, 0);    //lcd顯示在第1行第11字
  lcd.write(timer[3] / 10 + 0x30);
  lcd.write((timer[3] % 10) + 0x30);
  lcd.setCursor(14, 0);    //lcd顯示在第1行第15字
  lcd.write(timer[4] / 10 + 0x30);
  lcd.write((timer[4] % 10) + 0x30);

  if (timer[5] > 40)timer[5] = 20;
  if (timer[5] < 20)timer[5] = 40;
  lcd.setCursor(14, 1);    //lcd顯示在第2行第15字
  lcd.write(timer[5] / 10 + 0x30);
  lcd.write((timer[5] % 10) + 0x30);
}
void sowcu(void)       //設定LCD游標位置
{
  switch (it)
  {
    case 0:         // 若0 為第1行第1字
      lcd.setCursor(0, 0);
      break;
    case 1:        // 若1 為第1行第4字
      lcd.setCursor(3, 0);
      break;
    case 2:        // 若2 為第1行第7字
      lcd.setCursor(6, 0);
      break;
    case 3:        // 若3 為第1行第11字
      lcd.setCursor(10, 0);
      break;
    case 4:        // 若4 為第1行第15字
      lcd.setCursor(14, 0);
      break;
    case 5:        // 若5 為第2行第15字
      lcd.setCursor(14, 1);
      break;

  }
}
void settime(void)      //設定時間及定時功能
{
  int sok = 0;
  it = 0;
  while (sok == 0)
  {
    if (digitalRead(SETU) == LOW) //按下設定+鍵時
    {
      timer[it] = timer[it] + 1;  //設定值加1
      showt();                    //顯示設定值
      BEE();
      while (digitalRead(SETU) == LOW); //等待放開按鍵
      sowcu();     //設定LCD游標位置
      delay(100);
    }
    if (digitalRead(SETD) == LOW) //按下設定-鍵時
    {
      timer[it] = timer[it] - 1;  //設定值減1
      showt();                   //顯示設定值
      BEE();
      while (digitalRead(SETD) == LOW); //等待放開按鍵
      sowcu();     //設定LCD游標位置
      delay(100);
    }

    if (digitalRead(SETT) == LOW) //按下設定鍵時
    {
      BEE();
      while (digitalRead(SETT) == LOW); //等待放開按鍵
      it++;           //陣列位置加1
      sowcu();   //設定LCD游標位置
      delay(100);
      if (it > 5)
      {
        sok = 1; //加到6時 設定完成
      }
    }
  }
  lcd.noBlink();     //不顯示游標

}
void BEE()
{
  digitalWrite(bzr, LOW); delay(30); digitalWrite(bzr, HIGH); //蜂鳴器響
}
void loop(void)
{
  for (ik = 0; ik < 200; ik++) //200次迴圈
  {
    if (BT.available())
    {
      msg = BT.readString();                      //讀出字串給msg
      msg.trim();                                 //消去字串的0d 0a
      if (msg.substring(1, 2) == "T")             //收到的字串第2字是T時
      {
        timer[0] =  msg.substring(2, 4).toInt();  //依序取出 目前時間 時時、分分、秒秒、定時1時時 定時2時時  溫度
        timer[1] =  msg.substring(4, 6).toInt();
        timer[2] =  msg.substring(6, 8).toInt();
        timer[3] =  msg.substring(8, 10).toInt();
        timer[4] =  msg.substring(10, 12).toInt();
        timer[5] =  msg.substring(12, 14).toInt();
        BT.print("w15fisb04time");                //寫入資料到firebase 的fisb-time欄位 把資料的T變t 表示已讀出設定值
        BT.print("\"t");
        BT.print(msg.substring(2, 15));
        showt();     //顯示設定值在lcd上
      }
      delay(50);
      BT.print("w02fisb04data");          //寫入目前溫度到firebase 的fisb-data欄位
      BT.write(tmp / 10 + 0x30); //小時
      BT.write(tmp % 10 + 0x30);
    }
    if (digitalRead(SETT) == LOW) //按下設定鍵時
    { BEE();
      while (digitalRead(SETT) == LOW); //等待放開按鍵
      delay(100);
      sk = 1;
      lcd.setCursor(0, 0);
      lcd.blink();        //顯示游標
      settime();     //設定時間及定時功能
      sk = 0;
    }
    delay(5);//延時，單位mS
    lli++;
    if (lli > 20)
    {
      lli = 0;
      lii = analogRead(A0);   //可變電阻值
      lii2 = map(lii, 0, 1024, 0, 255); //感光值0~1024  對應成調光值0~255
      analogWrite(led, lii2);   //輸出調光訊號
    }
    if (si == 1)       //1秒時間到，顯示時鐘
    {
      lcd.setCursor(0, 0);
      lcd.write(timer[0] / 10 + 0x30); //小時
      lcd.write(timer[0] % 10 + 0x30);
      lcd.setCursor(3, 0);
      lcd.write(timer[1] / 10 + 0x30); //分
      lcd.write(timer[1] % 10 + 0x30);
      lcd.setCursor(6, 0);
      lcd.write(timer[2] / 10 + 0x30); //秒
      lcd.write(timer[2] % 10 + 0x30);
      si = 0;
      if (fed > 0)      //若已到鬧鐘時間，fed會被設定成>0
      {
        BEE();
        fed--;          //fed減1
        if (fed == 0 ) //減到0 時
        {
          digitalWrite(mfed, LOW);  //餵食馬達停
        }
      }
      if (timer[0] == timer[3] && timer[1] == 0 && timer[2] == 0) //餵食時段1 時間到
      {
        fed = 5; //設成5秒的餵食作動時間
        digitalWrite(mfed, HIGH); //餵食馬達啟動
      }
      if (timer[0] == timer[4] && timer[1] == 0 && timer[2] == 0) //餵食時段2 時間到
      {
        fed = 5; //設成5秒的餵食作動時間
        digitalWrite(mfed, HIGH); //餵食馬達啟動
      }
      BT.print("rfisb04time");            //讀取firebase fisb-time欄位
    }
  }
  sensors.requestTemperatures();
  // 取得溫度讀數（攝氏）並輸出，
  // 參數0代表匯流排上第0個1-Wire裝置
  temp1 = sensors.getTempCByIndex(0);
  tmp = temp1;
  lcd.setCursor(5, 1);        //lcd顯示在第1行第6字
  lcd.print(temp1);           //顯示溫度
  if (temp1 > timer[5] + 0.5)     //當溫度大於設定值+0.5時
  {
    digitalWrite(fan, HIGH);    //風扇開
    digitalWrite(het, HIGH);    //加熱關
  }
  else if (temp1 < timer[5] - 0.5)     //當溫度小於設定值-0.5時
  {
    digitalWrite(fan, LOW);    //風扇關
    digitalWrite(het, LOW);    //加熱開
  }
  else
  {
    digitalWrite(fan, LOW);    //風扇關
    digitalWrite(het, HIGH);    //加熱關
  }
}

ISR (TIMER1_OVF_vect)
{
  TCNT1 = -15625;               // Ticks for 1 second @16 MHz,prescale=1024
  if (sk == 0)
  {
    si = 1;
    timer[2] = timer[2] + 1;    //時鐘秒加1 超過59秒要進位
    if (timer[2] > 59)
    { timer[2] = 0;
      timer[1] = timer[1] + 1;    //時鐘分加1 超過59分要進位
      if (timer[1] > 59)
      { timer[1] = 0;
        timer[0] = timer[0] + 1;    //時鐘時加1 超過23時要進位
        if (timer[0] > 23)
        {
          timer[0] = 0;
        }
      }
    }
  }
}

