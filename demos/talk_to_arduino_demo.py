import serial
import time

s = serial.Serial('COM6', 9600, timeout=1)

# while True:
#     print(s.readline())

for i in range(5):
    res = s.readline()
    print(i, res)
    time.sleep(0.1)
