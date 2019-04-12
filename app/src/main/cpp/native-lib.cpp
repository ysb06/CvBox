#include <jni.h>
#include <opencv2/opencv.hpp>

using namespace cv;

extern "C"
JNIEXPORT void JNICALL
Java_com_hfg_cvbox_MainActivity_runCvMain(JNIEnv *env, jobject instance, jlong matAddrInput, jlong matAddrResult, jint mode) {

    Mat &matInput = *(Mat *)matAddrInput;
    Mat &matResult = *(Mat *)matAddrResult;

    Mat dst;

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
    }
}