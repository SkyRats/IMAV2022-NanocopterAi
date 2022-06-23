
import numpy as np
import cv2 as cv



im = cv.imread('gate3.png' ) #Loads the image
hsv = cv.cvtColor(im, cv.COLOR_BGR2HSV) #Changes the formatting to HSV
orangeLower = np.array([22,100,100])
orangeUpper = np.array([25,255,255])
mask = cv.inRange(hsv,orangeLower, orangeUpper)# Isolates the color Orange
blur_mask = cv.blur(mask, (5,5))# Smooths the color mask


contours, hierarchy = cv.findContours(blur_mask, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE ) # Finds the edges of each orange shape, but only external shapes

for i in contours: #Goes over each shape found in the last function
    aprox =cv.approxPolyDP(i,8,True) #Aproximates the shape using fancy math, magic or something just as powerful
    if len(aprox) == 4: # In the future, make sure the 4 corners form a square ( the corners are not perfect, so not as easy as it seems)
        center = [(aprox[3][0][0] + aprox[0][0][0])//2, (aprox[2][0][1] + aprox[0][0][1])//2] # Seems like the corners are in some sort of order, so the specific numbers reflect that
        
print(center) #What we want - the rest of the code should proceed to use this coordinates to change speed in the y and z axis of the drone, aligning it to the gate 
