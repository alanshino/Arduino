const int led=13;
const int sig=2;
void setup() 
{ 
  Serial.begin(9600);
  pinMode(led,OUTPUT);
  digitalWrite(led,LOW);
}
void loop() 
{
  unsigned long cm;
  pinMode(sig,OUTPUT);
  cm=ping(sig);                               // 抓取距離
  Serial.print("distance=");
  Serial.print(cm);
  Serial.println("cm");
  digitalWrite(led,HIGH);
  delay(cm*10);
  digitalWrite(led,LOW);
  delay(cm*10);
}
int ping(int sig)
{
   unsigned long cm,duration;
   pinMode(sig,OUTPUT);
   digitalWrite(sig,LOW);
   delayMicroseconds(2);
   digitalWrite(sig,HIGH);
   delayMicroseconds(5);
   digitalWrite(sig,LOW);
   pinMode(sig,INPUT);
   duration=pulseIn(sig,HIGH);
   cm=duration/29/2;
   return cm;
}  
