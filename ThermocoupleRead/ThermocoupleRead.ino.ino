/*
 * Author: Justin Heimerl
 * Email: jpheimerl@gmail.com
 * Purpose: The following code serves to interface the SEN-30004 with an Arduino Uno.
 * The SEN-30004 contains 4 MAX31855T thermocouple to digital converters. 
 * Pinout:
 * Arduino Pin 13 -> SEN 30004 Pin 13
 * Arduino Pin 12 -> SEN 30004 Pin 12
 * Arduino Pin 10 -> SEN 30004 Pin 6 (THIS IS IMPORTANT THE PDF ON THE WEBPAGE FOR THE SEN30004 IS WRONG)
 * Then after one channel reads find the pins that correspond to the other channels(likely 3-5 on the SEN)
 * 
 */


//MAX31855 communicates over SPI.
#include <SPI.h>
//I used pin 10 as the CS Line. When the CS line is pulled low the sensor begins communicating.
//Declaring the other two pins does not really matter I just did it anyways.
int maxSO =  12;
int maxCS =10;
int maxSCK = 13;


void setup(){
  //Begin serial for monitoring.
  Serial.begin(9600);
  //delay(500);
  //Set all pins to high so they dont send data.
  for(int i=3;i<11;i++){
    digitalWrite(i,HIGH);
    pinMode(i,OUTPUT);
  }
  SPI.begin();
  
}




void loop(){
  //Begin SPI transaction. MAX31855 has max 5MHz clock, I used 4MHz. It send MSB first, and is a SPI mode 1 device.
  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE1));
  //Set the chip select line to low to start reading.
  digitalWrite(maxCS,LOW);
  //declare a byte array to store recieved data.
  byte byteArray [4];
  //This for loop recieves the data. The MAX31855 sends 32 bits (4 bytes). Call SPI transfer 4 times to recieve all data.
  for(int i=0;i<4;i++){
    byte b=SPI.transfer(10);   
    //store the byte in the array.                                                                                                                                                                                                              
    byteArray[i]=b;
  }
  //Set the CS pin to high to stop data transmission.
  digitalWrite(maxCS,HIGH);
  //End SPI
  SPI.endTransaction();
  //We want to store the 1s and 0s, declare an array and populate it.
  int bitArray [32];
  int index=0;
  //The nested for loops populate the array. There is an easier way to do this but I'm an EE.
  for(int i=0;i<4;i++){
    for(int j=7;j>=0;j--){
      byte byteToAccess=byteArray[i];
      int bitToAdd=bitRead(byteToAccess,j);
      bitArray[index]=bitToAdd;//add bits from MSB to LSB (ie index 31 is LSB, 0 is MSB)
      index++;
    }
  }
  //Confirm the data is not garbage.
  for(int i:bitArray){
    Serial.println(i);
  }
  //Signal the end of SPI data.
  Serial.println("end of data transmission");
  //Store the temp in a variable.
  float tempSum=0.0;
  //The temperature data is stored in bits 18 -> 30. Since we stored MSB to LSB this is array[1]->array[13]
  for(int i=1;i<14;i++){
    //calculate the temperature from bianary (LSB is 2^-2,MSB is 2^10).
    tempSum=tempSum+(float)bitArray[i]*pow(2,11-i);
  }
  Serial.println("Temperature=");
  Serial.println(tempSum);
  //The 16th bit will be high if there is any error. Usually this is indicative of no thermocouple connection or a bad arduino to SEN connection.
  if(bitArray[15]==1){
    Serial.println("unknown fault");
  }
  delay(1000);
}

