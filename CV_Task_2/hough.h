#ifndef HOUGH_H
#define HOUGH_H


#include <opencv2/opencv.hpp>
#include<vector>
using namespace cv;
using namespace std;

Mat Gaussian_filter(Mat img, int dimention, float sigma);
Mat convolve2(Mat image, Mat kernel);
Mat sobel_edge(Mat img);
double find_max(Mat mag);
Mat non_max(Mat image, Mat angles);
Mat canny_threshold(Mat img, double lowThresholdRatio, double highThresholdRatio);
Mat hestressis(Mat image);
Mat canny_edge(Mat image);
Mat generate_gaussian_kernal(int dimention, float sigma);
void findNonZero(const Mat& image, vector<int>& x_idxs, vector<int>& y_idxs);
vector<pair<float, float>> hough_line_transform(Mat& image, int angle_step = 1, int threshold = 100);
Mat apply_hough(int thresold);
vector<RotatedRect> detectEllipses(Mat edges, float threshold);
Mat Apply_Ellipse(Mat image,float threshold);
Mat hough_circle(Mat& image, int threshold, int minRad, int maxRad);
Mat Apply_circle(Mat& image, int threshold, int minRad, int maxRad);
#endif // HOUGH_H
