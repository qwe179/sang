import sys
import cv2

def read_cam():
    cap = cv2.VideoCapture("nvarguscamerasrc ! video/x-raw(memory:NVMM), width=(int)1280, height=(int)720,format=(string)NV12, framerate=(fraction)24/1 ! nvvidconv flip-method=2 ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink")
    if cap.isOpened():
        cv2.namedWindow("demo", cv2.WINDOW_AUTOSIZE)
        cv2.namedWindow('CAM_RotateWindow')
        while True:
            ret_val, img = cap.read();
            img =Rotate(frame,180)
            cv2.imshow('demo',img)
            cv2.waitKey(10)
    else:
     print "camera open failed"

    cv2.destroyAllWindows()

def Rotate(src,degrees):
    if degrees==90:
        dst=cv2.transpose(src)
        dst=cv2.flip(dst,1)

    elif degrees==180:
        dst=cv2.flip(src,0)

    elif degrees==280:
        dst = cv2.transpose(scr)
        dst = cv2.flip(dst,0)
    else:
        dst=bull
        return dst




if __name__ == '__main__':
    read_cam()
