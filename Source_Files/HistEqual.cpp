#include "function.h"

using cv::Mat;
using std::map;

// 直方图均衡化的定义
void HistEqual(Mat& gray, Mat& result)
{
    // 哈希表统计0~255像素值的个数
    map<int, int>pixelCounter;
    for (int i = 0; i < gray.rows; i++)
    {
        uchar* grayptr = gray.ptr<uchar>(i);
        for (int j = 0; j < gray.cols; j++)
        {
            int value = grayptr[j];
            pixelCounter[value]++;
        }
    }
    //统计0~255像素值的频率，并计算累计频率
    map<int, double> pixel_fre;
    int pixel_sum = gray.cols * gray.rows;
    double cumul_fre = 0;
    for (int i = 0; i < 256; i++)
    {
        // 累计频率 哪些灰度数量多，映射曲线越陡峭，均衡后越均匀
        cumul_fre += double(pixelCounter[i]) / pixel_sum;
        pixel_fre[i] = cumul_fre;
    }
    //根据累计频率进行转换
    for (int i = 0; i < gray.rows; i++)
    {
        uchar* grayptr1 = gray.ptr<uchar>(i);
        uchar* resultptr = result.ptr<uchar>(i);
        for (int j = 0; j < gray.cols; j++)
        {
            int value = grayptr1[j];
            double fre = pixel_fre[value];
            // 原始灰度值乘以累计频率
            resultptr[j] = fre * value;
        }
    }
}
