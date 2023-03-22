#ifndef Function_H
#define Function_H

#include <opencv2/opencv.hpp>
#include <vector>

using std::vector;
using cv::Mat;

// 显示vector中的数据
void showVector(vector<int>& x, bool console = false);

// 直方图均衡化函数的声明
void HistEqual(Mat& gray, Mat& result);

// 拉普拉斯算子
void Laplacian(Mat& gray, Mat& result, int padding=0);

// 频移
void fftshift(Mat plane0, Mat plane1);

// 理想低/高通滤波器
void ILPF(Mat& gray, Mat& result, int fc, bool hpass);

// level阶巴特沃斯低通滤波器
void BLPF(Mat& gray, Mat& result, int fc, int level=2);

// 高斯低通滤波器
void GLPF(Mat& gray, Mat& result, int fc);

// 运行python程序
void runPython(vector<int>& location_x, vector<int>& location_y, const char* filepath);

// 面部滤波
void faceFilter(Mat& face, int fc, const char*filepath);

#endif
