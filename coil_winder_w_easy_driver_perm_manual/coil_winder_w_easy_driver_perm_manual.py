import serial
import time
import os
import numpy as np
import time
import serial.tools.list_ports

timestr = time.strftime("%Y%m%d-%H%M%S")
mylist = []

ports = list(serial.tools.list_ports.comports())
print(ports)
arduinoPort=""
for p in ports:
   #print (p) #
   if "CH340" in str(p):
        arduinoPort=str(p).split(' - ')[0]

if arduinoPort == '':
 print ("UNO not found!"  )
 sys.exit()
print("arduinoPort:"+arduinoPort)

ser = serial.Serial(arduinoPort, 115200, timeout=1) # 115200 sets up serial connection (make sure baud rate is correct - matches Arduino)    Also check the Arduino serial port
### the time out number is critical. 1 seems to be working
fileNameTag="950_coil_B_"
samples=10000
line = 0
terminate=0
toAppend=["Date"]
toAppendV=[timestr]
# don't use the   ser.readline() twice, you idiot. It will skip every other line and you will waste 2 hours to fix that!
TheHeader=[]
while line<=samples and terminate<1:
    line = line+1
    TheLine=    ser.readline()
    try:
        item=list(TheLine.decode('utf-8').rstrip().split(",") )
        if item[0]=='header:':
            TheHeader=item
            print(TheHeader)
            numLayersIndex=TheHeader.index("numLayers")
            nTurnsPerLayerIndex=TheHeader.index("nTurnsPerLayer")

        if item[0]=='TerminateTransmission':
            print("Hasta la vista, baby")
            terminate=1
    except:
        item=[]

    if len(item) ==len(TheHeader)  and item[0]=="ToBeLogged":

        if len(mylist)==0:
         fileNameTag=fileNameTag+item[numLayersIndex]+"Layers_"+item[ nTurnsPerLayerIndex]+"TurnsPerLayer"
         print(fileNameTag)

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
     #orange15mm_5_8_10_coil
    np.savetxt("log_data_"+fileNameTag+"_"+timestr+".csv", mat, delimiter=",", fmt='%s' ,
              header=TheHeader, comments="")
    print("saved to "+fileNameTag)
else:
    print("no data is logged!")


#print(mat)
