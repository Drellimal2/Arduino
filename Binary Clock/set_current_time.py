import serial
import time

ser = serial.Serial('//./COM5', 9600)
ser.write(bytes('T' +str(int(time.time())), 'utf-8'))
ser.close()
