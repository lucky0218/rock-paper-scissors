#include"INC.h"

using namespace std;
using namespace cv;

//debug_output
void print_bgr_difference(Mat& img);


Mat ellipse_detect(Mat );
Mat YCrCb_Otsu_detect(Mat& );
Mat skinDetect(const Mat&, double, double, double, double);
Mat change_contrast(Mat&);
Mat scale_mat(Mat);   //undone
double get_brightness(Mat&);
double get_constrast(Mat&);
