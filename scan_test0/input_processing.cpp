#include "functions.h"

using namespace std;
using namespace cv;


Mat ellipse_detect(Mat src){
	Mat img = src.clone();
	Mat skinCrCbHist = Mat::zeros(Size(256, 256), CV_8UC1);
	//利用opencv自带的椭圆生成函数先生成一个肤色椭圆模型
	ellipse(skinCrCbHist, Point(113, 155.6), Size(23.4, 15.2), -20.0, 0.0, 360.0, Scalar(255, 255, 255), -1);
	Mat ycrcb_image;
	Mat output_mask = Mat::zeros(img.size(), CV_8UC1);
	cvtColor(img, ycrcb_image, CV_BGR2YCrCb); //首先转换成到YCrCb空间
	for (int i = 0; i < img.cols; i++)   //利用椭圆皮肤模型进行皮肤检测
		for (int j = 0; j < img.rows; j++)
		{
			Vec3b ycrcb = ycrcb_image.at<Vec3b>(j, i);
			if (skinCrCbHist.at<uchar>(ycrcb[1], ycrcb[2]) > 0)   //如果该落在皮肤模型椭圆区域内，该点就是皮肤像素点
				output_mask.at<uchar>(j, i) = 255;
		}

	Mat detect;
	img.copyTo(detect, output_mask);  //返回肤色图
	return detect;
}

Mat YCrCb_Otsu_detect(Mat& src){
	Mat ycrcb_image;
	cvtColor(src, ycrcb_image, CV_BGR2YCrCb); //首先转换成到YCrCb空间
	Mat detect;
	vector<Mat> channels;
	split(ycrcb_image, channels);
	Mat output_mask = channels[1];
	threshold(output_mask, output_mask, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	src.copyTo(detect, output_mask);
	return detect;
}

Mat skinDetect(const cv::Mat& image, double minHue, double maxHue, double minSat, double maxSat){
	//创建HSV图像
	cv::Mat hsv;
	cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);
	std::vector<cv::Mat> channels;
	cv::split(hsv, channels);
	//创建大于最小色调的掩码
	cv::Mat mask1;
	cv::threshold(channels[0], mask1, minHue, 255, cv::THRESH_BINARY);
	//创建小于最大色调的掩码
	cv::Mat mask2;
	cv::threshold(channels[0], mask2, maxHue, 255, cv::THRESH_BINARY_INV);

	//创建色调掩码
	cv::Mat hueMask;
	//如果输入的最小色的小于最大色调，取交
	if (minHue < maxHue)
	{
		hueMask = mask1 & mask2;
	}
	//如果输入的最小色的大于最大色调，即横跨了0度中轴线
	else
	{
		hueMask = mask1 | mask2;
	}

	//创建饱和度掩码
	cv::Mat satMask;
	//由于饱和度没有中轴线，直接调用cv::inRange函数创建二值图即可
	cv::inRange(channels[1], minSat, maxSat, satMask);
	return hueMask & satMask;
}

Mat change_contrast(Mat &src1) {
	int height = src1.rows;//求出src1的高
	int width = src1.cols;//求出src1的宽
	Mat dst = Mat::zeros(src1.size(), src1.type());  //这句很重要，创建一个与原图一样大小的空白图片              
	float alpha =1.1;//调整对比度为1.1
	float beta = 40;//调整亮度加
	
	for (int row = 0; row < height; row++){
		for (int col = 0; col < width; col++){
			if (src1.channels() == 3){
				//判断是否为3通道图片
				//将遍历得到的原图像素值，返回给变量b,g,r
				float b = src1.at<Vec3b>(row, col)[0];//blue
				float g = src1.at<Vec3b>(row, col)[1];//green
				float r = src1.at<Vec3b>(row, col)[2];//red
				//开始操作像素，对变量b,g,r做改变后再返回到新的图片。
				dst.at<Vec3b>(row, col)[0] = saturate_cast<uchar>(b * alpha + beta);
				dst.at<Vec3b>(row, col)[1] = saturate_cast<uchar>(g * alpha + beta);
				dst.at<Vec3b>(row, col)[2] = saturate_cast<uchar>(r * alpha + beta);
			}
		}
	}
	return dst;
}

double get_brightness(Mat& img) {
	//0.2126 * R + 0.7152 * G + 0.0722 * B
	double ans = 0;
	int height = img.rows;
	int width = img.cols;
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			ans += (img.at<Vec3b>(i, j)[0]) * 0.0722 + (img.at<Vec3b>(i, j)[1]) * 0.7152 + (img.at<Vec3b>(i, j)[2]) * 0.2126;
	ans = ans / (double)(height * width);
	return ans;
} 

double get_contrast(Mat& img) {
	srand(0);
	return 0;
}

Mat scale_mat(Mat img) {
	cv::Size dsize = cv::Size(256, 256);
	cv::Mat dst = cv::Mat(dsize, CV_8UC3);
	resize(img, dst,dsize);
	return dst;
}