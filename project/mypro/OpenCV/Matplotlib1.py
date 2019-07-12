import cv2

from matplotlib import pyplot as plt

imageFile='./data/lena.jpg'
img=cv2.imread(imageFile)

plt.axis('off')
imgRGB=cv2.cvtColor(img,cv2.COLOR_BGR2RGB)
plt.imshow(imgRGB)
plt.show()
