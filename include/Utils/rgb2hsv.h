#ifndef UTILS_H
#define UTILS_H

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"

namespace utils {

using namespace cv;
const int max_value_H = 179;
const int max_value_S = 255;
const int max_value_V = 255;

int high_H = max_value_H * 0.928;
int low_H = max_value_H * 0;//0.246;
int high_S = max_value_S * 1.00;
int low_S = max_value_S * 0.08;
int high_V = max_value_V * 1.00;
int low_V = max_value_V * 0.00;

// color_tresholder(Mat img_in, Mat img_out)
Mat color_tresholder(const Mat img_in) {

   Mat frame_HSV, frame_BGR, frame_threshold, output;
   // Convert from BGR to HSV colorspace
   cvtColor(img_in, frame_HSV, COLOR_BGR2HSV);
   // Detect the object based on HSV Range Values
   inRange(frame_HSV, Scalar(low_H, low_S, low_V), Scalar(high_H, high_S, high_V), frame_threshold);
   // imshow("frame_threshold", frame_threshold);
   // waitKey();
   bitwise_and(img_in, img_in, output, frame_threshold);
   // imshow("output", output);
   // waitKey();
   return output;
}
}


#endif

