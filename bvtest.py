import cv2
import sys
sys.path.append('build')
import bv


im = cv2.imread('/home/zack/Pictures/walkers.jpg', cv2.IMREAD_GRAYSCALE)
imfilled = im.copy()
rects = bv.get_face(im)
print(rects)


cv2.imshow("Original image", im)
cv2.waitKey(0)



