#include <QString>
#include <QFileDialog>
#include <QFile>
#include <opencv2/opencv.hpp>
#include <string.h>
#include <QDir>
#include <QPixmap>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include "hough.h"
#include <vector>
using namespace cv;
using namespace std;

extern Mat image;
Mat generate_gaussian_kernal(int dimention, float sigma)
{

    Mat result = Mat::zeros(dimention, dimention, CV_64FC1);
    int xf = dimention / 2;
    int yf;
    if (dimention % 2 == 0)
    {
        yf = xf;
    }
    else
    {
        yf = xf + 1;
    }
    double r, s = 2.0 * sigma * sigma;
    // sum is for normalization
    double sum = 0.0;

    // generating 5x5 kernel
    for (int x = -xf; x < yf; x++)
    {
        for (int y = -xf; y < yf; y++)
        {
            r = sqrt(x * x + y * y);
            result.at<double>((x + 2), (y + 2)) = (exp(-(r * r) / s)) / (M_PI * s);
            sum += result.at<double>((x + 2), (y + 2));
        }
    }

    // normalising the Kernel
    for (int i = 0; i < 5; ++i)
        for (int j = 0; j < 5; ++j)
            result.at<double>(i, j) /= sum;
    return result;
}
Mat Gaussian_filter(Mat img, int dimention, float sigma)
{
    Mat kernel = generate_gaussian_kernal(dimention, sigma);
    return convolve2(img, kernel);
}

Mat convolve2(Mat image, Mat kernel)
{
    int x = image.rows;
    int y = image.cols;
    Mat result = Mat::zeros((x), (y), CV_64FC1);
    cv::filter2D(image, result, CV_64F, kernel);
    return result;
}
Mat sobel_edge(Mat img)
{
    Mat kernel_x = (Mat_<char>(3, 3) << -1, 0, 1, -2, 0, 2, -1, 0, 1);
    Mat kernel_y = (Mat_<char>(3, 3) << -1, -2, -1, 0, 0, 0, 1, 2, 1);
    Mat image_x = convolve2(img, kernel_x);
    Mat image_y = convolve2(img, kernel_y);
    Mat final_img = Mat::zeros(img.rows, img.cols, CV_64FC1);
    for (int i = 0; i < img.rows; ++i)
    {
        for (int j = 0; j < img.cols; ++j)
        {
            final_img.at<double>(i, j) = sqrt(pow(image_x.at<double>(i, j), 2) + pow(image_y.at<double>(i, j), 2));
        }
    }
    return final_img;
}

