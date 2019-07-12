import cv2
#from matplotlib import pyplot as plt


def on_press(event,x,y,flags,param):
	global src
	if event==cv2.EVENT_FLAG_LBUTTON:
#	cv2.destroyAllWindows()
#			print('you pressed',event.button,int(event.xdata),int(event.ydata))
		height,width=src.shape
		matrix = cv2.getRotationMatrix2D((width/2, height/2),90, 1)
		src = cv2.warpAffine(src, matrix, (width, height))
		cv2.imshow("dst",src)
	


#fig=plt.figure()
#cid=fig.canvas.mpl_connect('button_press_event',on_press)
#plt.axis('off')
src=cv2.imread('./data/Lena.jpg',cv2.IMREAD_GRAYSCALE)
#plt.imshow(src,cmap='gray',interpolation='bicubic')
#plt.show()
#cv2.namedWindow('dst')
cv2.imshow("dst",src)
cv2.setMouseCallback('dst',on_press)
cv2.waitKey(0)
cv2.destroyAllWindwos()

