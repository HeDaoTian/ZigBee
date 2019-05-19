import serial
import time
import struct
import csv
import os
#import numpy as np
#import matplotlib.pyplot as plt




ard = serial.Serial('/dev/cu.usbmodem14111' ,9600)
def write_csv(counter):
    count=0
    while count<int(counter):  
        count = count+1
        line = ard.readline().strip()
        print(line)     

        with open ('DataOutput.csv','a') as f:
             writer = csv.writer(f,lineterminator='\r')
             writer.writerow([fields,line])

    
if __name__ == '__main__':
    #with open ('output.csv','a') as f:
        #writer = csv.DictWriter(f,fieldnames=['Distance','Rssi'])
        #writer.writeheader()      
    freq = raw_input("Please identify the sending frequency(millisecond): ")
    siz = raw_input("Please identify the payload size of sending package(1~255): ")
    counter = raw_input("How many samples do you collect? ")
    var = (freq+","+siz+"\n")
    field=raw_input('Please enter test distance: ')
    fields=(field+' meter')
    ard.write(var)
    write_csv(counter)


 
    
    


    

        
ard.close()         

   