double find_max(Mat mag)
{
    double max = 0;
    for (int i = 0; i < mag.rows; ++i)
    {
        for (int j = 0; j < mag.cols; ++j)
        {
            if (mag.at<double>(i, j) > max)
            {
                max = mag.at<double>(i, j);
            }
        }
    }

    return max;
}
Mat non_max(Mat image, Mat angles)
{
    Mat result = Mat::zeros(image.rows, image.cols, CV_64FC1);
    angles *= 180 / M_PI;
    for (int i = 0; i < angles.rows; ++i)
    {
        for (int j = 0; j < angles.cols; ++j)
        {
            if (angles.at<double>(i, j) < 0)
            {
                angles.at<double>(i, j) += 180;
            }
        }
    }
    double q, r;
    for (int i = 1; i < (image.rows - 1); ++i)
    {
        for (int j = 1; j < (image.cols - 1); ++j)
        {

            if ((0 < angles.at<double>(i, j) && angles.at<double>(i, j) < 22.5) || (157.5 < angles.at<double>(i, j) && angles.at<double>(i, j) <= 180))
            {
                q = image.at<double>(i, j + 1);
                r = image.at<double>(i, j - 1);
            }
            else if (22.5 <= angles.at<double>(i, j) && angles.at<double>(i, j) < 67.5)
            {
                q = image.at<double>(i + 1, j - 1);
                r = image.at<double>(i - 1, j + 1);
            }
            else if (67.5 <= angles.at<double>(i, j) && angles.at<double>(i, j) < 112.5)
            {
                q = image.at<double>(i + 1, j);
                r = image.at<double>(i - 1, j);
            }

            else if (112.5 <= angles.at<double>(i, j) && angles.at<double>(i, j) < 157.5)
            {
                q = image.at<double>(i - 1, j - 1);
                r = image.at<double>(i + 1, j + 1);
            }
            if ((image.at<double>(i, j) >= q) && (image.at<double>(i, j) >= r))
            {
                result.at<double>(i, j) = image.at<double>(i, j);
            }
            else
            {
                result.at<double>(i, j) = 0;
            }
        }
    }
    return result;
}
Mat canny_threshold(Mat img, double lowThresholdRatio, double highThresholdRatio)
{

    double highThreshold = find_max(img) * highThresholdRatio;
    double lowThreshold = highThreshold * lowThresholdRatio;
    Mat result = Mat::zeros(img.rows, img.cols, CV_64FC1);
    int weak = 20;
    int strong = 255;
    for (int i = 0; i < img.rows; ++i)
    {
        for (int j = 0; j < img.cols; ++j)
        {
            if (img.at<double>(i, j) >= highThreshold)
            {
                result.at<double>(i, j) = strong;
            }
            else if ((img.at<double>(i, j) < highThreshold) && (img.at<double>(i, j) >= lowThreshold))
            {
                result.at<double>(i, j) = weak;
            }
        }
    }
    return result;
}
Mat hestressis(Mat image)
{
    for (int i = 1; i < image.rows; ++i)
    {
        for (int j = 1; j < image.cols; ++j)
        {
            if (image.at<double>(i, j) == 20)
            {

                if ((image.at<double>(i + 1, j - 1) == 255) ||
                    (image.at<double>(i + 1, j) == 255) ||
                    (image.at<double>(i + 1, j + 1) == 255) ||
                    (image.at<double>(i, j - 1) == 255) ||
                    (image.at<double>(i, j + 1) == 255) ||
                    (image.at<double>(i - 1, j - 1) == 255) ||
                    (image.at<double>(i - 1, j) == 255) ||
                    (image.at<double>(i - 1, j + 1) == 255))
                {
                    image.at<double>(i, j) = 255;
                }
                else
                {
                    image.at<double>(i, j) = 0;
                }
            }
        }
    }
    return image;
}
Mat canny_edge(Mat image)
{
    Mat result = Gaussian_filter(image, 5, .5);
    Mat kernel_x = (Mat_<char>(3, 3) << -1, 0, 1, -2, 0, 2, -1, 0, 1);
    Mat kernel_y = (Mat_<char>(3, 3) << -1, -2, -1, 0, 0, 0, 1, 2, 1);
    Mat image_x = convolve2(result, kernel_x);
    Mat image_y = convolve2(result, kernel_y);
    Mat mag = Mat::zeros(result.rows, result.cols, CV_64FC1);
    for (int i = 0; i < result.rows; ++i)
    {
        for (int j = 0; j < result.cols; ++j)
        {
            mag.at<double>(i, j) = sqrt(pow(image_x.at<double>(i, j), 2) + pow(image_y.at<double>(i, j), 2));
        }
    }
    mag /= find_max(mag);
    mag *= 255;
    Mat angles = Mat::zeros(mag.rows, mag.cols, CV_64FC1);

    for (int i = 0; i < result.rows; ++i)
    {
        for (int j = 0; j < result.cols; ++j)
        {
            angles.at<double>(i, j) = atan(image_y.at<double>(i, j) / image_x.at<double>(i, j));
        }
    }
    result = non_max(mag, angles);
    Mat fianl_result = canny_threshold(result, .05, .09);
    fianl_result = hestressis(fianl_result);
    return fianl_result;
}

