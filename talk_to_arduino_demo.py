import serial
import time

s = serial.Serial('COM6', 115200)

for i in range(100):
    res = s.read()
    print(res)
    time.sleep(100)
