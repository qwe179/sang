import cv2
import pafy
url='https://www.youtube.com/watch?v=mrIeTpgrZks'
video=pafy.new(url)
print('title=',video.title)
print('video.rating=',video.rating)
print('video.duration=',video.duration)

best=video.getbest(preftype='webm')

cap=cv2.VideoCapture(best.url)
while True:
	retval,frame=cap.read()
	if not retval:
		break
	cv2.imshow('frame',frame)
	gray=cv2.cvtColor(frame,cv2.COLOR_BGR2GRAY)
	cv2.imshow('edges',edges)
	
	key=cv2.waitKey(25)
	if key ==27:
		break;
	
if cap.isOpened():
	cap.release()
cv2.destroyAllWindows()
