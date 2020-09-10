#include <Wire.h>
#include "elapsedMillis.h"
#define MPU_ADDR 0x68
#define X_ACCEL_REG 0x3B
#define Y_ACCEL_REG 0x3D
#define Z_ACCEL_REG 0x3F
#define GYRO_X_REG 0x43
#define GYRO_Y_REG 0x45
#define GYRO_Z_REG 0x47
#define INTERRUPT_PIN 2


int16_t a_x,a_y,a_z;
int16_t gyro_x,gyro_y,gyro_z;//ADC readings are 16 bit
int IMU_CHECK_INTERVAL_MSEC = 100;
elapsedMillis sinceLastIMUCheck;
volatile bool mpu_interrupt = false; 
float x_offset,y_offset,z_offset;

void set_offset(float* x,float* y,float* z){
  *x = .9;
  *y = .01;
  *z = .15;
}


void init_MPU(){
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0x08); //Disables temperature sensor. Can enable by resetting device.
  Wire.endTransmission(1);
  
}

void enable_MPU_data_ready_interrupts(){
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x38);
  Wire.write(0x01);
  Wire.endTransmission(1);
  
}



void get_register_data(uint16_t reg,int16_t* data){
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(reg);
  Wire.endTransmission(0);//Don't end transmission yet.
  Wire.requestFrom(MPU_ADDR,2,1); //Store data inside buffer in wire library
  byte MSB = Wire.read();//MPU has high register first,then low register
  byte LSB = Wire.read();
  *data = ((MSB << 8) | LSB); //16bits that represent our data from the MPU.
}


void update_data(){
  get_register_data(X_ACCEL_REG,&a_x);
  get_register_data(Y_ACCEL_REG,&a_y);
  get_register_data(Z_ACCEL_REG,&a_z);
  get_register_data(GYRO_X_REG,&gyro_x);
  get_register_data(GYRO_Y_REG,&gyro_y);
  get_register_data(GYRO_Z_REG,&gyro_z);
}

float condition_acceleration_data(int16_t* data){
  float my_data = ((float)*data/16384.0) * 9.81;
  return my_data;
}



void setup() {
  Serial.begin(9600);
  Wire.begin();
  init_MPU();
  enable_MPU_data_ready_interrupts();
  pinMode(INTERRUPT_PIN, INPUT_PULLUP);
  Serial.println("hello");
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmp_data_ready_ISR, LOW);
  set_offset(&x_offset,&y_offset,&z_offset);
}

void dmp_data_ready_ISR()
{
  mpu_interrupt = true;
}

float acc_x,acc_y,acc_z;
float moving_avg_acc_x,moving_avg_acc_y,moving_avg_acc_z;

void loop() {
 if(mpu_interrupt){
  mpu_interrupt = false;
  update_data();
  for(int i = 0;i<5;i++){
    acc_x = condition_acceleration_data(&a_x)-x_offset;
    acc_y = condition_acceleration_data(&a_y)-y_offset;
    acc_z = condition_acceleration_data(&a_z)-z_offset;
    moving_avg_acc_x +=acc_x;
    moving_avg_acc_y +=acc_y;
    moving_avg_acc_z +=acc_z;
  }
  moving_avg_acc_x = moving_avg_acc_x/6 ;
  moving_avg_acc_y = moving_avg_acc_y/6 ;
  moving_avg_acc_z = moving_avg_acc_z/6 ;
  
  
  Serial.print("X acceleration = ");
  Serial.println(moving_avg_acc_x);
  Serial.print("Y acceleration = ");
  Serial.println(moving_avg_acc_y);
  Serial.print("Z acceleration = ");
  Serial.println(moving_avg_acc_z);
 }

}




