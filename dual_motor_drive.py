import serial
import time

# print(bytes('a/r/n'))

s = serial.Serial('COM6', 115200, timeout=10)
time.sleep(2.0)
#
print(s.readline())
s.write(b'100\n')
for i in range(5):
    s.write(b'700\n')
#     # time.sleep(0.9)
    print(i,s.read())
    s.write(b'300\n')
#     time.sleep(0.9)
    print(i,s.read())
