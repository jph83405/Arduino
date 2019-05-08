
//Light sensor - A0
//Temp sensor - A1
//Sound sensor - A2

//nominal light level = 1023.0
//nominal temp level = 755.0
//

#include <EEPROM.h>


//int time = 0;

int tempDetected;
int lightDetected;
int soundDetected;
int addr=EEPROM.length()-2;


  
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(10000);
  addr=0;
  
 // for(int i = 0;i<EEPROM.length();i++){
   // EEPROM.write(i,0);
  //}
  
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  pinMode(11,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(9,OUTPUT);
}

void loop() {
  //Serial.println(EEPROM.length());
  tempDetected=0;
  lightDetected=0;
  soundDetected=0;
  //Serial.println("HERE");
  digitalWrite(9,LOW);
  digitalWrite(10,LOW);
  digitalWrite(11,LOW);
  
  
  double lightLevel=(double)analogRead(A0);
  delay(30);
  //Serial.print("light level = ");
  //Serial.println(lightLevel);
  if(lightLevel>800){
    lightDetected=1;
    Serial.println("light detected");
  }
  else{
    Serial.println("no light detected");
  }

  double tempLevel=(double)analogRead(A1);
  delay(30);
  //Serial.print("temp level = ");
  //Serial.println(tempLevel);
  if(tempLevel>900){
    tempDetected=1;
    Serial.println("temp detected");
  }
  else{
    Serial.println("no temp detected");
  }

  double soundLevel=(double)analogRead(A2);
  delay(30);
  //Serial.print("sound level = ");
  //Serial.println(soundLevel);
  if(soundLevel>600){
    soundDetected=1;
    Serial.println("sound detected");
  }
  else{
    Serial.println("no sound detected");
  }
  Serial.println("");


  if(soundDetected&&tempDetected&&lightDetected){
    digitalWrite(9,HIGH);
  }
  else if(tempDetected&&lightDetected&&!soundDetected){
    digitalWrite(10,HIGH);
  }
  else if(tempDetected&&!lightDetected&&soundDetected){
    digitalWrite(10,HIGH);
  }
  else if(!tempDetected&&lightDetected&&soundDetected){
    digitalWrite(10,HIGH);
  }
  else if(tempDetected&&!lightDetected&&!soundDetected){
    digitalWrite(11,HIGH);
  }
  else if(!tempDetected&&lightDetected&&!soundDetected){
    digitalWrite(11,HIGH);
  }
  else if (!tempDetected&&!lightDetected&&soundDetected){
    digitalWrite(11,HIGH);
  }
  else{
    digitalWrite(11,HIGH);
  }
  EEPROM.write(addr,lightLevel);
  addr++;
  EEPROM.write(addr,tempLevel);
  addr++;
  EEPROM.write(addr,soundLevel);
  addr++;
  if(addr==EEPROM.length()){
    addr=0;
  } 
  //delay(1000);
  //time++;
  //Serial.print("time=");
  //Serial.println(time);
    
  
}


