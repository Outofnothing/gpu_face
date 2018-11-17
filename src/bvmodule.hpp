#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

namespace gpu_face
{

	CV_EXPORTS_W void get_face(Mat &image, OutputArray &rect_matrix, double);
}
