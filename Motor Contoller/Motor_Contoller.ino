/*
 * Code controls an h-bridge for Power Electronics Project.
 * Functions for PWM control added.
 * Feedback could also be done to contorl motor speed.
 */



void setup() {
Serial.begin(9600);
pinMode(3,OUTPUT);
pinMode(4,OUTPUT);
pinMode(5,OUTPUT);
pinMode(6,OUTPUT);
Serial.println("CONFIGURED");
}

void loop() {
  spin_motor_CW();
  delay(5000);
  stop_motor();
  delay(1000);
  spin_motor_CCW();
  delay(5000);
  stop_motor();
  delay(1000);
}

void stop_motor(){
  digitalWrite(4,HIGH);//PNP high side off
  digitalWrite(6,LOW);//NPN low side off
  digitalWrite(3,HIGH); //PNP high side off
  digitalWrite(5,LOW); //NPN low side off
  Serial.println("stop motor");
}

void spin_motor_CW(){
  digitalWrite(4,HIGH);//PNP high side off
  digitalWrite(6,LOW);//NPN low side off
  digitalWrite(3,LOW); //PNP high side on
  digitalWrite(5,HIGH); //NPN low side on
  Serial.println("Motor spinning CW");
}


void spin_motor_CCW(){
  digitalWrite(4,LOW);//PNP high side on
  digitalWrite(6,HIGH);//NPN low side on
  digitalWrite(3,HIGH); //PNP high side off
  digitalWrite(5,LOW); //NPN low side off
  Serial.println("Motor spinning CCW");
}

void variable_spin_CW(int spin){
  digitalWrite(4,HIGH);//PNP high side off
  digitalWrite(6,LOW);//NPN low side off
  analogWrite(3,spin); //PNP high side on with variable duty cycle
  digitalWrite(5,HIGH); //NPN low side on
}

void variable_spin_CCW(int spin){
  analogWrite(4,spin);//PNP high side on
  digitalWrite(6,HIGH);//NPN low side on
  digitalWrite(3,HIGH); //PNP high side off
  digitalWrite(5,LOW); //NPN low side off
}


