
import numpy as np
import cv2 as cv

def gateFinder(im):
# Loads the grayscale image, processes it until we can find the contours a little easier

    blur_im = cv.blur(im, (3,3))
    edges = cv.Canny(blur_im, 77, 186)
    kernel = np.ones((5,5), np.uint8)
    dilEdges = cv.dilate(edges,kernel, iterations=2)
    contours, hierarchy = cv.findContours(dilEdges, cv.RETR_CCOMP, cv.CHAIN_APPROX_SIMPLE )
    #Aproximates the contours, and saves the ones who form a straight line with a minimal size
    lines = []
    for i in contours:
        aprox =cv.approxPolyDP(i,8,True)
        if len(aprox) >= 2:
            for i in range(len(aprox)):
                for j in range(1, len(aprox)-i):
                    dx = aprox[i][0][0] - aprox[j+i][0][0]
                    dy = aprox[i][0][1] - aprox[j+i][0][1]
                    distance = np.sqrt((dx*dx + dy*dy))
                    if distance >= 10:
                        if (dx < 2 and dx > -2) or (dy < 2 and dy > -2):
                            lines.append([aprox[i], aprox[j+i], distance])

    squares = []
    #Checks if the lines found share one coordinate and if they have about the same lenght. If so, they can form a square
    while len(lines) > 1:
        j = 1
        while j <= len(lines)-2:
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
                    squares.append([lines[0][0], lines[0][1], lines[j][1]])
                    lines.pop(j)
                    lines.pop(0)
                elif (dx12 >= -10 and dx12 <= 10) and (dy12 >= -10 and dy12 <= 10):
                    squares.append([lines[0][0], lines[0][1], lines[j][0]])
                    lines.pop(j)
                    lines.pop(0)
                elif (dx21 >= -10 and dx21 <= 10) and (dy21 >= -10 and dy21 <= 10):
                    squares.append([lines[0][0], lines[0][1], lines[j][1]])
                    lines.pop(j)
                    lines.pop(0)
                elif (dx22 >= -10 and dx22 <= 10) and (dy22 >= -10 and dy22 <= 10):
                    squares.append([lines[0][0], lines[0][1], lines[j][0]])
                    lines.pop(j)
                    lines.pop(0)
            j += 1
        lines.pop(0)

    # Calculates the centers for each square found, removes the ones on the extremities.
    Centers = []
    for i in squares:

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
        #if (CenterX >= 10 and CenterX <= im.shape[1] - 10) and (CenterY >= 10 and CenterY <= im.shape[0] - 20):
        Centers.append([CenterX,CenterY])


    # Calculates the average of the centers found, which is roughly the desired center of the square

    if len(Centers) != 0:
        SumX = 0
        SumY = 0
        for i in Centers:
            SumX += i[0]
            SumY += i[1]

        mCenter = [SumX//len(Centers), SumY//len(Centers)]
        return mCenter
    else:
        return False





        



