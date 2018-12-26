import serial
import time

#com transmitter init (motor)
s = serial.Serial()
s.timeout = 10
s.baudrate = 115200
s.bytesize=8
s.parity='N'
s.stopbits=1
s.port = 'COM3'
s.setDTR(False)
s.open()


x = [200, 100, 150]
y = [100, 200, 125]

for i,pos in enumerate(x):
    s.write(bytes((x[i],y[i])))
    s.read(1) #send next trigger
    time.sleep(1)
