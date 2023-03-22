#include "function.h"

#define PY_SSIZE_T_CLEAN

using namespace std;
using namespace cv;

void faceFilter(Mat& face, int fc, const char*filepath)
{
    // 运行face.py得到特征点位置
    vector<int> location_x;
    vector<int> location_y;
    runPython(location_x, location_y, filepath);

    int numOfFace = location_x.size() / 81;
    if (numOfFace == 0)
        return;
    else
    {
        for (int i = 0; i < numOfFace; i++)
        {
            Rect leftEyeRect(location_x[i * 81 + 17], location_y[i * 81 + 19], location_x[i * 81 + 21] - location_x[i * 81 + 17], location_y[i * 81 + 41] - location_y[i * 81 + 19]);
            Mat leftEye = face(leftEyeRect);
            Mat le = leftEye.clone();
            Rect rightEyeRect(location_x[i * 81 + 22], location_y[i * 81 + 24], location_x[i * 81 + 26] - location_x[i * 81 + 22], location_y[i * 81 + 46] - location_y[i * 81 + 24]);
            Mat rightEye = face(rightEyeRect);
            Mat re = rightEye.clone();
            Rect mouthRect(location_x[i * 81 + 48], location_y[i * 81 + 51], location_x[i * 81 + 54] - location_x[i * 81 + 48], location_y[i * 81 + 57] - location_y[i * 81 + 51]);
            Mat mouth = face(mouthRect);
            Mat mo = mouth.clone();
            Rect facRect(location_x[i * 81], location_y[i * 81 + 69], location_x[i * 81 + 16] - location_x[i * 81], location_y[i * 81 + 8] - location_y[i * 81 + 69]);
            Mat fac = face(facRect);

            int channels = fac.channels();
            Mat imageRGB[3];
            split(fac, imageRGB);
            // 分别进行高斯低通滤波
            for (int i = 0; i < channels; i++)
                GLPF(imageRGB[i], imageRGB[i], fc);

            // 合并通道
            merge(imageRGB, 3, fac);
            fac.convertTo(fac, CV_8U, 255);
            fac.copyTo(face(facRect));
            le.copyTo(face(leftEyeRect));
            re.copyTo(face(rightEyeRect));
            mo.copyTo(face(mouthRect));
        }
    }    
}