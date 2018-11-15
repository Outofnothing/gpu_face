// WARNING: this sample is under construction! Use it on your own risk.
#if defined _MSC_VER && _MSC_VER >= 1400
#pragma warning(disable : 4100)
#endif


#include <iostream>
#include <iomanip>
#include "bvmodule.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/cudaobjdetect.hpp"
#include "opencv2/cudaimgproc.hpp"
#include "opencv2/cudawarping.hpp"

using namespace std;
using namespace cv;
using namespace cv::cuda;

namespace gpu_face {

    static void convertAndResize(const Mat &src, Mat &gray, Mat &resized, double scale) {
        if (src.channels() == 3) {
            cv::cvtColor(src, gray, COLOR_BGR2GRAY);
        } else {
            gray = src;
        }

        Size sz(cvRound(gray.cols * scale), cvRound(gray.rows * scale));

        if (scale != 1) {
            cv::resize(gray, resized, sz);
        } else {
            resized = gray;
        }
    }

    static void convertAndResize(const GpuMat &src, GpuMat &gray, GpuMat &resized, double scale) {
        if (src.channels() == 3) {
            cv::cuda::cvtColor(src, gray, COLOR_BGR2GRAY);
        } else {
            gray = src;
        }

        Size sz(cvRound(gray.cols * scale), cvRound(gray.rows * scale));

        if (scale != 1) {
            cv::cuda::resize(gray, resized, sz);
        } else {
            resized = gray;
        }
    }
    void get_face(cv::Mat &image, cv::OutputArray rect_matrix) {
        if (getCudaEnabledDeviceCount() == 0) {
           cout << "No GPU found or the library is compiled without CUDA support" << endl;
        }

        cv::cuda::printShortCudaDeviceInfo(cv::cuda::getDevice());
        string cascadeName;
        
        cascadeName = "/home/zack/datasets/cascade/face.xml";
        

        Ptr<cuda::CascadeClassifier> cascade_gpu = cuda::CascadeClassifier::create(cascadeName);
        cv::CascadeClassifier cascade_cpu;
        if (!cascade_cpu.load(cascadeName)) {
            cout << "ERROR: Could not load cascade classifier \"" << cascadeName << "\"" << endl;
        }
        CV_Assert(!image.empty());
        namedWindow("result", 1);

        Mat frame, frame_cpu, gray_cpu, resized_cpu, frameDisp;
        vector<Rect> faces;

        GpuMat frame_gpu, gray_gpu, resized_gpu, facesBuf_gpu;

        /* parameters */
        bool useGPU = true;
        double scaleFactor = 1.0;
        bool findLargestObject = false;
        bool filterRects = true;
        bool helpScreen = false;

        (image.empty() ? frame : image).copyTo(frame_cpu);
        frame_gpu.upload(image.empty() ? frame : image);

        convertAndResize(frame_gpu, gray_gpu, resized_gpu, scaleFactor);
        convertAndResize(frame_cpu, gray_cpu, resized_cpu, scaleFactor);

        TickMeter tm;
        tm.start();

        if (useGPU) {
            cascade_gpu->setFindLargestObject(findLargestObject);
            cascade_gpu->setScaleFactor(1.2);
            cascade_gpu->setMinNeighbors((filterRects || findLargestObject) ? 4 : 0);

            cascade_gpu->detectMultiScale(resized_gpu, facesBuf_gpu);
            cascade_gpu->convert(facesBuf_gpu, faces);
        }

        for (size_t i = 0; i < faces.size(); ++i) {
            rectangle(resized_cpu, faces[i], Scalar(255));
        }

        tm.stop();
        double detectionTime = tm.getTimeMilli();
        double fps = 1000 / detectionTime;

        //print detections to console
        cout << setfill(' ') << setprecision(2);
        cout << setw(6) << fixed << fps << " FPS, " << faces.size() << " det";
        //rect_matrix = faces;
        if ((filterRects || findLargestObject) && !faces.empty()) {
            for (size_t i = 0; i < faces.size(); ++i) {

                cout << ", [" << setw(4) << faces[i].x
                     << ", " << setw(4) << faces[i].y
                     << ", " << setw(4) << faces[i].width
                     << ", " << setw(4) << faces[i].height << "]";
            }
        }
        cout << endl;
        cv::cvtColor(resized_cpu, frameDisp, COLOR_GRAY2BGR);
        imshow("result", frameDisp);

    }
}
