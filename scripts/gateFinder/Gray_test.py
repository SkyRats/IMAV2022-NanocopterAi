
import numpy as np
import cv2 as cv



im = cv.imread('gate1.png', cv.IMREAD_GRAYSCALE) #Loads the image
blur_im = cv.blur(im, (3,3))
edges = cv.Canny(blur_im, 77, 186)
kernel = np.ones((5,5), np.uint8)
dilEdges = cv.dilate(edges,kernel, iterations=2)
contours, hierarchy = cv.findContours(dilEdges, cv.RETR_CCOMP, cv.CHAIN_APPROX_SIMPLE )
#cv.imshow('thresh', dilEdges)
#cv.imshow('im', im)
#cv.waitKey(-1)

lines = []
for i in contours:
    aprox =cv.approxPolyDP(i,8,True)
    if len(aprox) >= 2:
        for i in range(len(aprox)):
            for j in range(1, len(aprox)-i):
                dx = aprox[i][0][0] - aprox[j+i][0][0]
                dy = aprox[i][0][1] - aprox[j+i][0][1]
                distance = np.sqrt((dx*dx + dy*dy))
                if distance >= 100:
                    if (dx < 10 and dx > -10) or (dy < 10 and dy > -10):
                        
                        lines.append([aprox[i], aprox[j+i], distance])

squares = []

while len(lines) > 1:
    j = 1
    while j <= len(lines)-1:
        if lines[0][2] >= lines[j][2] -10 and lines[0][2] <= lines[j][2] + 10:
            dx11 = lines[0][0][0][0] - lines[j][0][0][0]
            dx12 = lines[0][0][0][0] - lines[j][1][0][0]
            dx21 = lines[0][1][0][0] - lines[j][0][0][0]
            dx22 = lines[0][1][0][0] - lines[j][1][0][0]
            dy11 = lines[0][0][0][1] - lines[j][0][0][1]
            dy12 = lines[0][0][0][1] - lines[j][1][0][1]
            dy21 = lines[0][1][0][1] - lines[j][0][0][1]
            dy22 = lines[0][1][0][1] - lines[j][1][0][1]
            if (dx11 >= -10 and dx11 <= 10) and (dy11 >= -10 and dy11 <= 10):
                squares.append([lines[0][0], lines[0][1], lines[j][1], 0])
                lines.pop(j)
                lines.pop(0)
            elif (dx12 >= -10 and dx12 <= 10) and (dy12 >= -10 and dy12 <= 10):
                squares.append([lines[0][0], lines[0][1], lines[j][0],0])
                lines.pop(j)
                lines.pop(0)
            elif (dx21 >= -10 and dx21 <= 10) and (dy21 >= -10 and dy21 <= 10):
                squares.append([lines[0][0], lines[0][1], lines[j][1],1])
                lines.pop(j)
                lines.pop(0)
            elif (dx22 >= -10 and dx22 <= 10) and (dy22 >= -10 and dy22 <= 10):
                squares.append([lines[0][0], lines[0][1], lines[j][0],1])
                lines.pop(j)
                lines.pop(0)
        j += 1
    lines.pop(0)

Centers = []
for i in squares:
    cv.drawMarker(im, i[0][0], (0,255,0))
    cv.drawMarker(im, i[0][0], (0,255,0))
    cv.drawMarker(im, i[0][0], (0,255,0))
    maiorX = 0
    smaiorX = 1
    maiorY = 0
    smaiorY = 1
    for j in range(len(i)-1):
        if i[j][0][0] > i[maiorX][0][0]:
            smaiorX = maiorX
            maiorX = j
        if i[j][0][1] > i[maiorY][0][1]:
            smaiorY = maiorY
            maiorY = j
    menorX = 3 - maiorX - smaiorX
    menorY = 3 - maiorY - smaiorY
    CenterX = i[menorX][0][0] + (i[maiorX][0][0] - i[menorX][0][0])//2
    CenterY = i[menorX][0][1] + (i[maiorY][0][1] - i[menorY][0][1])//2
    if (CenterX >= 100 and CenterX <= im.shape[1] - 100) and (CenterY >= 100 and CenterY <= im.shape[0] - 200):
        Centers.append([CenterX,CenterY])

SumX = 0
SumY = 0
for i in Centers:
    SumX += i[0]
    SumY += i[1]

mCenter = [SumX//len(Centers), SumY//len(Centers)]
cv.drawMarker(im, mCenter, (255,0,0))

cv.imshow("im", im)
cv.waitKey(-1)





        



