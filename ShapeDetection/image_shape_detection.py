import cv2
import numpy as np

def shape_detection(img):
    '''
        inputs:
            img = sorce img
        output:
            draw contours on img and detect its shape
    '''
    contours, h = cv2.findContours(img, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)
    for c in contours:
        cv2.drawContours(edges, c, -1, (0, 0, 0), 3)
        length = cv2.arcLength(c, True)
        approx = cv2.approxPolyDP(c, 0.02*length, True)
        num_edges = len(approx)
        x, y, w, h = cv2.boundingRect(approx)
        if num_edges == 3:
            shape = "Triangle"
        elif num_edges > 4:
            shape = "Circle"
        elif num_edges == 4:
            ratio = w / float(h)
            if ratio - 1 <= 0.5:
                shape = "Square"
            else:
                shape = "Rectangle"
        cv2.putText(edges, shape, (x+(w//2), y+(h//2)), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 0), 2)
# Reading img
img = cv2.imread("Shape Detection.png")
# Making it gray
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
# add a blue
blured = cv2.GaussianBlur(gray, (7, 7), 1)
# canny edge detection
canny = cv2.Canny(blured, 50, 50)
# copy to implement on new img
edges = img.copy()
# shape detection function
shape_detection(canny)
cv2.imshow("edges", edges)
cv2.waitKey(0)