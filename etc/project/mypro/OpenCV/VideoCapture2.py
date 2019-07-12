import cv2

cap=cv2.VideoCapture('http://192.168.1.59:4747/video')

frame_size=(int(cap.get(cv2.CAP_PROP_FRAME_WIDTH)),
int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT)))
print('frame_size=',frame_size)

while True:
	retval,frame=cap.read()
	if not retval:
		break
	cv2.imshow('frame',frame)

	key=cv2.waitKey(25)
	if key ==27:
		break;
	
if cap.isOpened():
	cap.release()
cv2.destroyAllWindows()
