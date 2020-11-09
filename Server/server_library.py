"""
==================================
Adding libraries to the file 
=================================="""
#font: for the editing of fonts family, shape and style
#PIL: for the team's logo
#opencv: for the videocapture
#tkinter: for editing and creating the gui window
#time: for the timer used
from tkinter import font
from tkinter import ttk
from PIL import Image
from PIL import ImageTk
from cv2 import cv2
import tkinter as tk
import time
from realtime_shape_detection import *
from main import *
import socket
import concurrent.futures
import pickle




FORMAT = "utf-8" # decoding and encoding format
BOX_ARDUINO_PORT = 12345 # console receving port
BOX_ARDUINO_PORT = 12345 # box recieving port

SERVER = socket.gethostbyname(socket.gethostname()) # the host server ip
# print(SERVER)
# CONSOLE_ADDR = (SERVER, CONSOLE_ARDUINO_PORT) # tuple for ip and port 
BOX_ADDR = (SERVER, BOX_ARDUINO_PORT)
server = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # making server object for udp comm
# server.bind(CONSOLE_ADDR) # binding console
server.bind(BOX_ADDR) # binding box

gui_depth = 0
gui_pressure = 0



def reading():
    '''
        inputs:
            data: the data received in bytes
            length: length of array
        outputs:
            return array if length 3(Sensors) or 5(Forces)
    '''
    # if length == 3:
    #     msg = pickle.loads(data) 
    #     return msg
    # elif length == 5:
    #     msg = pickle.loads(data)
    #     return msg
    out = []
    # real_data = []
    data, addr = server.recvfrom(4096)
    # data = pickle.loads(data)
    # for i in data:
    #     real_data.append(i)
    accX = (-1 ** data[2])*(data[0] + data[1]/100)
    out.append(accX)
    accY = (-1 ** data[5])*(data[3] + data[4]/100)
    out.append(accY)
    pressure = data[6] * 255 + data[7]
    out.append(pressure)
    depth = pressure / (1000 * 9.81)
    out.append(depth)
    return out
    # for i in data:
    #     out.append(i)
    # return out

def reading_test():
    out = []
    data, addr = server.recvfrom(4096)
    data = pickle.loads(data)
    for i in data:
        out.append(i)

    return out


def send_to_gui():
    '''
        inputs:
            data: reciving data
            length: length of array
        outputs:
            print values on the gui
    '''
    out = []
    print("Entered send to gui")
    global gui_pressure
    global gui_depth
    while True:
        data = reading()
        print(f"data to send to gui: {data}")
        gui_pressure = data[2]
        #print(gui_pressure)
        gui_depth = data[3]
        #print(gui_depth)
    

# def start():
#     '''
#         Start receiving
#     '''
#     print("Entered Start")
#     with concurrent.futures.ThreadPoolExecutor() as excutor: # to handle more than one operation at same time
#         while True:
#             data = excutor.submit(reading)
# def test():
#     print("Test Waiting")
#     data, addr = server.recvfrom(4096)
#     print(f"data: {data}")
#     print("Received")


"""
===================================
Initialisation of the window
==================================="""
#Initialising the window "gui" by using tkinter library
#AND assigning the title of the window to TORPEDO'20 ROV GUI
#AND editing the background color of the window
gui = tk.Tk()
gui.wm_title("Torpedo'20 ROV GUI")
gui.config(bg="#1c100b")
"""
=================================
Initialization of font
================================="""
textFont = font.Font(family="Times New Roman", size=20)

"""
==================================
Dividing the whole window into two frames (left and right) & readings in frames
=================================="""
#LeftFrame: for logo, timer, speed, compass, and depth
#RightFrame: for the video capture screens
leftFrame = tk.Frame(gui, width="150", height="150", relief="ridge", borderwidth="2", bg="#1c100b")
leftFrame.grid(row=0, column=0)
rightFrame = tk.Frame(gui, width="500", height="200", relief="ridge", borderwidth="2", bg="#1c100b")
rightFrame.grid(row=0, column=1)
speedFrame = tk.Frame(leftFrame, relief="groove", borderwidth=6, bg="#1c100b")
speedFrame.grid(row=2, column=0)
depthFrame = tk.Frame(leftFrame, relief="groove", borderwidth=6, bg="#1c100b")
depthFrame.grid(row=3, column=0)
pressureFrame = tk.Frame(leftFrame, relief="groove", borderwidth=6)
pressureFrame.grid(row=4, column=0)

