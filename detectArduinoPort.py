import serial
import time
import os
import numpy as np



import sys
import serial.tools.list_ports

serPort = ""
int1 = 0
str1 = ""
str2 = ""

# Find Live Ports
ports = list(serial.tools.list_ports.comports())
for p in ports:
   print (p) # This causes each port's information to be printed out.
           # To search this p data, use p[1].

while int1 < 9:   # Loop checks "COM0" to "COM8" for Adruino Port Info.

  if "CH340" in p[1]:  # Looks for "CH340" in P[1].
        str2 = str(int1) # Converts an Integer to a String, allowing:
        str1 = "COM" + str2 # add the strings together.

  if "CH340" in p[1] and str1 in p[1]: # Looks for "CH340" and "COM#"
     print ("Found Arduino Uno on " + str1  )
     int1 = 9 # Causes loop to end.

  if int1 == 8:
     print ("UNO not found!"  )
     sys.exit() # Terminates Script.

  int1 = int1 + 1

time.sleep(1)  # Gives user 5 seconds to view Port information -- can be   changed/removed.


timestr = time.strftime("%Y%m%d-%H%M%S")
mylist = []
myCalibList = []

ser = serial.Serial(str1, 115200, timeout=1) #