import cv2
import numpy as np
def shapeDetection(cap):
    def get_contours(img, out):
        '''
            input:
                img: sorce img or frame
                out: output img
            output:
                draw contours, put text with its shape and draw a triangle around it
        '''
        contours, h = cv2.findContours(img, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)
        for cnt in contours:
            area = cv2.contourArea(cnt)
            #print(area)
            # min_area = cv2.getTrackbarPos("Minimum Area", "Parameters")
            # max_area = cv2.getTrackbarPos("Maximum Area", "Parameters")
            min_area = 500
            max_area = 10000
            if area > min_area and area < max_area:
                #cv2.drawContours(out, contours, -1, (255, 255, 255), 2)
                length = cv2.arcLength(cnt, True)
                approx = cv2.approxPolyDP(cnt, 0.02*length, True)
                num_edges = len(approx)
                x, y, w, h = cv2.boundingRect(approx)
                #print(num_edges)
                if num_edges == 3:
                    shape = "Triangle"
                elif num_edges > 6 and num_edges < 9:
                    shape = "Circle"
                elif num_edges == 4:
                    ratio = w / float(h)
                    if ratio - 1 <= 0.5:
                        shape = "Square"
                    else:
                        shape = "Rectangle"
                else:
                    continue
                cv2.putText(copied_frame, shape, (x+(w//2), y+(h//2)), cv2.FONT_HERSHEY_SIMPLEX, 2, (0, 255, 0), 2)
                cv2.rectangle(copied_frame, (x, y), (x + w, y + h), (255, 0, 0), 3)
                


    # a function to do nothing in the track bar creation
    def nothing(x):
        pass
    # track bars
    # cv2.namedWindow("Parameters")
    # cv2.resizeWindow("Parameters", 600, 200)
    # cv2.createTrackbar("Threshold 1", "Parameters", 10, 255, nothing) # value of threshold 1
    # cv2.createTrackbar("Threshold 2", "Parameters", 78, 255, nothing) # value of threshold 2
    # cv2.createTrackbar("Minimum Area", "Parameters", 500, 50000, nothing)
    # cv2.createTrackbar("Maximum Area", "Parameters", 50000, 50000, nothing)

    #cap = cv2.VideoCapture(0)

    
    _, frame = cap.read()
    #frame = cv2.imread("ShapeDetection.png")
    copied_frame = frame.copy() # copy to not to edit the main frame
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY) # meking it gray
    blur = cv2.GaussianBlur(gray, (7, 7), 1) # blur filter
    # threshold1 = cv2.getTrackbarPos("Threshold 1", "Parameters") # first threshold to canny
    # threshold2 = cv2.getTrackbarPos("Threshold 2", "Parameters") # second threshold to canny
    threshold1 = 10
    threshold2 = 78
    canny = cv2.Canny(blur, threshold1, threshold2)
    kernel = np.ones((5, 5))
    dilation = cv2.dilate(canny, kernel) # dilate to get rid of some noise
    get_contours(dilation, copied_frame)
        #cv2.imshow("Contoured", copied_frame)
    return copied_frame
    #if cv2.waitKey(1) == 27: # if esc button is clicked, then break
        #break

    #cap.release()
    #cv2.destroyAllWindows()

# cap = cv2.VideoCapture(0)

# while True:
#     frame = shapeDetection(cap)
#     cv2.imshow("frame", frame)
#     if cv2.waitKey(1) == 27:
#         break
# cap.release()
# cv2.destroyAllWindows()