#!/opt/local/bin/python
# -*- coding: utf-8 -*-
import cv2

cam = cv2.VideoCapture("nvarguscamerasrc ! video/x-raw(memory:NVMM), width=(int)1280, height=(int)720,format=(string)NV12, framerate=(fraction)24/1 ! nvvidconv flip-method=2 ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink")

def Rotate(src, degrees):
    if degrees == 90:
        dst = cv2.transpose(src)
        dst = cv2.flip(dst, 1)

    elif degrees == 180:
        dst = cv2.flip(src, -1)

    elif degrees == 270:
        dst = cv2.transpose(src)
        dst = cv2.flip(dst, 0)
    else:
        dst = null
    return dst

CAM_ID = 0

if cam.isOpened() == False: 
    print 'Can\'t open the CAM(%d)' % (CAM_ID)
    exit()

width = cam.get(cv2.CAP_PROP_FRAME_WIDTH)
height = cam.get(cv2.CAP_PROP_FRAME_HEIGHT)
print 'size = [%f, %f]\n' % (width, height)


cv2.namedWindow('CAM_RotateWindow')

while(True):
    ret, frame = cam.read()
    
    
    img = Rotate(frame, 180) #90 or 180 or 270

    
    cv2.imshow('CAM_RotateWindow', img)


    if cv2.waitKey(10) >= 0:
        break;

cam.release()
cv2.destroyWindow('CAM_OriginalWindow')

cv2.destroyWindow('CAM_RotateWindow')
