#define TRIGPIN 2
#define ECHOPIN 3
#define LED1 12
#define LED2 13
const int speaker=7;
char beeTone[]="GEEFDDCDEFGGGGEEFDDCEGGEDDDDDEFEEEEEFGGEEFDDCEGGC";
char starTone[]="CCGGAAGFFEEDDCGGFFEEDGGFFEEDCCGGAAGFFEEDDC";
int i,j;
byte beeBeat[]={1,1,2,1,1,2,1,1,1,1,1,1,2,
1,1,2,1,1,2,1,1,1,1,4,
 1,1,1,1,1,1,2,1,1,1,1,1,1,2,
1,1,2,1,1,2,1,1,1,1,4};
byte starBeat[]={1,1,1,1,1,1,2,1,1,1,1,1,1,2, //小星星節拍。
 1,1,1,1,1,1,2,1,1,1,1,1,1,2,
 1,1,1,1,1,1,2,1,1,1,1,1,1,2};
unsigned int frequency[7]={523,587,659,694,784,880,988};//音符頻率。
char toneName[]="CDEFGAB";
const int beeLen=sizeof(beeTone); //小蜜蜂音符總數。
const int starLen=sizeof(starTone);
unsigned long tempo=180; //每分鐘180 拍
int num; 
  
 long ping() { 
   digitalWrite(TRIGPIN, LOW); 
   delayMicroseconds(2); 
   digitalWrite(TRIGPIN, HIGH); 
   delayMicroseconds(10); 
   digitalWrite(TRIGPIN, LOW); 
   return pulseIn(ECHOPIN, HIGH)/58; 
 } 
  
 void setup() {
   Serial.begin(9600);
   pinMode(TRIGPIN, OUTPUT); 
   pinMode(ECHOPIN, INPUT); 
   pinMode(LED1, OUTPUT); 
   pinMode(LED2, OUTPUT); 
 } 
  
 void loop() { 
   long cm = ping(); 
   Serial.println(cm);
    
   if (cm < 30) { 
     digitalWrite(LED1, HIGH); 
     delay(cm*1.5 + 10); 
     digitalWrite(LED1, LOW); 
     delay(1000);
     digitalWrite(LED2, HIGH); 
     delay(cm*1.5 + 10); 
     digitalWrite(LED2, LOW); 
     for(i=0;i<beeLen;i++){
        playTone(beeTone[i],beeBeat[i]); 
        cm = ping(); 
         Serial.println(cm);
         if (cm>15){
            break;
         }
     }
   } else if (cm>30&&cm<50){
      digitalWrite(LED2, HIGH); 
      delay(cm*1.5 + 10); 
      digitalWrite(LED2, LOW); 
     for(i=0;i<starLen;i++) {
      playTone(starTone[i],starBeat[i]);
      cm = ping(); 
       Serial.println(cm);
       if (cm<15){
          break;
       }
     }
   }else{
      tone(speaker,500);
      delay(5000);
      noTone(speaker);
   }
   delay(100); 
 } 
 void playTone(char toneNo,byte beatNo) //播放音符函式。
{
   unsigned long duration=beatNo*60000/tempo; //計算每拍時間（毫秒）。
   for(j=0;j<7;j++)
   {
      if(toneNo==toneName[j]) //查音符表。
   {
   tone(speaker,frequency[j]); //播放音符。
   delay(duration); //此音符的節拍。
   noTone(speaker); //關閉聲音。
  }
 }
} 
