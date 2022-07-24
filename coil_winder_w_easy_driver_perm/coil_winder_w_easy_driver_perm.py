import serial
import time
import os
import numpy as np
import time
timestr = time.strftime("%Y%m%d-%H%M%S")
mylist = []

ser = serial.Serial('COM4', 115200, timeout=1) # 115200 sets up serial connection (make sure baud rate is correct - matches Arduino)    Also check the Arduino serial port
### the time out number is critical. 1 seems to be working
fileNameTag=""
samples=5000
line = 0
terminate=0
toAppend=["Gauge","Date"]
toAppendV=["32",timestr]
# don't use the   ser.readline() twice, you idiot. It will skip every other line and you will waste 2 hours to fix that!
TheHeader=[]
while line<=samples and terminate<1:
    line = line+1
    TheLine=    ser.readline()
    #print(TheLine)
    try:
        item=list(TheLine.decode('utf-8').rstrip().split(",") )
        if item[0]=='header:':
            TheHeader=item
            print(TheHeader)
        if item[0]=='TerminateTransmission':
            print("Hasta la vista, baby")
            terminate=1
    except:
        item=[]

    if len(item) ==len(TheHeader)  and item[0]=="ToBeLogged":
        del item[0]
        item=item+toAppendV



        mylist.append(item)
        print(item)


mat = np.array(mylist)

ser.close()

if len(mylist)>0:
    TheHeader=TheHeader+toAppend
    del TheHeader[0]
    TheHeader= ','.join(TheHeader)

    np.savetxt("log_data_orange15mm_5_8_10_coil_"+fileNameTag+"_"+timestr+".csv", mat, delimiter=",", fmt='%s' ,
              header=TheHeader, comments="")


#print(mat)
print("done")