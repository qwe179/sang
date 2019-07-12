import cv2

from matplotlib import pyplot as plt
from PIL import Image
im1=Image.open('orange.jpg')
img1_re=im1.resize((600,600))
img1_re.save('orange(600x600).jpg')

im2=Image.open('apple.jpg')
img2_re=im2.resize((600,600))
img2_re.save('apple(600x600).jpg')

im3=Image.open('baboon.jpg')
img3_re=im3.resize((600,600))
img3_re.save('baboon(600x600).jpg')

im4=Image.open('Lena.jpg')
img4_re=im4.resize((600,600))
img4_re.save('Lena(600x600).jpg')



path='./data/'
imgBGR1=cv2.imread(path+'orange(600x600).jpg')
imgBGR2=cv2.imread(path+'apple(600x600).jpg')
imgBGR3=cv2.imread(path+'baboon(600x600).jpg')
imgBGR4=cv2.imread(path+'Lena(600x600).jpg')


imgRGB1=cv2.cvtColor(imgBGR1,cv2.COLOR_BGR2RGB)
imgRGB2=cv2.cvtColor(imgBGR2,cv2.COLOR_BGR2RGB)
imgRGB3=cv2.cvtColor(imgBGR3,cv2.COLOR_BGR2RGB)
imgRGB4=cv2.cvtColor(imgBGR4,cv2.COLOR_BGR2RGB)

fig,ax=plt.subplots(2,2,figsize=(10,10),sharey=True)
fig.canvas.set_window_title('Sample Pictures')



ax[0][0].axis('off')
ax[0][0].imshow(imgRGB1,aspect='auto')

ax[0][1].axis('off')
ax[0][1].imshow(imgRGB2,aspect='auto')

ax[1][0].axis('off')
ax[1][0].imshow(imgRGB3,aspect='auto')

ax[1][1].axis('off')
ax[1][1].imshow(imgRGB4,aspect="auto")

plt.subplots_adjust(left=0,bottom=0,right=1,top=1,wspace=0.05,hspace=0.05)
plt.savefig("./data/0206.png",bbox_inches='tight')
plt.show()
