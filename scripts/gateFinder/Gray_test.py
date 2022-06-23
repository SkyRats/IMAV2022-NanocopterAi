
import numpy as np
import cv2 as cv



im = cv.imread('gate3.png', cv.IMREAD_GRAYSCALE) #Loads the image


for i in range(1, 250):
    ret, thresh = cv.threshold(im, i, 255, 0)
    #contours, hierarchy = cv.findContours(thresh, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE )
    #cv.drawContours(im, contours, -1, (0,255,0), 3)
    print(i)
    cv.imshow('test', thresh)
    cv.waitKey(-1)
