import serial

ser=serial.Serial('/dev/cu.usbmodem1411')
File=open("SensorData.txt","a+")
while(1):
    i=0
    data=ser.readline()
    File.write(data)
    print(data)
    
