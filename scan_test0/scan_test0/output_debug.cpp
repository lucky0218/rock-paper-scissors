#include "functions.h"

using namespace std;
using namespace cv;

void print_bgr_difference(Mat& img) {

	if (img.empty()) return;
	freopen("rgb.txt", "w", stdout);   
	cout << "图像的高和宽分别是：" << img.rows << "和" << img.cols << endl;
	for (int i = 0; i < img.rows; i++) printf("Col %13d", i);
	cout << endl;
	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			int b = img.at<Vec3b>(i, j)[0];
			int g = img.at<Vec3b>(i, j)[1];
			int r = img.at<Vec3b>(i, j)[2];
			//注意OpenCV中保存形式为BGR，
			printf("B%4dG%4dR%4d, ", b, g, r);
		}
		cout << endl;
	}
	imwrite("./pictures/output_contrast_changed.jpg", img);
}

void print_it(Mat& image) {
	imwrite("./pictures/it.jpg", image);
}

void print_enhanced_ellipse_detect(Mat image) {
	Mat output_mask = enhanced_ellipse_detect(image);
	Mat img_eed;
	image.copyTo(img_eed, output_mask);
	print_processed_brightness(img_eed);
	imwrite("./pictures/enhanced_ellipse_detect.jpg", img_eed);
}

void print_contrast_processed__ellipse_detect(Mat image) {
	image = change_contrast(image);
	Mat output_mask = ellipse_detect(image);
	Mat img_eed;
	image.copyTo(img_eed, output_mask);
	print_processed_brightness(img_eed);
	imwrite("./pictures/contrast_processed__ellipse_detect.jpg", img_eed);
}

void print_noise_processed_enhanced_ellipse_detect(Mat image) {
	freopen("rgb.txt", "w", stdout);
	Mat output_mask = enhanced_ellipse_detect(image);
	noise_process(output_mask);
	cout<<"max distance:"<<max_distance() << endl;
	cout<<"jjj"<<ave_distance_begin()<<"   "<< ave_distance_end()<<endl;
	Mat img_eed;
	image.copyTo(img_eed, output_mask);
	cout << "brightness:"<<get_brightness(img_eed)<<endl;
	imwrite("./pictures/noise_processed_enhanced_ellipse_detect.jpg", img_eed);
}

void print_processed_brightness(Mat& img) {
	cout << "亮度为" << get_brightness(img)<<endl;
}