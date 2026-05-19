import serial
import collections
import time
import os
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

#------------------------ escrever arquivo
j=1
filePath = "test/t"+str(j)+".csv"
while os.path.exists(filePath):
    j += 1
    filePath = "test/t"+str(j)+".csv"
print("Writing on file "+filePath)
file = open(filePath, "x")
file.write("test "+str(j)+"\n")
file.write("Force (N), Time (ms)\n")
file.flush()

#------------------------ ploting config
fig = plt.figure(figsize=(12,6),facecolor='#DEDEDE')
ax = fig.add_subplot(1, 1, 1)
loads = collections.deque()
times = collections.deque()
maxMeasures = 200
measuresToSave = 100
maxframes = 25
ax.set_title("Teste estático "+str(j))
ax.set_xlabel("Tempo (s)")
ax.set_ylabel("Força (N)")

#------------------------ serial start

print("Initializing static test", end = "")

SerialObj = serial.Serial('COM16')
SerialObj.baudrate = 115200
SerialObj.bytesize = 8
SerialObj.parity = 'N'
SerialObj.stopbits = 1
SerialObj.timeout = 3
time.sleep(1)
SerialObj.write(b'S')
p = 0
while p<=3:
    time.sleep(1)
    print(".", end = "")
    p += 1
print("Done!\n")
SerialObj.flushInput()

def readMeasurings(i):
    if SerialObj.in_waiting:
        packet = SerialObj.readline()
        reading = packet.decode('utf').rstrip('\r\n')
        values = reading.split(',')

        if len(values) ==2:
            file.write(reading+"\n")
            loads.append(float(values[0]))
            times.append(int(values[1])/1000)
        else:
            print("Error: Mensage Corrupted: "+reading)

        if len(loads) > maxMeasures:
            loads.popleft()
            times.popleft()
            
        if i%measuresToSave == 0:
            file.flush()
        
        if i % 10 == 0:
            ax.cla()
            ax.plot(times, loads, color='tab:blue')
        
ani = FuncAnimation(fig, readMeasurings, interval=1, save_count = maxframes)
plt.show()



