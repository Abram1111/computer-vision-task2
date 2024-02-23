#include <bits/stdc++.h>
#include "contourpoint.h"
#include "countour.h"

// constructor
using namespace std;

Contour::Contour(int contour[10][2])
{



    //int row[10];
    //int col[10];

    deque<int> temp_r ;
    deque<int> temp_c ;

    for (int index =0;index<10;index++)
    {
        this->countour_rows[index]=contour[index][0];
        this->countour_cols[index]=contour[index][1];
        temp_r.push_back(contour[index][0]);
        temp_c.push_back(contour[index][1]);


    }
    //this->countour_rows=row;
    //this->countour_cols=col;

    // circular shifting
    rotate(temp_r, 1, 10);
    rotate(temp_c, 1, 10);

    //calculating average distance
    double average = 0;
    double distance = 0;

    for (int index =0;index<10;index++)
    {
        distance=(this->countour_rows[index]-temp_r[index])*(this->countour_rows[index]-temp_r[index]) +(this->countour_cols[index]-temp_c[index])*(this->countour_cols[index]-temp_c[index]);
        distance=pow(distance,0.5);
        average+=distance;

    }
    average = average/100;
    this->average_distance=average;



}
cv::Point cv_create_Point(int x, int y) {
    return *new cv::Point(x, y);
}
void Contour::draw_contour (cv::Mat img, int val ,int w1 ,int w2 )
{
    for (int i=0; i<10;i++)
    {
        ContourPoint point (this->countour_rows[i], this->countour_cols[i]);
        point.draw_point(img);
        cv::Point poi=cv_create_Point(point.row, point.col);
        cv::Point poin=cv_create_Point(this->countour_rows[i+1], this->countour_cols[i+1]);
        cv::line(img, poi ,poin,0, 2, cv::LINE_AA);
    }
}


void Contour::calc_energies(cv::Mat img )
{
    ContourPoint prior_point(0, 0);
    ContourPoint test(0, 0);

    for (int i = 0; i < 10; i++)

    {
        ContourPoint next_point(this->countour_rows[i+1], this->countour_cols[i+1]);

        ContourPoint point(this->countour_rows[i], this->countour_cols[i]);

        if (i==0)
                {

                    prior_point.row = this->countour_rows[9];
                    prior_point.col = this->countour_cols[9];

                }
                else
                {
                    prior_point.row = this->countour_rows[i-1];
                    prior_point.col = this->countour_cols[i-1];

                }

//        point.energies = {{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},
//            {0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},
//            {0,0,0,0,0,0,0}};


        for(int i=0;i<7;i++){
            for(int j=0;j<7;j++){
                for(int l=0;l<3;l++){
                    point.energies[i][j][l]=0;
                }
            }
        }

        point.calc_energy_distance (this->countour_rows, this->countour_cols);
        point.calc_energy_deviation(prior_point, next_point);
        point.calc_energy_gradient (img);
        point.add_energies();
        test=point;
    }
}

// additional functions

void Contour::rotate (deque<int> deq,int d, int n)
{
    // Push first d elements
    // from last to the beginning
    for (int i = 0; i < d; i++)
    {
        int val = deq.back();
        deq.pop_back();
        deq.push_front(val);
    }

}


void Contour::update_points()
{
    //int row[10];
    //int col[10];

    deque<int> temp_r;
    deque<int> temp_c;

    for (int index = 0; index < 10; index++)

    {
        ContourPoint point(this->countour_rows[index], this->countour_cols[index]);
        point.adjust_point();

        this->countour_rows[index] = point.row;
        this->countour_cols[index] = point.col;

        temp_r.push_back(point.row);
        temp_c.push_back(point.col);

    }

    // circular shifting
    rotate(temp_r, 1, 10);
    rotate(temp_c, 1, 10);

    //calculating average distance
    double average = 0;
    double distance = 0;

    for (int index = 0; index < 10; index++)
    {
        distance = (this->countour_rows[index] - temp_r[index])*(this->countour_rows[index] - temp_r[index]) + (this->countour_cols[index] - temp_c[index])*(this->countour_cols[index] - temp_c[index]);
        distance = pow(distance, 0.5);
        average += distance;
    }
    average = average / 100;
    this->average_distance = average;
}

void Contour::get_contour_points(){
//    def get_contour_points(self):
//            # print("*******************")
//            contour=[]
//            for i in range(len(self.contour_r)):
//                contour.append((self.contour_r[i],self.contour_c[i]))
//            # print(contour)
//            return contour

    int contour_points[10][2];
    for (int i=0;i<10;i++){

        contour_points[i][0]=countour_rows[i];
        contour_points[i][1]=countour_cols[i];
    }

//    vector<vector<cv::Point>> contours;
//    findContours(img.clone(), contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE); // Retrieve only external contour
//    double len2 = arcLength(contours[0], true);
}
