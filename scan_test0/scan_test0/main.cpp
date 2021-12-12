#include "functions.h"

using namespace std;
using namespace cv;


int main() {

	Mat image = imread("./pictures/2.jpg");
	Mat mask, img_1, img_ellipse, img_Otsu, img_processed, result,output_mask,img_ellip;
	//设置的色调范围为160~15之间(320~30)，饱和度范围在25~180之间(0.1~0.65)
	mask = skinDetect(image, 160, 15, 55, 166);
	Mat detected(image.size(), CV_8UC3, cv::Scalar(0, 0, 0));
	//通过copyTo加掩码的方法将原图拷贝到检测图中
	image.copyTo(detected, mask);
	
	img_1 = change_contrast(image);
	//image = change_contrast(image);
	if (image.empty()) {
		printf("could not load image...\n");
		return -1;
	}
	image = scale_mat(image);
	print_enhanced_ellipse_detect(image);
	print_noise_processed_enhanced_ellipse_detect(image);
	//imwrite("./pictures/output_before.jpg", img_ellipse);

	img_Otsu = YCrCb_Otsu_detect(image);
//	img_processed = img_ellipse & img_Otsu;
	
	//medianBlur(img_processed, result, 3);
	//imshow("detected", detected);

	Mat img_ellipse_1 = ellipse_detect(img_1);
	img_Otsu = YCrCb_Otsu_detect(img_1);
//	img_processed = img_ellipse & img_Otsu & detected;

	cout << get_brightness(image) << endl<<get_brightness(img_1);
	print_bgr_difference(img_ellipse);
	
	//imshow("example3_brightness_changed", img_ellipse_1);
	//imshow("example4", img_Otsu);
	//imshow("detected", detected);
	//imshow("contrast_chaged", img_1);
	waitKey(0);
	return 0;
}

