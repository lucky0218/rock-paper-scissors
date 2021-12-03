#include"INC.h"

using namespace std;
using namespace cv;

Mat ellipse_detect(Mat& );
Mat YCrCb_Otsu_detect(Mat& );
Mat skinDetect(const Mat&, double, double, double, double);
Mat change_contrast(Mat);
double get_brightness(Mat&);
Mat scale_mat(Mat);   //undone