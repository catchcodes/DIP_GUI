#include "function.h"
#include <core/core_c.h>

using namespace cv;
using std::map;


// 理想低/高通滤波器
void ILPF(Mat& gray, Mat& result, int fc, bool hpass)
{
	// 扩展图像矩阵，为2，3，5的倍数时运算速度快
	// int m = cv:: getOptimalDFTSize(gray.rows);
	// int n = cv::getOptimalDFTSize(gray.cols);
	Mat padded;
	// 零填充
	// cv::copyMakeBorder(gray, padded, 0, m - gray.rows, 0, n - gray.cols, cv::BORDER_CONSTANT);
	gray.convertTo(padded, CV_32FC1);
	int row = padded.rows;
	int col = padded.cols;
	if (fc > MIN(row, col))	
		throw "截止频率超出图像范围";
	Mat filter = Mat::zeros(padded.size(), CV_32FC1);
	for (int i = 0; i < row; i++)
	{
		float* data = filter.ptr<float>(i);
		for (int j = 0; j < col; j++)
		{
			double d = sqrt(pow((i - row / 2.0), 2) + pow((j - col / 2.0), 2));
			if (hpass)
			{
				data[j] = 1 ? d > fc : 0;
			}
			else
			{
				data[j] = 1 ? d <= fc : 0;
			}
		}
	}
	// imshow("滤波器", filter);
	// 实部和虚部
	Mat plane[] = {padded, Mat::zeros(padded.size(), CV_32FC1)};
	Mat complexIm;
	merge(plane, 2, complexIm); //合并通道 （把两个矩阵合并为一个2通道的Mat类容器）
	dft(complexIm, complexIm);  //进行傅立叶变换，结果保存在自身
	split(complexIm, plane);    //分离通道

	fftshift(plane[0], plane[1]);

	Mat Real, Imag, BLUR;
	Real = plane[0].mul(filter);
	Imag = plane[1].mul(filter);
	fftshift(Real, Imag); //效果一样 周期性
	Mat plane1[] = { Real, Imag };

	merge(plane1, 2, BLUR);//实部与虚部合并

	idft(BLUR, BLUR);
	split(BLUR, plane);                     //分离通道，主要获取通道
	magnitude(plane[0], plane[1], result);  //求幅值(模)
	normalize(result, result, 0, 1.0, NORM_MINMAX);  //归一化便于显示
}

void fftshift(Mat plane0, Mat plane1)
{
	// -2 : 1111_……_1110
	plane0 = plane0(Rect(0, 0, plane0.cols & -2, plane0.rows & -2));
	int cx = plane0.cols / 2;
	int cy = plane0.rows / 2;

	Mat part1_r(plane0, Rect(0, 0, cx, cy));
	Mat part2_r(plane0, Rect(cx, 0, cx, cy));
	Mat part3_r(plane0, Rect(0, cy, cx, cy));
	Mat part4_r(plane0, Rect(cx, cy, cx, cy));

	Mat temp;
	part1_r.copyTo(temp);  //左上与右下交换位置(实部)
	part4_r.copyTo(part1_r);
	temp.copyTo(part4_r);
	part2_r.copyTo(temp);  //右上与左下交换位置(实部)
	part3_r.copyTo(part2_r);
	temp.copyTo(part3_r);

	Mat part1_i(plane1, Rect(0, 0, cx, cy));  //元素坐标(cx,cy)
	Mat part2_i(plane1, Rect(cx, 0, cx, cy));
	Mat part3_i(plane1, Rect(0, cy, cx, cy));
	Mat part4_i(plane1, Rect(cx, cy, cx, cy));

	part1_i.copyTo(temp);  //左上与右下交换位置(虚部)
	part4_i.copyTo(part1_i);
	temp.copyTo(part4_i);
	part2_i.copyTo(temp);  //右上与左下交换位置(虚部)
	part3_i.copyTo(part2_i);
	temp.copyTo(part3_i);
}
