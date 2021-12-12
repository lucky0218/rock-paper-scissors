#include "functions.h"

using namespace std;
using namespace cv;

void print_bgr_difference(Mat& img) {

	if (img.empty()) return;
	freopen("rgb_1.txt", "w", stdout);   
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
	imwrite("./pictures/noise_processed_enhanced_ellipse_detect.jpg", image);
}
void print_enhanced_ellipse_detect(Mat image) {
	Mat output_mask = enhanced_ellipse_detect(image);
	Mat img_eed;
	image.copyTo(img_eed, output_mask);
	imwrite("./pictures/enhanced_ellipse_detect.jpg", img_eed);
}

void print_noise_processed_enhanced_ellipse_detect(Mat image) {
	Mat output_mask = enhanced_ellipse_detect(image);
	noise_process(output_mask);
	Mat img_eed;
	image.copyTo(img_eed, output_mask);
	imwrite("./pictures/noise_processed_enhanced_ellipse_detect.jpg", img_eed);
}