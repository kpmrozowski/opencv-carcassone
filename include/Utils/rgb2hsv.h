#ifndef UTILS_H
#define UTILS_H

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include <Utils/display.h>

namespace utils {

using namespace cv;
const int max_value_H = 179;
const int max_value_S = 255;
const int max_value_V = 255;

int high_H = max_value_H * 0.4;
int low_H = max_value_H * 0.05;//0.246;
int high_S = max_value_S * 1.00;
int low_S = max_value_S * 0.1;
int high_V = max_value_V * 1.00;
int low_V = max_value_V * 0.60;


// color_tresholder(Mat img_in, Mat img_out)
Mat color_tresholder(const Mat img_in) {

   Mat frame_HSV, frame_BGR, frame_threshold, closing_dst, erosion_dst, output;
   // Convert from BGR to HSV colorspace
   cvtColor(img_in, frame_HSV, COLOR_BGR2HSV);
   // Detect the object based on HSV Range Values
   inRange(frame_HSV, Scalar(low_H, low_S, low_V), Scalar(high_H, high_S, high_V), frame_threshold);
   
   // Zamykanie maski
   int morph_elem = 0;
   int morph_size = 20;
   int morph_operator = 1;
   int const max_operator = 4;
   int const max_elem = 2;
   int const max_kernel_size = 21;

   Mat element = getStructuringElement( morph_elem, Size( 2*morph_size + 1, 2*morph_size+1 ), cv::Point( morph_size, morph_size ) );
   // Since MORPH_X : 2,3,4,5 and 6
   int operation = morph_operator + 2;
   morphologyEx( frame_threshold, closing_dst, operation, element );

   // Erozja Maski
   int erosion_type = 0;
   int erosion_elem = 0;
   int erosion_size = 2;
   int dilation_elem = 0;
   int dilation_size = 0;
   if( erosion_elem == 0 ) { erosion_type = MORPH_RECT; }
   else if( erosion_elem == 1 ){ erosion_type = MORPH_CROSS; }
   else if( erosion_elem == 2) { erosion_type = MORPH_ELLIPSE; }
   element = getStructuringElement( erosion_type,
                       Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                       cv::Point( erosion_size, erosion_size ) );
   dilate( closing_dst, erosion_dst, element );
   // display("frame_threshold", frame_threshold);
   bitwise_and(img_in, img_in, output, erosion_dst);
   // display("output", output);
   return output;
}
}


#endif

