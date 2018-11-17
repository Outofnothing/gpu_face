import cv2
import sys
sys.path.append('build')
import bv


im = cv2.imread('/home/zack/Pictures/people.jpg', cv2.IMREAD_GRAYSCALE)
imfilled = im.copy()
rects = bv.get_face(im,1.2)
print(rects)


cv2.imshow("Original image", im)
cv2.waitKey(0)



