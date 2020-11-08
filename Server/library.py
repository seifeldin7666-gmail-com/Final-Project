import cv2
import numpy as np

def getColorBounds(range):
    hsv = cv2.cvtColor(range , cv2.COLOR_BGR2HSV)
    lower = 0
    x = hsv[0,0,0]
    if(x -10 >0):
        lower=x-10
    lower = np.array([lower , 50 , 50] , dtype=np.uint8)
    upper = 255
    if(x+20<255):
        upper = x+10
    return lower , np.array([upper , 255 , 255] , dtype=np.uint8)        

def getMask(img , lower , upper):
    img = cv2.GaussianBlur(img , (3 , 3) , 0.1)
    img = cv2.medianBlur(img , 3)
    hsv = cv2.cvtColor(img ,cv2.COLOR_BGR2HSV)
    mask = cv2.inRange(hsv , lower , upper)
    mask = cv2.dilate(mask , np.ones((5,5)))
    mask = cv2.erode(mask , np.ones((5,5)) )
    return img , mask

def getLines(mask , frame):
    low , up = autoCanny(mask , 0.1)
    edges = cv2.Canny(mask, low, up)
    #cv2.imshow("edges",edges)
    lines = cv2.HoughLinesP(edges,1,np.pi/180,40,minLineLength=10,maxLineGap=30)
    i = 0
    if lines is not None:
        for x1,y1,x2,y2 in lines[0]:
            i+=1
            cv2.line(frame,(x1,y1),(x2,y2),(255,0,0),5)
    #print (i, "Lines")
    return edges , frame
    
def autoCanny(mask , sigma):
    med = np.median(mask)
    lower = int(max(0, (1.0 - sigma) * med))
    upper = int(min(255, (1.0 + sigma) * med))
    return lower , upper    
        