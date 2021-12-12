#include"INC.h"

using namespace std;
using namespace cv;

//debug_output
void print_bgr_difference(Mat&);
void print_enhanced_ellipse_detect(Mat);
void print_noise_processed_enhanced_ellipse_detect(Mat);
void print_it(Mat& );

Mat ellipse_detect(Mat& );
Mat enhanced_ellipse_detect(Mat&);
Mat YCrCb_Otsu_detect(Mat& );
Mat skinDetect(const Mat&, double, double, double, double);
Mat change_contrast(Mat&);
Mat scale_mat(Mat);   //to be improved
void noise_process(Mat&);
double get_brightness(Mat&);
double get_constrast(Mat&);
