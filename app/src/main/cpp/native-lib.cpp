#include <jni.h>
#include <opencv2/opencv.hpp>

using namespace cv;

extern "C"
JNIEXPORT void JNICALL
Java_com_hfg_cvbox_MainActivity_runCvMain(JNIEnv *env, jobject instance, jlong matAddrInput, jlong matAddrResult, jint mode) {

    Mat &matInput = *(Mat *)matAddrInput;
    Mat &matResult = *(Mat *)matAddrResult;

    Mat dst;
    int mean = 0;

    switch (mode) {
        case 0:
            cvtColor(matInput, matResult, COLOR_RGBA2RGB);
            break;
        case 1:
            cvtColor(matInput, matResult, COLOR_RGBA2GRAY);
            break;
        case 2:
            cvtColor(matInput, dst, COLOR_RGBA2GRAY);
            matResult = 255 - dst;
            break;
        case 3:
            cvtColor(matInput, dst, COLOR_RGBA2RGB);
            matResult = Scalar(255, 255, 255) - dst;
            break;
        case 4:
            cvtColor(matInput, dst, COLOR_RGBA2RGB);
            matResult = dst + Scalar(100, 100, 100);
            break;
        case 5:
            cvtColor(matInput, dst, COLOR_RGBA2GRAY);
            for(int i = 0; i < matInput.rows; i++) {
                for(int j = 0; j < matInput.cols; j++) {
                    mean += matInput.at<uchar>(i, j);
                }
            }
            mean = mean / (matInput.rows * matInput.cols);
            matResult = matInput + Scalar(128 - mean, 128 - mean, 128 - mean);
            break;
    }
}