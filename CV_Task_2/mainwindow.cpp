#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "hough.h"
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
#include <iostream>
#include <vector>
#include <contourpoint.h>
#include <countour.h>

using namespace cv;
using namespace std;

Mat image;
int hough_threshold = 100;
int max_radius=20;


template<typename T>
std::vector<double> linspace(T start_in, T end_in, int num_in)
{

  std::vector<double> linspaced;

  double start = static_cast<double>(start_in);
  double end = static_cast<double>(end_in);
  double num = static_cast<double>(num_in);

  if (num == 0) { return linspaced; }
  if (num == 1)
    {
      linspaced.push_back(start);
      return linspaced;
    }

  double delta = (end - start) / (num - 1);

  for(int i=0; i < num-1; ++i)
    {
      linspaced.push_back(start + delta * i);
    }
  linspaced.push_back(end);
  return linspaced;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString directory = QFileDialog::getOpenFileName(this,
                                                     tr("Find Files"), QDir::homePath(), "Image files (*.jpg *.png)");
    String name = directory.toStdString();
    image = imread(name);
    QPixmap pix(directory);

    ui->label_2->setPixmap(pix.scaled(550, 700, Qt::KeepAspectRatio));

}

void MainWindow::on_pushButton_2_clicked()
{
    Mat result = apply_hough(hough_threshold);
    imwrite("final.png", result);
    QPixmap pix2("final.png");
    ui->label->setPixmap(pix2.scaled(550, 700, Qt::KeepAspectRatio));
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    hough_threshold = value;
}

void MainWindow::on_pushButton_4_clicked()
{
    float threshold;
      threshold = (float)hough_threshold / 10000;

      Mat result = Apply_circle(image, hough_threshold, 10, max_radius);
      imwrite("final.png", result);
      QPixmap pix2("final.png");
      ui->label->setPixmap(pix2.scaled(550, 700, Qt::KeepAspectRatio));
}

void MainWindow::on_pushButton_10_clicked()
{
    float threshold;
    threshold = ((float)hough_threshold + 200) / 1000;
    Mat result = Apply_Ellipse(image, threshold);
    imwrite("final.png", result);
    QPixmap pix2("final.png");
    ui->label->setPixmap(pix2.scaled(550, 700, Qt::KeepAspectRatio));
}

void MainWindow::on_pushButton_5_clicked()
{
    QString directory = QFileDialog::getOpenFileName(this,
                                                     tr("Find Files"), QDir::homePath(), "Image files (*.jpg *.png)");
    String name = directory.toStdString();
    image = imread(name,IMREAD_GRAYSCALE);
    QPixmap pix(directory);
    ui->label_14->setPixmap(pix.scaled(400, 400, Qt::KeepAspectRatio));

}


void MainWindow::on_pushButton_6_clicked()
{
//    cv::resize(image,image,cv::Size(970,958),INTER_LINEAR);
//   Mat canny_image,padded;
//   Canny(image,canny_image, 50, 150, 3);
//   copyMakeBorder(canny_image,padded,2,2,2,2,BORDER_CONSTANT,Scalar(255));
//   vector<double> init=linspace(0.0, 2 * M_PI, 10);

//   vector<array<int, 2> > intial_contour;
//   for (double d : init){

//       intial_contour.push_back({(int)(485 + 400 * cos(d)), (int)(485 + 400 * sin(d))});

//   }
//   int init_cont[10][2];

//     cout<<endl;
//     for (int i=0;i<10;i++){
//         init_cont[i][0]=(int)(485 + 400 * cos(init[i]));
//         init_cont[i][1]=(int)(485 + 400 * sin(init[i]));

//     }
//     Contour contour(init_cont);
//     Mat output,loop;
//     cout<<"hhhjbj";
//     for (int i=0;i<10;i++){
//         loop=padded.clone();
//         contour.calc_energies(loop);
//         contour.update_points();
//         contour.draw_contour(loop);

//    }
//   contour.get_contour_points();
//   imwrite("final.png", loop);
//   QPixmap pix2("final.png");
//   ui->label_15->setPixmap(pix2.scaled(400, 400, Qt::KeepAspectRatio));

   }






void MainWindow::on_horizontalSlider_2_valueChanged(int value)
{
    max_radius=value;

}