/************************************************************************/
void findNonZero(const Mat &image, vector<int> &x_idxs, vector<int> &y_idxs)
{
    CV_Assert(image.type() == CV_8UC1);
    x_idxs.clear();
    y_idxs.clear();
    for (int y = 0; y < image.rows; y++)
    {
        const uchar *row_ptr = image.ptr<uchar>(y);
        for (int x = 0; x < image.cols; x++)
        {
            if (row_ptr[x] != 0)
            {
                x_idxs.push_back(x);
                y_idxs.push_back(y);
            }
        }
    }
}

vector<pair<float, float>> hough_line_transform(Mat &image, int angle_step, int threshold)
{
    int height = image.rows;
    int width = image.cols;
    double diagonal = sqrt(pow(height, 2) + pow(width, 2));
    vector<float> rhos;
    for (double i = -diagonal; i <= diagonal; i += 1)
    {
        rhos.push_back(i);
    }
    vector<float> thetas;
    for (int i = -90; i < 90; i += angle_step)
    {
        thetas.push_back(i * CV_PI / 180.0);
    }
    int num_thetas = thetas.size();
    vector<float> cos_t(num_thetas);
    vector<float> sin_t(num_thetas);
    for (int i = 0; i < num_thetas; i += 1)
    {
        cos_t[i] = cos(thetas[i]);
        sin_t[i] = sin(thetas[i]);
    }
    int accumulator_rows = 2 * diagonal;
    int accumulator_cols = num_thetas;
    Mat accumulator(accumulator_rows, accumulator_cols, CV_8UC1, Scalar(0));
    vector<int> y_idxs;
    vector<int> x_idxs;
    findNonZero(image, x_idxs, y_idxs);
    for (int i = 0; i < x_idxs.size(); i += 1)
    {
        int x = x_idxs[i];
        int y = y_idxs[i];
        for (int t_idx = 0; t_idx < num_thetas; t_idx += 1)
        {
            int rho = round(x * cos_t[t_idx] + y * sin_t[t_idx] + diagonal);
            accumulator.at<uchar>(rho, t_idx) += 1;
        }
    }
    vector<pair<float, float>> lines;
    for (int y = 0; y < accumulator_rows; y += 1)
    {
        for (int x = 0; x < accumulator_cols; x += 1)
        {
            if (accumulator.at<uchar>(y, x) > threshold)
            {
                float rho = rhos[y];
                float theta = thetas[x];
                lines.push_back(make_pair(rho, theta));
            }
        }
    }
    return lines;
}
Mat apply_hough(int thresold)
{
    Mat edges;
    Canny(image, edges, 50, 150, 3);
    vector<pair<float, float>> lines = hough_line_transform(edges, 1, thresold);
    cv::Mat result = image.clone();
    for (int i = 0; i < lines.size(); i += 1)
    {
        float rho = lines[i].first;
        float theta = lines[i].second;
        float a = cos(theta);
        float b = sin(theta);
        float x0 = a * rho;
        float y0 = b * rho;
        float x1 = x0 + 1000 * (-b);
        float y1 = y0 + 1000 * a;
        float x2 = x0 - 1000 * (-b);
        float y2 = y0 - 1000 * a;
        Point pt1(x1, y1);
        Point pt2(x2, y2);
        line(result, pt1, pt2, Scalar(0, 0, 255), 2, LINE_AA);
    }
    return result;
}

