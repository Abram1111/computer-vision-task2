#include "contourpoint.h"
#include <iostream>
#include<cmath>
using namespace std;


ContourPoint::ContourPoint(int row,int col)
{
    this->col=col;
    this->row=row;
}

void ContourPoint::draw_point(cv::Mat img,int val,int w1,int w2){
    for(int i=(row-w1)/2;i<(row+w1)/2+1;i++){
        for (int j=(col-w2)/2;j<(col+w2)/2+1;j++){
            img.at<int>(i,j)=val;
        }
    }
    for(int i=(row-w2)/2;i<(row+w2)/2+1;i++){
        for (int j=(col-w1)/2;j<(col+w1)/2+1;j++){
            img.at<int>(i,j)=val;
        }
    }

}


void ContourPoint:: calc_energy_distance(int contour_r[10],int contour_c[10],bool shrink){

    int r =row, c=col,sum=0;
    int contsum[10];

    float energy_distance[7][7];
    for(int i=-3; i<4; i++){
        for(int j=-3;j<4;j++){
            for (int k=0;k<10;k++)
            {
                contsum[k]=((r+i)*(r+i)-2*(r+i)*contour_r[k]+contour_r[k]*contour_r[k] )+((c+j)*(c+j)-2*(c+j)*contour_c[k]+contour_c[k]*contour_c[k]);
                sum += contsum[k];
            }

            energy_distance[i+3][j+3] = sum;
        }
    }
    vector<vector<float>> normalized;
//    cout<<"dis: ";
        normalized=norm(energy_distance);

        for(int i=0;i<7;i++){
            for(int j=0;j<7;j++){
                energies[i][j][0]=normalized[i][j];
            }
        }

}

void ContourPoint::calc_energy_deviation(ContourPoint prior_point,ContourPoint next_point){

    int r =row, c=col;
    int d2next,d2prior;
    float energy_deviation[7][7];
    for(int i=-3; i<4; i++){
        for(int j=-3;j<4;j++){
            d2next=((r+i)*(r+i)-2*(r+i)*next_point.row+next_point.row*next_point.row)+((c+j)*(c+j)-2*(c+j)*next_point.col+next_point.row*next_point.row);
            d2prior=((r+i)*(r+i)-2*(r+i)*prior_point.row+prior_point.row*prior_point.row)+((c+j)*(c+j)-2*(c+j)*prior_point.col+prior_point.row*prior_point.row);

            energy_deviation[i+3][j+3] = pow(abs(d2next-d2prior),2);

        }
    }
//    cout<<"dev: ";
    vector<vector<float>> normalized;
    normalized=norm(energy_deviation);

    for(int i=0;i<7;i++){
        for(int j=0;j<7;j++){

            energies[i][j][1]=normalized[i][j];
        }
    }
}

void ContourPoint::calc_energy_gradient(cv::Mat img,bool to_low){

//    cout << "M = " << endl << " "  << img << endl << endl;
    int r =row, c=col;
    float energy_gradient[7][7];
    for(int i=-3; i<4; i++){
        for(int j=-3;j<4;j++){

            if(!isnan(img.at<float>(r+i,c+j)))
            {
                energy_gradient[i+3][j+3]=(float)img.at<float>(r+i,c+j)*(float)img.at<float>(r+i,c+j);
            }
            else{energy_gradient[i+3][j+3]=0;}


        }
    }

    vector<vector<float>> normalized;
//    cout<<"grad: ";
    normalized=norm(energy_gradient);

    for(int i=0;i<7;i++){
        for(int j=0;j<7;j++){
            energies[i][j][2]=normalized[i][j];
        }
    }
}

void ContourPoint:: add_energies(){
//    cout<<"sum";
    for(int i=0;i<7;i++){
        for(int j=0;j<7;j++){
            total_energies[i][j]=energies[i][j][0] + energies[i][j][1] + energies[i][j][2];
//            cout<<"dist:"<<energies[i][j][0]<<" devi: " << energies[i][j][1]<<" grad " << energies[i][j][2]<<" tot "<<total_energies[i][j]<<"  \n " ;
        }
        }
}



void ContourPoint::adjust_point(){
    int minimum[2] ={0,0},min=0;

    for(int i=0;i<7;i++){
       for(int j=0;j<7;j++){
           if(total_energies[i][j] < min){
               min=total_energies[i][j];
              minimum[0] =i;
              minimum[1]=j;
           }
        }
    }


    row += minimum[0] - 3;
    col += minimum[1] - 3;



}



vector<vector<float>>  ContourPoint:: norm(float arr[7][7]){

        vector<vector<float>> out;
        vector<float>v2;
        float max=0;

        for(int i=0;i<7;i++){
            for(int j=0;j<7;j++)
            {
                if (arr[i][j]>max)
                {max=arr[i][j];}

            }
        }


        for(int i=0;i<7;i++){
            for(int j=0;j<7;j++){
                v2.push_back(arr[i][j]/max);
            }
            out.push_back(v2);
            v2.clear();
        }


        return out;


}

void ContourPoint::set_row(int row){
    this->row=row;
}

int ContourPoint::get_row(){
    return this->row;
}

void ContourPoint::set_col(int col){
    this->col=col;
}

int ContourPoint::get_col(){
    return this->col;
}

