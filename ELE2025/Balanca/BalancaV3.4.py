import serial
import collections
import time
import os
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

#------------------------ escrever arquivo
j = 1
filePath = f"test/t{j}.csv"
while os.path.exists(filePath):
    j += 1
    filePath = f"test/t{j}.csv"
print("Writing on file " + filePath)
file = open(filePath, "x")
file.write(f"test {j}\n")
file.write("Force (N), Time (ms)\n")
file.flush()

#------------------------ ploting config
fig = plt.figure(figsize=(12,6), facecolor='#DEDEDE')
ax = fig.add_subplot(1, 1, 1)
loads = collections.deque()
times = collections.deque()
maxMeasures = 200
measuresToSave = 100
maxframes = 25
ax.set_title(f"Teste estático {j}")
ax.set_xlabel("Tempo (s)")
ax.set_ylabel("Força (N)")

#------------------------ serial start
print("Initializing static test", end="")

SerialObj = serial.Serial('COM16', baudrate=115200, bytesize=8, parity='N', stopbits=1, timeout=3)
time.sleep(1)
SerialObj.write(b'S')

p = 0
while p <= 3:
    time.sleep(1)
    print(".", end="")
    p += 1
print("Done!\n")
SerialObj.flushInput()

while True:
    packet = SerialObj.readline()
    reading = packet.decode('utf-8', errors='ignore').strip()
    print(f"DEBUG: {reading}")
    if "Measuring:" in reading:
        break

print("Iniciando coleta de dados!")

def readMeasurings(i):
    if SerialObj.in_waiting:
        packet = SerialObj.readline()
        reading = packet.decode('utf-8', errors='ignore').strip()
        
        if ',' not in reading:
            print(f"Ignorando: {reading}")
            return  
        
        values = reading.split(',')

        if len(values) == 2:
            try:
                load = float(values[0])
                time_val = int(values[1]) / 1000
                file.write(reading + "\n")
                loads.append(load)
                times.append(time_val)
            except ValueError:
                print(f"Erro ao converter valores: {values}")
        else:
            print(f"Erro: Mensagem corrompida - {reading}")

        if len(loads) > maxMeasures:
            loads.popleft()
            times.popleft()
            
        if i % measuresToSave == 0:
            file.flush()
        
        if i % 10 == 0:
            ax.cla()
            ax.plot(times, loads, color='tab:blue')
        
ani = FuncAnimation(fig, readMeasurings, interval=1, save_count=maxframes)
plt.show()