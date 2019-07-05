#include <jni.h>
#include <opencv2/opencv.hpp>
#include <android/log.h>

using namespace cv;
using namespace std;

void detectHand(Mat &src, Mat &output);

extern "C"
JNIEXPORT void JNICALL
Java_com_hfg_cvbox_MainActivity_runCvMain(JNIEnv *env, jobject instance, jlong matAddrInput, jlong matAddrResult, jint mode) {

    Mat &matInput = *(Mat *)matAddrInput;
    Mat &matResult = *(Mat *)matAddrResult;

    Mat dst;
    int mean = 0;

    switch (mode) {
        case 0: {
            cvtColor(matInput, matResult, COLOR_RGBA2RGB);
            break;
        }
        case 1: {
            cvtColor(matInput, matResult, COLOR_RGBA2GRAY);
            break;
        }
        case 2: {
            cvtColor(matInput, dst, COLOR_RGBA2GRAY);
            matResult = 255 - dst;
            break;
        }
        case 3: {
            cvtColor(matInput, dst, COLOR_RGBA2RGB);
            matResult = Scalar(255, 255, 255) - dst;
            break;
        }
        case 4: {
            cvtColor(matInput, dst, COLOR_RGBA2RGB);
            matResult = dst + Scalar(100, 100, 100);
            break;
        }
        case 5: {
            cvtColor(matInput, dst, COLOR_RGBA2GRAY);
            for (int i = 0; i < matInput.rows; i++) {
                for (int j = 0; j < matInput.cols; j++) {
                    mean += matInput.at<uchar>(i, j);
                }
            }
            mean = mean / (matInput.rows * matInput.cols);
            matResult = matInput + Scalar(128 - mean, 128 - mean, 128 - mean);
            break;
        }
        case 6: {
            detectHand(matInput, matResult);
            break;
        }
    }
}

void detectHand(Mat &src, Mat &output) {
    Mat frame;
    cvtColor(src, frame, COLOR_BGR2YCrCb);
    inRange(frame, Scalar(0, 77, 133), Scalar(255, 127, 173), frame);
    erode(frame, frame, Mat(3, 3, CV_8U, Scalar(1)), Point(-1, -1), 2);

    vector<vector<Point>> contours;
    vector<Vec4i> contoursHierarchy;

    findContours(frame, contours, contoursHierarchy, RETR_CCOMP, ContourApproximationModes::CHAIN_APPROX_SIMPLE);

    cvtColor(frame, output, COLOR_GRAY2BGR);
    Scalar color_red = Scalar(255, 0, 0);
    Scalar color_blue = Scalar(0, 0, 255);

    vector<vector<Point>> hull(contours.size());
    vector<int> ihull(contours.size());
    vector<Vec4i> defects(contours.size());
    for(int i = 0; i < contours.size(); i++) {
        convexHull(Mat(contours[i]), hull[i]);
        convexHull(Mat(contours[i]), ihull);
        if(hull[i].size() > 3 )
        {
            convexityDefects(contours[i], ihull, defects[i]);
        }
    }
    __android_log_print(ANDROID_LOG_INFO, "CV_NDK", "Get Defects");

    for(int i = 0; i < contours.size(); i++) {
        drawContours(output, contours, i, color_blue, 2, LINE_8, contoursHierarchy);
        drawContours(output, hull, i, color_red, 2);
        //circle(output, hull[i][1], 10, Scalar(255, 0, 255), 10);

        __android_log_print(ANDROID_LOG_INFO, "CV_NDK", "Draw Defects");
        //*
        for(const Vec4i& v : defects)
        {
            float depth = v[3] / 256;
            if (depth > 10) //  filter defects by depth, e.g more than 10
            {
                int startidx = v[0]; Point ptStart(contours[i][startidx]);
                int endidx = v[1]; Point ptEnd(contours[i][endidx]);
                int faridx = v[2]; Point ptFar(contours[i][faridx]);

                line(output, ptStart, ptEnd, Scalar(0, 255, 0), 1);
                line(output, ptStart, ptFar, Scalar(0, 255, 0), 1);
                line(output, ptEnd, ptFar, Scalar(0, 255, 0), 1);
                circle(output, ptFar, 4, Scalar(0, 255, 255), 5);
            }
        }
        //*/
    }

    /*
    vector<Point2f> approx;
    for(size_t i = 0; i < contours.size(); i++) {
        approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true) * 0.01, true);

        //__android_log_print(ANDROID_LOG_INFO, "CV_NDK", "Area: %f", fabs(contourArea(approx)));
        if(fabs(contourArea(approx)) > 100000) {
            for (int k = 0; k < approx.size(); k++)
                circle(output, approx[k], 15, color_red, 3);
        }
    }
    */
}