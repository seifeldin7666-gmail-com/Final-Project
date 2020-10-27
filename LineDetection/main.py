import library as lib
import cv2
import numpy as np

color = np.array([[[100,0,255]]],dtype = np.uint8)
# print(color)

lower , upper = lib.getColorBounds(color)

cap = cv2.VideoCapture(0)

while True:
    _,frame = cap.read()
    frame,mask = lib.getMask(frame , lower , upper)
    edges , frame = lib.getLines(mask , frame)    
    
    ##Now we begin contours
    
    cv2.imshow("Vdieo" , frame)
    cv2.imshow("Line detector" , edges)
    
    if cv2.waitKey(1) == ord('q'):
        break
cap.release()
cv2.destroyAllWindows()    
    







# cv2.imshow("res",result)
# cv2.waitKey(0)