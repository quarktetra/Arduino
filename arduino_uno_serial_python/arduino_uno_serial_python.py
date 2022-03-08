import serial
import time
import os
import numpy as np
import time
timestr = time.strftime("%Y%m%d-%H%M%S")
mylist = []

ser = serial.Serial('COM4', 250000, timeout=.1) # 115200 sets up serial connection (make sure baud rate is correct - matches Arduino)    Also check the Arduino serial port

samples=2000
line = 0
magnetS="0"
skipcount=0
while line<=samples :
    line = line+1
    #print(ser.readline())
    try:
        item=list(map(int,ser.readline().decode('utf-8').rstrip().split(",") ))
        if len(item) ==4 :
            item.append(magnetS)
            mylist.append(item)
            #print(item)

            #print(item)
    except:
        skipcount=skipcount+1
        print("skip="+str(skipcount))

mat = np.array(mylist)

ser.close()

np.savetxt("log_data_magnet"+magnetS+"_"+timestr+".csv", mat, delimiter=",", fmt='%s' ,
           header="time, voltage,current,trigger,magnet", comments="")

#print(mat)
print("done")