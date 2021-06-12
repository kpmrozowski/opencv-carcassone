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
const String window_capture_name = "Video Capture";
const String window_detection_name = "Object Detection";
static void on_low_H_thresh_trackbar(int, void *)
{
    low_H = min(high_H-1, low_H);
    setTrackbarPos("Low H", window_detection_name, low_H);
}
static void on_high_H_thresh_trackbar(int, void *)
{
    high_H = max(high_H, low_H+1);
    setTrackbarPos("High H", window_detection_name, high_H);
}
static void on_low_S_thresh_trackbar(int, void *)
{
    low_S = min(high_S-1, low_S);
    setTrackbarPos("Low S", window_detection_name, low_S);
}
static void on_high_S_thresh_trackbar(int, void *)
{
    high_S = max(high_S, low_S+1);
    setTrackbarPos("High S", window_detection_name, high_S);
}
static void on_low_V_thresh_trackbar(int, void *)
{
    low_V = min(high_V-1, low_V);
    setTrackbarPos("Low V", window_detection_name, low_V);
}
static void on_high_V_thresh_trackbar(int, void *)
{
    high_V = max(high_V, low_V+1);
    setTrackbarPos("High V", window_detection_name, high_V);
}

// color_tresholder(Mat img_in, Mat img_out)
Mat color_tresholder(Mat img_in) {
//    createTrackbar("Low H", window_detection_name, &low_H, max_value_H, on_low_H_thresh_trackbar);
//    createTrackbar("High H", window_detection_name, &high_H, max_value_H, on_high_H_thresh_trackbar);
//    createTrackbar("Low S", window_detection_name, &low_S, max_value_S, on_low_S_thresh_trackbar);
//    createTrackbar("High S", window_detection_name, &high_S, max_value_S, on_high_S_thresh_trackbar);
//    createTrackbar("Low V", window_detection_name, &low_V, max_value_V, on_low_V_thresh_trackbar);
//    createTrackbar("High V", window_detection_name, &high_V, max_value_V, on_high_V_thresh_trackbar);

   Mat frame_HSV, frame_BGR, frame_threshold, output;
   // Convert from BGR to HSV colorspace
   cvtColor(img_in, frame_HSV, COLOR_BGR2HSV);
   // Detect the object based on HSV Range Values
   inRange(frame_HSV, Scalar(low_H, low_S, low_V), Scalar(high_H, high_S, high_V), frame_threshold);
   bitwise_and(img_in, img_in, output, frame_threshold);
   return output;
}
}


#endif

