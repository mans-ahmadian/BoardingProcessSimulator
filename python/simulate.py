import os
import cv2
import pandas as pd
import numpy as np 
from dataclasses import dataclass


class Passenger:
    def __init__(self,SeatRow,SeatCol):
        self.TicketNumber
        self.SeatRow
        self.SeatCol
        self.position
        
class SceneWriter:
    def __init__(self, outputDir="output",
                numberOfSeatRows=20,numberOfSeatColumns=3,
                seatSize=20,seatColor=(255, 0, 0),seatPadding=5,
                aisleSize=50,seatBlock=2,
                personSize=15,personColor=(0.256,0),
                seatColumnChars=['A','B','C','D','E','F','G','H','I','J','K']):
        self.OutputDir=outputDir
        self.NumberOfSeatRows = numberOfSeatRows
        self.NumberOfSeatColumns=numberOfSeatColumns
        self.SeatSize=seatSize
        self.SeatColor=seatColor
        self.SeatPadding=seatPadding
        self.AisleSize=aisleSize
        self.SeatBlock=seatBlock
        self.PersonSize=personSize
        self.PersonColor=personColor
        self.SeatColumnChars=seatColumnChars
        self.imageWidth=(seatSize*2+seatPadding) *numberOfSeatColumns *self.SeatBlock +(self.SeatBlock-1)* aisleSize 
        self.imageHeight=(seatSize*2+seatPadding) *numberOfSeatRows+seatPadding
        #print(self.imageHeight)
    def DrawSeats(self,image):
        SeatBlockStart=self.AisleSize+(self.NumberOfSeatColumns * self.SeatSize*2)+(self.NumberOfSeatColumns-1)*self.SeatPadding
    
        for rowSeat in range(0,self.NumberOfSeatRows):
            seatCol=0
            for seatBlock in range(0,self.SeatBlock):
                for columnSeat in range(0,self.NumberOfSeatColumns):
                    seatStartX=rowSeat * (self.SeatSize*2+self.SeatPadding)+self.SeatPadding+self.SeatSize
                    seatStartY=columnSeat * (self.SeatSize*2+self.SeatPadding)+(seatBlock*SeatBlockStart)
                    seatEndX=seatStartX+self.SeatSize
                    seatEndY=seatStartY+self.SeatSize*2
                    thickness = 2
                    image = cv2.rectangle(image, (seatStartY,seatStartX), (seatEndY,seatEndX), self.SeatColor, thickness)
                    seatNumber=str(rowSeat+1)+self.SeatColumnChars[seatCol]
                    image = cv2.putText(image, seatNumber, (seatStartY+10,seatStartX+15),cv2.FONT_HERSHEY_SIMPLEX,.4,self.SeatColor,thickness )
                    seatCol=seatCol+1
                    #print(seatStartX,seatEndX)
        
    def getNumberOfSeats(self):
        return self.NumberOfSeatRows*self.NumberOfSeatColumns * self.SeatBlock
    
    def AddFrame(self,frameNo,passengers):   
        imageName=os.path.join(self.OutputDir,"Frame__"+str(frameNo).zfill(4)+".jpg")
        image = np.zeros((self.imageHeight,self.imageWidth,3), dtype=np.uint8)
        self.DrawSeats(image)
        cv2.imwrite(imageName,image)
        
   
passengers=[]
       
sw=SceneWriter("D:\\MyData\\mans_personal_data\\SourceCode\\AirLineSimulation\\output")
numberOfSeats=sw.getNumberOfSeats()
# give ticket to passengers

for i in range(numberOfSeats):
    print()

for time in range(0,100):    
    sw.AddFrame(time,passengers)  
