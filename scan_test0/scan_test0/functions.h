#pragma once
#include"INC.h"

using namespace std;
using namespace cv;

typedef pair<int, int> P;
const double eps = 1e-8;

//debug_output
void print_bgr_difference(Mat&);
void print_enhanced_ellipse_detect(Mat);
void print_noise_processed_enhanced_ellipse_detect(Mat);
void print_processed_brightness(Mat&);
void print_contrast_processed__ellipse_detect(Mat);
void print_it(Mat&);

Mat ellipse_detect(Mat& );
Mat enhanced_ellipse_detect(Mat&);
Mat YCrCb_Otsu_detect(Mat& );
Mat skinDetect(const Mat&, double, double, double, double);
Mat change_contrast(Mat&);
Mat scale_mat(Mat);   //to be improved
vector<P> noise_process(Mat&);
double get_brightness(Mat&);
double get_constrast(Mat&);
double add(double, double);
double ave_distance_begin(int =0,int =0);
double ave_distance_end(int = 0, int = 0);





//math functions
class point;
int max_distance();






//definition of classes
class point {
public:
	double x, y;
	point(const point& p) {
		x = p.x, y = p.y;
	}
	point(double x0=0, double y0=0) {
		x = x0, y = y0;
	}
	point operator +(const point& p)const {
		return point(add(x, p.x),add(y, p.y));
	}
	point operator -(const point& p)const {
		return point(add(x, -p.x),add(y, -p.y));
	}
	point operator *(const double d)const {
		return point(x*d,y*d);
	}

	double dot(point p) {
		return add(x * p.x, y * p.y);
	}
	double det(point p) {
		return add(x * p.y, -y * p.x);
	}
};