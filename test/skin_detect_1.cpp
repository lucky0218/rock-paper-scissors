#include<opencv2/opencv.hpp>
#include "opencv2/imgproc/types_c.h" 
#include<iostream>

using namespace std;
using namespace cv;

Mat ellipse_detect(Mat& src)
{
	Mat img = src.clone();
	Mat skinCrCbHist = Mat::zeros(Size(256, 256), CV_8UC1);
	//利用opencv自带的椭圆生成函数先生成一个肤色椭圆模型
	ellipse(skinCrCbHist, Point(113, 155.6), Size(23.4, 15.2), 43.0, 0.0, 360.0, Scalar(255, 255, 255), -1);
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

Mat YCrCb_Otsu_detect(Mat& src)
{
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

Mat skinDetect(const cv::Mat& image, double minHue, double maxHue, double minSat, double maxSat)
{
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

Mat change_contrast(Mat src1){
	int height = src1.rows;//求出src1的高
	int width = src1.cols;//求出src1的宽
	Mat dst = Mat::zeros(src1.size(), src1.type());  //这句很重要，创建一个与原图一样大小的空白图片              
	float alpha = 1.4;//调整对比度为1.5
	float beta = 5;//调整亮度加
	//循环操作，遍历每一列，每一行的元素
	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width; col++)
		{
			if (src1.channels() == 3)//判断是否为3通道图片
			{
				//将遍历得到的原图像素值，返回给变量b,g,r
				float b = src1.at<Vec3b>(row, col)[0];//nlue
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
	




int main(int argc, char** argv) {

	Mat image = imread("D:/git_repo/rock-paper-scissors/test-pictures/3.jpg");
	//Mat nana = image;
	Mat mask,img_1, img_ellipse, img_Otsu, img_processed,result;
	//设置的色调范围为160~15之间(320~30)，饱和度范围在25~180之间(0.1~0.65)
	mask = skinDetect(image, 160, 15, 55, 166);
	cv::Mat detected(image.size(), CV_8UC3, cv::Scalar(0, 0, 0));
	//通过copyTo加掩码的方法将原图拷贝到检测图中
	image.copyTo(detected, mask);
	img_1 = change_contrast(image);
	if (image.empty()) {
		printf("could not load image...\n");
		return -1;

	}
	//pyrDown(image, img);
	//namedWindow("test_opencv_setup", 0);
/*	for (int i = 0; i<image.rows; i++) {
		image.at<Vec3b>(i, 10)[0] = 255;
		image.at<Vec3b>(i, 10)[1] = 0;
		image.at<Vec3b>(i, 10)[2] = 0;
	}*/
	img_ellipse=ellipse_detect(image);
	img_Otsu = YCrCb_Otsu_detect(image);
	img_processed = img_ellipse & img_Otsu & detected;

	//medianBlur(img_processed, result, 3);
	imshow("example1", img_ellipse);
	imshow("example2", img_Otsu);
	imshow("detected", detected);


	img_ellipse = ellipse_detect(img_1);
	img_Otsu = YCrCb_Otsu_detect(img_1);
	img_processed = img_ellipse & img_Otsu & detected;
	imshow("example3", img_ellipse);
	imshow("example4", img_Otsu);
	//imshow("detected", detected);

	imshow("contrast_chaged", img_1);
	//imshow("finest", img_processed);
	//imshow("most excellent", result);

	waitKey(0);
	return 0;

}


