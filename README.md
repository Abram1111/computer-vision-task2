# Assignment 2: Edge and Boundary Detection

* This assignment aims to implement two main tasks in image processing, edge detection and boundary detection, using the Canny edge detector, Hough transform, and SNAKE algorithm.


## Task Description
* For all given images, the Canny edge detector will be used to detect edges. Then, lines, circles, and ellipses in these images will be detected using the Hough transform. Finally, the detected shapes will be superimposed on the images.
* For given images, the Active Contour Model (SNAKE) will be used to initialize the contour for a given object. Then, the greedy algorithm will be used to evolve the contour. The output will be represented as chain code, and the perimeter and area inside these contours will be computed.


## Implementation Details
* The assignment will be implemented using C++ programming language and the GUI will be developed using the Qt framework.


## Requirements
* C++ compiler (GCC or Clang)
* Qt framework (version 5 or higher)
* OpenCV library (version 3 or higher)


## How to Run
* Clone the repository or download the source code.
* Go to this file -> .\a02-team_10\desktop_application\release
* Run the executable file -> CV_TASK_2.
* Select an image to process.
* The output will be displayed on the screen.


## Important notes:
* You should implement these tasks without depending on OpenCV library or alike. However you can use the OpenCV Canny Edge Detector as preprocessing to Hough Transform.
* Plagiarizing lines will not be tolerated.
* You can start from the source code you delivered from the previous task so that you give your program a new version with new features so that by the end of the semester you will have your own application with multiple computer vision algorithms up and running. (optional)
* Don't forget to upload the report via github.
