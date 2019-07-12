import cv2
import matplotlib.pyplot as plt
from PIL import Image
def on_press(event):
	Img=Im.rotate(90)
	Img.show()
	
	


Im=Image.open('data/Lena.jpg')
Im.show()
fig=plt.figure()
cid=fig.canvas.mpl_connect('button_press_event',on_press)