"""
===================================
Adding the team logo
==================================="""
#Converting the image to a tkinter readable image
# logoImage = tk.PhotoImage(file = "TorpedoLogo2.png", width="500", height="500")
# #Creating a label for the image and placing it on row = ? and column = ? in the LEFTFRAME
# imgLabel = tk.Label(leftFrame, image=logoImage, width=400, height=300, bg="#1c100b")
# imgLabel.grid(row=0, column=0)


"""
============================================
Creating timer for the window
============================================
"""
#Initialization of the values for hour, minute and second
hour = 0
minute = 0
second = 0
#Creating a label for the timer and placing it on row = ? and column = ? in the LEFTFRAME
timerLabel = tk.Label(leftFrame, background="#1c100b", fg="white")
timerFont = font.Font(family="Times New Roman", size=20, weight="bold")
timerLabel.grid(row=2, column=0)

#@RETURN: HOUR : MINUTE : SECOND ----- Ex: 0 : 0 : 0
#@Brief: The function has a counter which adds the value of second every one second by 1
#then adds minute by one when second is equal to 60, with the same concept applies to variable holding the hour value
#finally, it shows the timer in a label, and repeats
def clock():
    #Allowing the function to read the variables HOUR, MINUTE, SECOND
    global hour
    global minute
    global second
    #Editing the value of the label to the new values for HOUR, MINUTE, SECOND
    timerLabel.config(text=str(hour) + " : " + str(minute) + " : " + str(second), font= timerFont)
    #Adding the value of SECOND by 1 for each iteration of the function
    second += 1
    #Checks whether SECOND is equal to 60, If So MINUTE is added by one and SECOND is reinitialized to 0
    #Then checks whether MINUTE is equal to 60, If So HOUR is added by one and MINUTE is reinitialized to 0
    if second == 60:
        second = 0
        minute += 1
    if minute == 60:
        minute = 0
        hour += 1
    #Updates the timerlabel after 1 second and recalls the function again until the program ends
    timerLabel.after(1000, clock)

"""
=====================================
Creating the speed meter
=====================================
#Two circle are used, the first one to specify the 100% speed
#And the other circle has a variable diameter, and center which changes according to the ROV's speed
"""
"""
speedProgressBar = ttk.Progressbar(leftFrame, length=150, orient='horizontal')
speedProgressBar.grid(row=1, column=0)
def speedUpdate():
    speedProgressBar.config(value=speed)
    speedProgressBar(1,speedUpdate)"""
# speed = 100
# #Initializing the dimensions of the circle (the x-coordinate point on the left side, and the y-coordiante of the point on the right side)
# xLeft = 30
# yLeft = 30
# maxSpeed = 255
# speedProgressDiameter = 50

# diameter = 100
# xRight = xLeft + diameter
# yRight = yLeft + diameter
# leftFrameCanvas = tk.Canvas(leftFrame, width=150, height=150)

# leftFrameCanvas.grid(row=1,column=0)
# backgroundCircle = leftFrameCanvas.create_oval(xLeft,yLeft,xRight,yRight,width=2,fill="red")
#

# def speedCircleUpdate():
#     global speed
#     global diameter
#     global maxSpeed
#     speedProgressDiameter = (maxSpeed * diameter) / speed
#     progressCentreX = (xRight + xLeft)/2
#     progressCentreY = (yRight + yLeft)/2
#     xProgressLeft = progressCentreX - (speedProgressDiameter / 2)
#     yProgressLeft = progressCentreY - (speedProgressDiameter / 2)
#     xProgressRight = xProgressLeft + speedProgressDiameter
#     yProgressRight = yProgressLeft + speedProgressDiameter
#     speedProgressCircle = leftFrameCanvas.create_oval(xProgressLeft,yProgressLeft,xProgressRight,yProgressRight,fill="cyan")    
#     leftFrameCanvas.after(100, speedCircleUpdate)
# """
# """speedLabel = tk.Label(leftFrame, background="#1c100b", fg="white")
# speedLabel.grid(row=3, column=0)

# def speedUpdate():
#     speedLabel.config(text="Speed is: "+ str(speed), font=textFont)
#     speedLabel.after(1, speedUpdate)
""" 
==================================================
Capturing the video from the computer's camera
==================================================
"""
upperLabel = tk.Label(rightFrame)
lowerLabel = tk.Label(rightFrame)
sideLabel = tk.Label(rightFrame)

upperLabel.grid(row=0, column=0)
lowerLabel.grid(row=1, column=0)
sideLabel.grid(row=0, column=1, rowspan=2)
cap = cv2.VideoCapture(0,cv2.CAP_DSHOW)
                