vector<RotatedRect> detectEllipses(Mat edges, float threshold)
{

    // Find contours
    vector<vector<Point>> contours;
    findContours(edges, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

    // Fit ellipses to contours
    vector<RotatedRect> ellipses;
    for (size_t i = 0; i < contours.size(); i++)
    {
        if (contours[i].size() >= 5)
        {
            RotatedRect ellipse = fitEllipse(contours[i]);
            double score = fabs(1 - (double)ellipse.size.width / ellipse.size.height);
            if (score < threshold)
            {
                ellipses.push_back(ellipse);
            }
        }
    }

    return ellipses;
}
Mat Apply_Ellipse(Mat image, float threshold)
{
    Mat grayImage, edgeImage;
    vector<RotatedRect> ellipses;
    // Load the image
    // Convert the image to grayscale
    cvtColor(image, grayImage, COLOR_BGR2GRAY);

    // Apply canny edge detection
    Canny(grayImage, edgeImage, 50, 100, 3);

    // Detect ellipses
    ellipses = detectEllipses(edgeImage, threshold);

    // Draw ellipses on the image
    Mat dst = image.clone();
    for (size_t i = 0; i < ellipses.size(); i++)
    {
        ellipse(dst, ellipses[i], Scalar(0, 255, 0), 3, LINE_AA);
    }
    return dst;
}
Mat hough_circle(Mat& image, int threshold, int minRad, int maxRad){
    cvtColor(image, image, COLOR_BGR2GRAY);
    Mat edges;
    Canny(image, edges, 50, 150, 3);
    int height = edges.rows; //Get the Height of the image
    int width = edges.cols; //Get the Width of the image
    //Theta
    int theta_range = 360;
    float deg_to_radian = M_PI / 180.0;
    //Get the accumulator
    vector<vector<vector<int>>> accumulator(width, vector<vector<int>>(height, vector<int>((maxRad-minRad+1),0)));
    for(int y = 0; y<height; y++){
        for(int x = 0; x<width; x++){
            if(edges.at<uchar>(y, x) !=0){
               for(int radius = minRad; radius<maxRad; radius++){
                   for(int theta = 0 ; theta<theta_range; theta++){
                       double y_not = y - radius * sin(theta * deg_to_radian);
                       double x_not = x - radius * cos(theta * deg_to_radian);
                       if(x_not<width && x_not>=0 && y_not<height && y_not>=0){
                           accumulator[(int)x_not][(int)y_not][radius-minRad]++;
                       }
                   }
               }
            }
        }
    }

    //Loop over the accumulator to get the circle based on their threshold value
    Mat circles = image.clone();
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            for  (int r = minRad; r < maxRad; r++)
            {
                if (accumulator[x][y][r-minRad] >= threshold)
                {
                    circle(circles, Point(x , y ), r - minRad, Scalar(0, 0, 255), 1, LINE_AA);
                }
            }
        }
    }
    return circles;
}
Mat Apply_circle(Mat& image, int threshold, int minRad , int maxRad ){
    Mat img;
    cvtColor(image, img, COLOR_BGR2GRAY);
    Mat edges;
    Canny(img, edges, 50, 150, 3);
    int height = edges.rows; //Get the Height of the image
    int width = edges.cols; //Get the Width of the image
    //Theta
    int theta_range = 360;
    float deg_to_radian = M_PI / 180.0;
    //Get the accumulator
    vector<vector<vector<int>>> accumulator(width, vector<vector<int>>(height, vector<int>((maxRad-minRad+1),0)));
    for(int y = 0; y<height; y++){
        for(int x = 0; x<width; x++){
            if(edges.at<uchar>(y, x) !=0){
               for(int radius = minRad; radius<maxRad; radius++){
                   for(int theta = 0 ; theta<theta_range; theta++){
                       double y_not = y - radius * sin(theta * deg_to_radian);
                       double x_not = x - radius * cos(theta * deg_to_radian);
                       if(x_not<width && x_not>=0 && y_not<height && y_not>=0){
                           accumulator[(int)x_not][(int)y_not][radius-minRad]++;
                       }
                   }
               }
            }
        }
    }

    //Loop over the accumulator to get the circle based on their threshold value
    Mat circles = image.clone();
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            for  (int r = minRad; r < maxRad; r++)
            {
                if (accumulator[x][y][r-minRad] >= threshold)
                {
                    circle(circles, Point(x , y ), r - minRad, Scalar(0, 0, 255), 1, LINE_AA);
                }
            }
        }
    }
    return circles;
}
