
#!/opt/local/bin/python
# -*- coding: utf-8 -*-
import cv2

CAM_ID = 0

cam = cv2.VideoCapture("nvarguscamerasrc ! video/x-raw(memory:NVMM), width=(int)1280, height=(int)720,format=(string)NV12, framerate=(fraction)24/1 ! nvvidconv flip-method=2 ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink")
if cam.isOpened() == False:
    print 'Can\'t open the CAM(%d)' % (CAM_ID)
    exit()

width = cam.get(cv2.CAP_PROP_FRAME_WIDTH);
height = cam.get(cv2.CAP_PROP_FRAME_HEIGHT);
print 'size = [%f, %f]\n' % (width, height)

cv2.namedWindow('CAM_Window')
cv2.resizeWindow('CAM_Window', 1280, 720)

while(True):
    ret, frame = cam.read()

    cv2.imshow('CAM_Window', frame)

    if cv2.waitKey(10) >= 0:
        break;
        
cam.release()

cv2.destroyWindow('CAM_Window')