def videoFromCam():
    #reading the input from the camera which returns two values the frame itself
    #and a boolean value which checks whether the frame is captured or not
    ret, normal = cap.read()
    #Checking whether the boolean value is equal to true: if so it prints "success" on the terminal
    #otherwise it prints "fail"
    if ret == True:
        #print("success")
        shapeFrame = shapeDetection(cap)
        lineFrame = lineFollower(cap)
        frame = cv2.resize(normal, (600,400)) # normal frame (not shape or line)
        shapeFrame = cv2.resize(shapeFrame, (480,320))
        lineFrame = cv2.resize(lineFrame, (480,320))
        # k = cv2.waitKey(1)
        # if k == -1:
        #     frame = normal
        # else:
        #     if chr(k) == "s":
        #         frame = shapeDetection(cap)
        #     if chr(k) == "l":
        #         frame = lineFollower(cap)
        rgbFrame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGBA)  #Converting the frame from BGR to an RGBA image for fromarray
        rgbShapeFrame = cv2.cvtColor(shapeFrame, cv2.COLOR_BGR2RGBA)  #Converting the frame from BGR to an RGBA image for fromarray
        rgbLineFrame = cv2.cvtColor(lineFrame, cv2.COLOR_BGR2RGBA)  #Converting the frame from BGR to an RGBA image for fromarray
        imgFrame = Image.fromarray(rgbFrame)                     #Converting the image to a PIL image
        imgShapeFrame = Image.fromarray(rgbShapeFrame)                     #Converting the image to a PIL image
        imgLineFrame = Image.fromarray(rgbLineFrame)                     #Converting the image to a PIL image
        imgtkFrame = ImageTk.PhotoImage(image=imgFrame)               #Converting the image to a tkinter readable image
        imgtkShapeFrame = ImageTk.PhotoImage(image=imgShapeFrame)               #Converting the image to a tkinter readable image
        imgtkLineFrame = ImageTk.PhotoImage(image=imgLineFrame)               #Converting the image to a tkinter readable image
    
        upperLabel.imgtk = imgtkShapeFrame
        upperLabel.configure(image = imgtkShapeFrame)
        lowerLabel.imgtk = imgtkLineFrame
        lowerLabel.configure(image = imgtkLineFrame)
        sideLabel.imgtk = imgtkFrame
        sideLabel.configure(image = imgtkFrame)
    else:
        print("fail")

    #Checking the mode: whether line detector or shape
    #if mode == ???:
    #    
    #else:

    #updating the value of the camera label after 30ms and recalling the function videoFromCam() again till the program ends
    upperLabel.after(30, videoFromCam)

"""
==============================
Showing sensors values
=============================="""
pressureLabel = tk.Label(pressureFrame, background="#1c100b", fg="white")
pressureLabel.pack()

def pressureUpdate():
    pressureVal = gui_pressure
    # print(f"pressureVal: {pressureVal}, gui_pressure: {gui_pressure}")
    pressureLabel.config(text="Pressure value is: " + str(pressureVal), font=textFont)
    pressureLabel.after(200,pressureUpdate)


depthLabel = tk.Label(leftFrame)
depthLabel.grid(row=5, column=0)

def depthUpdate():
    depthVal = gui_depth
    # print(f"depthVal: {depthVal}, gui_depth: {gui_depth}")
    depthLabel.config(text="Depth value is: " + str(depthVal), font=textFont)
    depthLabel.after(200,depthUpdate)

"""
===================================
Calling functions
==================================="""
#speedCircleUpdate() calls function which updates the value of the dimensions of the speed circle
#speedCircleUpdate()



def start_system():
    with concurrent.futures.ThreadPoolExecutor() as excutor:
        #excutor.submit(start)
        excutor.submit(send_to_gui)
        excutor.submit(videoFromCam) #videoFromCam() calls function which initiates the video capture
        # excutor.submit(speedUpdate) #speedUpdate() calls function which updates the value of the speed label
        excutor.submit(pressureUpdate) #pressureUpdate() calls function which updates the value of pressure
        excutor.submit(depthUpdate) #depthUpdate() calls function which updates the value of depth
        excutor.submit(clock) #Clock() calls function which initiates the timer
        gui.mainloop() #Letting the window go into a loop so it's always updated and does not close


# videoFromCam()
# speedUpdate()
# pressureUpdate()
# clock()
# depthUpdate()
# gui.mainloop()
start_system()