//EEPROM READER
#include <EEPROM.h>

int addr=0;
byte value;


void setup() {
  
  Serial.begin(9600);
}

void loop() {
  value=EEPROM.read(addr);
  if((addr+3)%3==0){
    Serial.print("temp level = ");
    Serial.println(value,DEC);
  }
  else if((addr+3)%3==1){
    Serial.print("sound level = ");
    Serial.println(value,DEC);
  }
  else{
    Serial.print("light level = ");
    Serial.println(value,DEC);
  }
  addr++;
  if(addr==EEPROM.length()){
    addr=0;
  }
  delay(100);    
  }

