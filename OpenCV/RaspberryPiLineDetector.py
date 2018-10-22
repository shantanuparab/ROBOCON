# -*- coding: utf-8 -*-
"""
Created on Mon Oct 22 11:39:31 2018

@author: DELL
"""
#from picamera.array import PiRGBArray
#from picamera import PiCamera
import numpy as np
import cv2

#cam = PiCamera()
#cam.resolution = (160,128)
#cam.framerate = 32
#rawCap = PiRGBArray(cam,size=(160,128))

vid = cv2.VideoCapture(1)
vid.set(3,160)
vid.set(4,128)
hsv = None
def mousePressed(evt,x,y,flag,param):
    global hsv
    if(evt == cv2.EVENT_LBUTTONDOWN):
        print(hsv[y][x])
#for frame in cam.capture_continuous(rawCap,format="bgr",use_video_port=True):
#    frame= frame.array
while vid.isOpened():
    ret,frame = vid.read()
    print(ret)
    cv2.imshow("frame3",frame)
    crop = frame[60:128,0:160]
    #cv2.imshow("crop",crop)
    hsv = cv2.cvtColor(crop,cv2.COLOR_BGR2HSV)
    #cv2.imshow("hsv",hsv)
    #lower = np.array([0,100,50],dtype = np.uint8)
    #higher = np.array([200,250,200],dtype = np.uint8)
    lower = np.array([0,0,220],dtype = np.uint8)
    higher = np.array([200,60,255],dtype = np.uint8)
    ranged = cv2.inRange(hsv,lower,higher)
    #cv2.imshow("ranged",ranged)
    cv2.setMouseCallback("crop",mousePressed)
    blur = cv2.GaussianBlur(ranged,(5,5),0)
    #cv2.imshow("blur",blur)
    im2,contours,hierarchy = cv2.findContours(blur.copy(), 1, cv2.CHAIN_APPROX_NONE)
    if(len(contours)>0):
        c = max(contours,key=cv2.contourArea)
        m = cv2.moments(c)
        
        cx = int(m['m10']/m['m00'])
        cy = int(m['m01']/m['m00'])
        
        cv2.line(crop,(cx,0),(cx,720),(0,0,255),1)
        cv2.line(crop,(0,cy),(1280,cy),(0,0,255),1)        
        
        cv2.drawContours(crop,contours,-1,(0,0,255),1)
        #print(cx,cy)
        cv2.imshow("crop1",crop)
        if cx >= 90:
            print("Turn Right!")
        if cx < 90 and cx > 70:
            print("On Track!")
        if cx <= 70:
            print("Turn Left!")
    #rawCap.truncate(0)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
#vid.release()
cv2.destroyAllWindows()
