#ifndef CONTOUR_H
#define CONTOUR_H
#include <bits/stdc++.h>


#include <opencv2/imgproc/imgproc.hpp>



class Contour
{
public:
            int contour[10][2];
            int countour_rows[10];
            int countour_cols[10] ;

            double average_distance;
            //float energies[7][7][3];
public:

    Contour(int contour[10][2]);


    // additional functions
    void rotate(std::deque<int> deq, int d, int n);
    void draw_contour(cv::Mat img, int val = 255, int w1 = 15, int w2 = 2);
    void calc_energies(cv::Mat img);

    // setters and getters
    int  get_contour();
    void set_contour(int countour[10][2]);
    int  get_rows();
    void set_rows(int row[10]);
    int  get_cols();
    void set_cols(int col[10]);
    void get_contour_points();
    void update_points();


};

#endif // CONTOUR_H
