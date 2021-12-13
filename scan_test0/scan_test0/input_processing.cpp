#include "functions.h"

using namespace std;
using namespace cv;


Mat ellipse_detect(Mat &src){
	Mat img = src.clone();
	Mat skinCrCbHist = Mat::zeros(Size(256, 256), CV_8UC1);
	//����opencv�Դ�����Բ���ɺ���������һ����ɫ��Բģ��
	ellipse(skinCrCbHist, Point(113, 155.6), Size(23.4, 15.2), -20.0, 0.0, 360.0, Scalar(255, 255, 255), -1);
	Mat ycrcb_image;
	Mat output_mask = Mat::zeros(img.size(), CV_8UC1);
	cvtColor(img, ycrcb_image, CV_BGR2YCrCb); //����ת���ɵ�YCrCb�ռ�
	for (int i = 0; i < img.cols; i++)   //������ԲƤ��ģ�ͽ���Ƥ�����
		for (int j = 0; j < img.rows; j++)
		{
			Vec3b ycrcb = ycrcb_image.at<Vec3b>(j, i);
			if (skinCrCbHist.at<uchar>(ycrcb[1], ycrcb[2]) > 0)   //���������Ƥ��ģ����Բ�����ڣ��õ����Ƥ�����ص�
				output_mask.at<uchar>(j, i) = 255;
		}
	return output_mask;
}


Mat enhanced_ellipse_detect(Mat &src) {
	Mat img = src.clone();
	Mat skinCrCbHist = Mat::zeros(Size(256, 256), CV_8UC1);
	ellipse(skinCrCbHist, Point(113, 155.6), Size(23.4, 15.2), -20.0, 0.0, 360.0, Scalar(255, 255, 255), -1);
	Mat ycrcb_image;
	Mat output_mask = Mat::zeros(img.size(), CV_8UC1);
	cvtColor(img, ycrcb_image, CV_BGR2YCrCb); 
	for (int i = 0; i < img.cols; i++) 
		for (int j = 0; j < img.rows; j++)
		{
			Vec3b ycrcb = ycrcb_image.at<Vec3b>(j, i);
			if (skinCrCbHist.at<uchar>(ycrcb[1], ycrcb[2]) > 0) 
				output_mask.at<uchar>(j, i) = 255;
		}

	for (int i = 0; i < img.cols; i++)
		for (int j = 0; j < img.rows; j++) {
			if (src.at<Vec3b>(j, i)[0] < 40) output_mask.at<uchar>(j, i) = 0;
		}

	return output_mask;
}

Mat YCrCb_Otsu_detect(Mat& src){
	Mat ycrcb_image;
	cvtColor(src, ycrcb_image, CV_BGR2YCrCb); //����ת���ɵ�YCrCb�ռ�
	Mat detect;
	vector<Mat> channels;
	split(ycrcb_image, channels);
	Mat output_mask = channels[1];
	threshold(output_mask, output_mask, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	src.copyTo(detect, output_mask);
	return detect;
}

Mat skinDetect(const cv::Mat& image, double minHue, double maxHue, double minSat, double maxSat){
	//����HSVͼ��
	cv::Mat hsv;
	cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);
	std::vector<cv::Mat> channels;
	cv::split(hsv, channels);
	//����������Сɫ��������
	cv::Mat mask1;
	cv::threshold(channels[0], mask1, minHue, 255, cv::THRESH_BINARY);
	//����С�����ɫ��������
	cv::Mat mask2;
	cv::threshold(channels[0], mask2, maxHue, 255, cv::THRESH_BINARY_INV);

	//����ɫ������
	cv::Mat hueMask;
	//����������Сɫ��С�����ɫ����ȡ��
	if (minHue < maxHue)
	{
		hueMask = mask1 & mask2;
	}
	//����������Сɫ�Ĵ������ɫ�����������0��������
	else
	{
		hueMask = mask1 | mask2;
	}

	//�������Ͷ�����
	cv::Mat satMask;
	//���ڱ��Ͷ�û�������ߣ�ֱ�ӵ���cv::inRange����������ֵͼ����
	cv::inRange(channels[1], minSat, maxSat, satMask);
	return hueMask & satMask;
}

Mat change_contrast(Mat &src1) {
	int height = src1.rows;
	int width = src1.cols;
	Mat dst = Mat::zeros(src1.size(), src1.type());  //������Ҫ������һ����ԭͼһ����С�Ŀհ�ͼƬ              
	float alpha =1.2;//�����Աȶ�Ϊ1.2
	float beta = 50;//�������ȼ�
	
	for (int row = 0; row < height; row++){
		for (int col = 0; col < width; col++){
			if (src1.channels() == 3){
				//�ж��Ƿ�Ϊ3ͨ��ͼƬ
				//�������õ���ԭͼ����ֵ�����ظ�����b,g,r
				float b = src1.at<Vec3b>(row, col)[0];//blue
				float g = src1.at<Vec3b>(row, col)[1];//green
				float r = src1.at<Vec3b>(row, col)[2];//red
				//��ʼ�������أ��Ա���b,g,r���ı���ٷ��ص��µ�ͼƬ��
				dst.at<Vec3b>(row, col)[0] = saturate_cast<uchar>(b * alpha + beta);
				dst.at<Vec3b>(row, col)[1] = saturate_cast<uchar>(g * alpha + beta);
				dst.at<Vec3b>(row, col)[2] = saturate_cast<uchar>(r * alpha + beta);
			}
		}
	}
	return dst;
}

double get_brightness(Mat& img,int add) {
	//0.2126 * R + 0.7152 * G + 0.0722 * B
	double ans = 0;
	int height = img.rows, width = img.cols;
	int tot = 0;
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++) {
			tot += img.at<Vec3b>(i, j)[0] > 0;
			ans += (img.at<Vec3b>(i, j)[0]) * 0.0722 + (img.at<Vec3b>(i, j)[1]) * 0.7152 + (img.at<Vec3b>(i, j)[2]) * 0.2126;
		}
			
	ans = ans / (double)(tot);
	return ans;
} 

double get_contrast(Mat& img) {
	srand(0);
	return 0;
}

Mat scale_mat(Mat img) {
	Size dsize = cv::Size(75, 100);
	Mat dst = cv::Mat(dsize, CV_8UC3);
	resize(img, dst,dsize);
	return dst;
}