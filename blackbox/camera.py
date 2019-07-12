import cv2
gst_str=('nvarguscamerasrc!''video/x-raw(memoty:NVMM),''width=640,height=480,''format=NV12,framerate=30/1!''nvvidconv !''nvegltransform ! nveglglessink -e')
capture = cv2.VideoCapture(gst_str)
capture.set(cv2.CAP_PROP_FRAME_WIDTH,640)
capture.set(cv2.CAP_PROP_FRAME_HEIGHT,480)

while True:
    ret,frame=capture.read()
    cv2.imshow("VideoFrame",frame)
    if cv2.waitKey(1) > 0:break

capture.release()
cv2.destroyAllWindows()

