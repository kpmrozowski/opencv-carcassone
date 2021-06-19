#ifndef UTILS_H
#define UTILS_H

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include <Utils/display.h>

namespace utils {

using namespace cv;

Mat get_element(int type, int size) {
   return getStructuringElement( type, Size( 2*size + 1, 2*size+1 ), cv::Point( size, size ) );;
}

// color_tresholder(Mat img_in, Mat img_out)
Mat color_tresholder(const Mat img_in) {

   const int max_value_H = 179;
   const int max_value_S = 255;
   const int max_value_V = 255;

   int morph_elem, morph_size, morph_operator, max_operator, max_elem, max_kernel_size;

   int erosion_type, erosion_elem, erosion_size, dilation_type, dilation_elem, dilation_size;
   int erosion_type_grass, erosion_elem_grass, erosion_size_grass, dilation_type_grass, dilation_elem_grass, dilation_size_grass;
   int erosion_type_town, erosion_elem_town, erosion_size_town, dilation_type_town, dilation_elem_town, dilation_size_town;
   int erosion_type_path, erosion_elem_path, erosion_size_path, dilation_type_path, dilation_elem_path, dilation_size_path;
   int erosion_type_penant, erosion_elem_penant, erosion_size_penant, dilation_type_penant, dilation_elem_penant, dilation_size_penant;

   int close_type, close_elem, close_size, open_type, open_elem, open_size;
   int close_type_grass, close_elem_grass, close_size_grass, open_type_grass, open_elem_grass, open_size_grass;
   int close_type_town, close_elem_town, close_size_town, open_type_town, open_elem_town, open_size_town;
   int close_type_path, close_elem_path, close_size_path, open_type_path, open_elem_path, open_size_path;
   int close_type_penant, close_elem_penant, close_size_penant, open_type_penant, open_elem_penant, open_size_penant;

   int high_H, low_H, high_S, low_S, high_V, low_V;
   int high_H_grass, low_H_grass, high_S_grass, low_S_grass, high_V_grass, low_V_grass;
   int high_H_town, low_H_town, high_S_town, low_S_town, high_V_town, low_V_town;
   int high_H_path, low_H_path, high_S_path, low_S_path, high_V_path, low_V_path;
   int high_H_penant, low_H_penant, high_S_penant, low_S_penant, high_V_penant, low_V_penant;

   Mat frame_HSV, frame_BGR, dilation_element, erosion_element, output;
   Mat frame_threshold, frame_threshold_grass, frame_threshold_town, frame_threshold_path, frame_threshold_penant;
   Mat morph_dst, morph_dst_grass, morph_dst_town, morph_dst_path, morph_dst_penant;

   // char* filter = "background";
   char* filter = "foreground";

   if (filter == "background") {
      high_H = max_value_H * 0.4;
      low_H = max_value_H * 0.05;//0.246;
      high_S = max_value_S * 1.00;
      low_S = max_value_S * 0.1;
      high_V = max_value_V * 1.00;
      low_V = max_value_V * 0.50;
   } else if (filter == "foreground") {
      high_H_grass = max_value_H * 0.30;
      low_H_grass = max_value_H * 0.18;
      high_S_grass = max_value_S * 1.00;
      low_S_grass = max_value_S * 0.30;
      high_V_grass = max_value_V * 1.00;
      low_V_grass = max_value_V * 0.40;

      high_H_town = max_value_H * 0.14;
      low_H_town = max_value_H * 0.00;
      high_S_town = max_value_S * 1.00;
      low_S_town = max_value_S * 0.00;
      high_V_town = max_value_V * 1.00;
      low_V_town = max_value_V * 0.10;

      high_H_path = max_value_H * 0.99;
      low_H_path = max_value_H * 0.30;
      high_S_path = max_value_S * 1.00;
      low_S_path = max_value_S * 0.00;
      high_V_path = max_value_V * 1.00;
      low_V_path = max_value_V * 0.40;

      high_H_penant = max_value_H * 0.30;
      low_H_penant = max_value_H * 0.18;
      high_S_penant = max_value_S * 1.00;
      low_S_penant = max_value_S * 0.30;
      high_V_penant = max_value_V * 1.00;
      low_V_penant = max_value_V * 0.40;
   }

   // Morfologia postac elementu
   if (filter == "background") {
      morph_elem = 0;
      morph_size = 20;
      morph_operator = 1;
      max_operator = 4;
      max_elem = 2;
      max_kernel_size = 21;
   } else if (filter == "foreground") {
      morph_elem = 2; // morph elem shape : 1-dots, 2-crosses
      morph_size = 1;
      morph_operator = 0;
      max_operator = 4;
      max_elem = 1;
      max_kernel_size = 21;
   }

   // Dylacja i erozja maski
   if (filter == "background") {
      erosion_elem = 0;
      erosion_size = 2;
      dilation_elem = 0;
      dilation_size = 0;
   } else if (filter == "foreground") {

      close_elem_grass = 2;
      close_size_grass = 2;
      open_elem_grass = 2;
      open_size_grass = 2;
      dilation_elem_grass = 2;
      dilation_size_grass = 6;
      erosion_elem_grass = 2;
      erosion_size_grass = 2;

      close_elem_town = 0;
      close_size_town = 16;
      open_elem_town = 0;
      open_size_town = 4;
      dilation_elem_town = 0;
      dilation_size_town = 6;
      erosion_elem_town = 0;
      erosion_size_town = 4;

      close_elem_path = 2;
      close_size_path = 2;
      open_elem_path = 2;
      open_size_path = 2;
      dilation_elem_path = 2;
      dilation_size_path = 5;
      erosion_elem_path = 2;
      erosion_size_path = 0;

      close_elem_penant = 2;
      close_size_penant = 2;
      open_elem_penant = 2;
      open_size_penant = 2;
      dilation_elem_penant = 2;
      dilation_size_penant = 2;
      erosion_elem_penant = 2;
      erosion_size_penant = 2;
   }

   if(      dilation_elem == 0 ) { dilation_type = MORPH_RECT; }
   else if( dilation_elem == 1 ) { dilation_type = MORPH_CROSS; }
   else if( dilation_elem == 2)  { dilation_type = MORPH_ELLIPSE; }
   if(      erosion_elem == 0 )  { erosion_type = MORPH_RECT; }
   else if( erosion_elem == 1 )  { erosion_type = MORPH_CROSS; }
   else if( erosion_elem == 2)   { erosion_type = MORPH_ELLIPSE; }

   if(      close_elem_grass == 0 ) { close_type_grass = MORPH_RECT; }
   else if( close_elem_grass == 1 ) { close_type_grass = MORPH_CROSS; }
   else if( close_elem_grass == 2)  { close_type_grass = MORPH_ELLIPSE; }
   if(       open_elem_grass == 0 )  { open_type_grass = MORPH_RECT; }
   else if(  open_elem_grass == 1 )  { open_type_grass = MORPH_CROSS; }
   else if(  open_elem_grass == 2)   { open_type_grass = MORPH_ELLIPSE; }
   if(      dilation_elem_grass == 0 ) { dilation_type_grass = MORPH_RECT; }
   else if( dilation_elem_grass == 1 ) { dilation_type_grass = MORPH_CROSS; }
   else if( dilation_elem_grass == 2)  { dilation_type_grass = MORPH_ELLIPSE; }
   if(       erosion_elem_grass == 0 )  { erosion_type_grass = MORPH_RECT; }
   else if(  erosion_elem_grass == 1 )  { erosion_type_grass = MORPH_CROSS; }
   else if(  erosion_elem_grass == 2)   { erosion_type_grass = MORPH_ELLIPSE; }

   if(      close_elem_town == 0 ) { close_type_town = MORPH_RECT; }
   else if( close_elem_town == 1 ) { close_type_town = MORPH_CROSS; }
   else if( close_elem_town == 2)  { close_type_town = MORPH_ELLIPSE; }
   if(       open_elem_town == 0 )  { open_type_town = MORPH_RECT; }
   else if(  open_elem_town == 1 )  { open_type_town = MORPH_CROSS; }
   else if(  open_elem_town == 2)   { open_type_town = MORPH_ELLIPSE; }
   if(      dilation_elem_town == 0 ) { dilation_type_town = MORPH_RECT; }
   else if( dilation_elem_town == 1 ) { dilation_type_town = MORPH_CROSS; }
   else if( dilation_elem_town == 2)  { dilation_type_town = MORPH_ELLIPSE; }
   if(       erosion_elem_town == 0 )  { erosion_type_town = MORPH_RECT; }
   else if(  erosion_elem_town == 1 )  { erosion_type_town = MORPH_CROSS; }
   else if(  erosion_elem_town == 2)   { erosion_type_town = MORPH_ELLIPSE; }

   if(      close_elem_path == 0 ) { close_type_path = MORPH_RECT; }
   else if( close_elem_path == 1 ) { close_type_path = MORPH_CROSS; }
   else if( close_elem_path == 2)  { close_type_path = MORPH_ELLIPSE; }
   if(       open_elem_path == 0 )  { open_type_path = MORPH_RECT; }
   else if(  open_elem_path == 1 )  { open_type_path = MORPH_CROSS; }
   else if(  open_elem_path == 2)   { open_type_path = MORPH_ELLIPSE; }
   if(      dilation_elem_path == 0 ) { dilation_type_path = MORPH_RECT; }
   else if( dilation_elem_path == 1 ) { dilation_type_path = MORPH_CROSS; }
   else if( dilation_elem_path == 2)  { dilation_type_path = MORPH_ELLIPSE; }
   if(       erosion_elem_path == 0 )  { erosion_type_path = MORPH_RECT; }
   else if(  erosion_elem_path == 1 )  { erosion_type_path = MORPH_CROSS; }
   else if(  erosion_elem_path == 2)   { erosion_type_path = MORPH_ELLIPSE; }

   if(      close_elem_penant == 0 ) { close_type_penant = MORPH_RECT; }
   else if( close_elem_penant == 1 ) { close_type_penant = MORPH_CROSS; }
   else if( close_elem_penant == 2)  { close_type_penant = MORPH_ELLIPSE; }
   if(       open_elem_penant == 0 )  { open_type_penant = MORPH_RECT; }
   else if(  open_elem_penant == 1 )  { open_type_penant = MORPH_CROSS; }
   else if(  open_elem_penant == 2)   { open_type_penant = MORPH_ELLIPSE; }
   if(      dilation_elem_penant == 0 ) { dilation_type_penant = MORPH_RECT; }
   else if( dilation_elem_penant == 1 ) { dilation_type_penant = MORPH_CROSS; }
   else if( dilation_elem_penant == 2)  { dilation_type_penant = MORPH_ELLIPSE; }
   if(       erosion_elem_penant == 0 )  { erosion_type_penant = MORPH_RECT; }
   else if(  erosion_elem_penant == 1 )  { erosion_type_penant = MORPH_CROSS; }
   else if(  erosion_elem_penant == 2)   { erosion_type_penant = MORPH_ELLIPSE; }


   // Convert from BGR to HSV colorspace
   cvtColor(img_in, frame_HSV, COLOR_BGR2HSV);
   // Detect the object based on HSV Range Values
   inRange(frame_HSV, Scalar(low_H, low_S, low_V), Scalar(high_H, high_S, high_V), frame_threshold);
   inRange(frame_HSV, Scalar(low_H_grass, low_S_grass, low_V_grass), Scalar(high_H_grass, high_S_grass, high_V_grass), morph_dst_grass);
   inRange(frame_HSV, Scalar(low_H_town, low_S_town, low_V_town), Scalar(high_H_town, high_S_town, high_V_town), morph_dst_town);
   inRange(frame_HSV, Scalar(low_H_path, low_S_path, low_V_path), Scalar(high_H_path, high_S_path, high_V_path), morph_dst_path);
   inRange(frame_HSV, Scalar(low_H_penant, low_S_penant, low_V_penant), Scalar(high_H_penant, high_S_penant, high_V_penant), morph_dst_penant);
   

   if (filter == "background") {
      dilation_element = get_element(dilation_type, dilation_size);
      erosion_element = get_element(erosion_type, erosion_size);
   }
   
   Mat dilation_element_grass =  get_element(dilation_type_grass,  dilation_size_grass );
   Mat dilation_element_town =   get_element(dilation_type_town,   dilation_size_town  );
   Mat dilation_element_path =   get_element(dilation_type_path,   dilation_size_path  );
   Mat dilation_element_penant = get_element(dilation_type_penant, dilation_size_penant);

   Mat erosion_element_grass =  get_element(erosion_type_grass,  erosion_size_grass );
   Mat erosion_element_town =   get_element(erosion_type_town,   erosion_size_town  );
   Mat erosion_element_path =   get_element(erosion_type_path,   erosion_size_path  );
   Mat erosion_element_penant = get_element(erosion_type_penant, erosion_size_penant);

   Mat close_element_grass =  get_element(close_type_grass,  close_size_grass );
   Mat close_element_town =   get_element(close_type_town,   close_size_town  );
   Mat close_element_path =   get_element(close_type_path,   close_size_path  );
   Mat close_element_penant = get_element(close_type_penant, close_size_penant);

   Mat open_element_grass =  get_element(open_type_grass,  open_size_grass );
   Mat open_element_town =   get_element(open_type_town,   open_size_town  );
   Mat open_element_path =   get_element(open_type_path,   open_size_path  );
   Mat open_element_penant = get_element(open_type_penant, open_size_penant);

   
   if (filter == "background") {
      Mat element = get_element(morph_elem,  morph_size );
      // Since MORPH_X : 2,3,4,5 and 6
      int operation = morph_operator + 2;
      morphologyEx( frame_threshold, morph_dst, operation, element );
      dilate( morph_dst, morph_dst, dilation_element );
      bitwise_and(img_in, img_in, output, morph_dst);
   } else if (filter == "foreground") {
      // grass
      morphologyEx(  morph_dst_grass,  morph_dst_grass,  MORPH_CLOSE, close_element_grass );
      morphologyEx(  morph_dst_grass,  morph_dst_grass,  MORPH_OPEN,  open_element_grass  );
      dilate(        morph_dst_grass,  morph_dst_grass,  dilation_element_grass );
      erode(         morph_dst_grass,  morph_dst_grass,  erosion_element_grass  );
      // town
      morphologyEx(  morph_dst_town,   morph_dst_town,   MORPH_OPEN,  open_element_town  );
      morphologyEx(  morph_dst_town,   morph_dst_town,   MORPH_CLOSE, close_element_town );
      erode(         morph_dst_town,   morph_dst_town,   erosion_element_town   );
      dilate(        morph_dst_town,   morph_dst_town,   dilation_element_town  );
      // path
      erode(         morph_dst_path,   morph_dst_path,   erosion_element_path   );
      dilate(        morph_dst_path,   morph_dst_path,   dilation_element_path  );
      morphologyEx(  morph_dst_path,   morph_dst_path,   MORPH_OPEN,  open_element_path  );
      morphologyEx(  morph_dst_path,   morph_dst_path,   MORPH_CLOSE, close_element_path );
      // penant
      morphologyEx(  morph_dst_penant, morph_dst_penant, MORPH_CLOSE, close_element_penant );
      morphologyEx(  morph_dst_penant, morph_dst_penant, MORPH_OPEN,  open_element_penant  );
      dilate(        morph_dst_penant, morph_dst_penant, dilation_element_penant);
      erode(         morph_dst_penant, morph_dst_penant, erosion_element_penant );

      // merge masks
      bitwise_and(img_in, img_in, output, morph_dst_grass);
      bitwise_and(img_in, img_in, output, morph_dst_town);
      bitwise_and(img_in, img_in, output, morph_dst_path);
      bitwise_and(img_in, img_in, output, morph_dst_penant);
   }
      // display("frame_threshold", frame_threshold);       destroyWindow("frame_threshold");
      // display("morph_dst", morph_dst);               destroyWindow("morph_dst");
      // display("morph_dst_grass", morph_dst_grass);   destroyWindow("morph_dst_grass");
      // display("morph_dst_town", morph_dst_town);       destroyWindow("morph_dst_town");
      // display("morph_dst_path", morph_dst_path);     destroyWindow("morph_dst_path");
      // display("morph_dst_penant", morph_dst_penant); destroyWindow("morph_dst_penant");
      // display("output", output);                         destroyWindow("output");
   return output;
}
}


#endif

