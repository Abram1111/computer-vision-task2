#ifndef CONTOURPOINT_H
#define CONTOURPOINT_H

#include <opencv2/imgproc/imgproc.hpp>
#include <vector>



class ContourPoint
{
public:
            int row ;
            int col ;
            float energies[7][7][3];
            float total_energies[7][7];
public:
    ContourPoint(int row,int col);
    void draw_point(cv::Mat img,int val=205,int w1=5,int w2=14);
    void calc_energy_distance(int contour_r[10],int contour_c[10],bool shrink=true);
    void calc_energy_deviation(ContourPoint prior_point,ContourPoint next_point);
    void calc_energy_gradient(cv::Mat img,bool to_low=false);
    void add_energies();
    void adjust_point();
    int get_row();
    void set_row(int row);
    int get_col();
    void set_col(int col);
    std::vector<std::vector<float>> norm(float arr[7][7]);



};

#endif // CONTOURPOINT_H
