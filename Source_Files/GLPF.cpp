#include "function.h"

using namespace cv;
using std::map;

// 高斯低通滤波器
void GLPF(Mat& gray, Mat& result, int fc)
{
	// 扩展图像矩阵，为2，3，5的倍数时运算速度快
	// int m = cv::getOptimalDFTSize(gray.rows);
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
			float d = sqrt(pow((i - row / 2.0), 2) + pow((j - col / 2.0), 2));
			data[j] = exp(-pow(d, 2) / (2 * pow(fc, 2)));
		}
	}
	// 实部和虚部
	Mat plane[] = { padded, Mat::zeros(padded.size(), CV_32FC1) };
	Mat complexIm;
	merge(plane, 2, complexIm); //合并通道 （把两个矩阵合并为一个2通道的Mat类容器）
	dft(complexIm, complexIm);  //进行傅立叶变换，结果保存在自身
	split(complexIm, plane);    //分离通道

	fftshift(plane[0], plane[1]);

	Mat Real, Imag, BLUR;
	Real = plane[0].mul(filter);
	Imag = plane[1].mul(filter);
	Mat plane1[] = { Real, Imag };
	fftshift(Real, Imag); //效果一样 周期性
	merge(plane1, 2, BLUR);//实部与虚部合并

	idft(BLUR, BLUR);
	split(BLUR, plane);                     //分离通道，主要获取通道
	magnitude(plane[0], plane[1], result);  //求幅值(模)
	normalize(result, result, 0, 1.0, NORM_MINMAX);  //归一化便于显示
}
