import cv2
import sys
sys.path.append('build')
import bv


im = cv2.imread('/home/zack/Pictures/people.jpg', cv2.IMREAD_GRAYSCALE)
imfilled = im.copy()
rect_list = bv.get_face(im,1.2)
rects, *_ = rect_list
for rect in rects:
	x, y, w, h = rect
	cv2.line(imfilled, (x, y), (x+w, y+h), (255, 0, 0))
print(rects)
print(rect_list)

cv2.imshow("image", imfilled)
cv2.waitKey(0)



