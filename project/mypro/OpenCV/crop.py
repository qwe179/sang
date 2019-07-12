from PIL import Image
import cv2
im=Image.open('data/Lena(600x600).jpg')
cropImage=im.crop((100,100,350,350))
cropImage.show()
cropImage.save('data/Lena-crop.jpg')



src = cv2.imread("data/Lena-crop.jpg",cv2.IMREAD_COLOR)
height, width,channel =src.shape
matrix = cv2.getRotationMatrix2D((width/2, height/2), 90, 1)
dst = cv2.warpAffine(src, matrix, (width, height))
cv2.imshow("src",src)
cv2.imshow("dst",dst)
cv2.waitKey(0)
cv2.destroyAllWindows()
