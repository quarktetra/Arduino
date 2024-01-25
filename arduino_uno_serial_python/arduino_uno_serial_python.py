import serial
import time
import os
import numpy as np
import time
timestr = time.strftime("%Y%m%d-%H%M%S")
mylist = []

ser = serial.Serial('COM5', 115200, timeout=1) # 115200 sets up serial connection (make sure baud rate is correct - matches Arduino)    Also check the Arduino serial port
### the time out number is critical. 1 seems to be working
fileNameTag="CoilA_Old_setup_mgRemoved_PowerSupply_TEST_TEST"
samples=1000
line = 0
magnetS="0"
damperT="3"
skipcount=0
Capacitor="1000"
Inductor="7.85"
Resistance="24.8"
# don't use the   ser.readline() twice, you idiot. It will skip every other line and you will waste 2 hours to fix that!
while line<=samples and skipcount<30:
    line = line+1
    TheLine=    ser.readline()
    print(TheLine)
    try:
        item=list(map(int,TheLine.decode('utf-8').rstrip().split(",") ))
        if len(item) ==5 :
            item.append(magnetS)
            item.append(damperT)
            item.append(Capacitor)
            item.append(Inductor)
            item.append(Resistance)

            mylist.append(item)
            #print(item)

            #print(item)
    except:
        skipcount=skipcount+1
        print("skip="+str(skipcount))

mat = np.array(mylist)

ser.close()

np.savetxt("log_data_magnet_"+fileNameTag+magnetS+"_"+timestr+".csv", mat, delimiter=",", fmt='%s' ,
           header="time, voltage,current,trigger,optic, magnet,damperT,Capacitor,Inductor,Resistance", comments="")


#print(mat)
print("done")