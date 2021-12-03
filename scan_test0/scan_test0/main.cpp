#include "functions.h"
//#include "INC.h"
using namespace std;
using namespace cv;


int main(int argc, char** argv) {

	Mat image = imread("./pictures/4.jpg");
	Mat mask, img_1, img_ellipse, img_Otsu, img_processed, result;
	//设置的色调范围为160~15之间(320~30)，饱和度范围在25~180之间(0.1~0.65)
	mask = skinDetect(image, 160, 15, 55, 166);
	cv::Mat detected(image.size(), CV_8UC3, cv::Scalar(0, 0, 0));
	//通过copyTo加掩码的方法将原图拷贝到检测图中
	image.copyTo(detected, mask);
	//image.resize((640,480));
	//image = im;
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
	img_ellipse = ellipse_detect(image);
	img_Otsu = YCrCb_Otsu_detect(image);
	img_processed = img_ellipse & img_Otsu & detected;

	//medianBlur(img_processed, result, 3);
	
	imshow("original", image);
	imshow("example1", img_ellipse);
	imshow("example2_Otsu", img_Otsu);
	//imshow("detected", detected);
	

	img_ellipse = ellipse_detect(img_1);
	img_Otsu = YCrCb_Otsu_detect(img_1);
	img_processed = img_ellipse & img_Otsu & detected;

	cout << get_brightness(image) << endl<<get_brightness(img_1);
	
	imshow("example3", img_ellipse);
	imshow("example4", img_Otsu);
	//imshow("detected", detected);

	//imshow("contrast_chaged", img_1);
	//imshow("finest", img_processed);
	//imshow("most excellent", result);
	

	waitKey(0);
	return 0;

}

