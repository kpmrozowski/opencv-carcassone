#ifndef TWM_CLASSIFYHOG_H
#define TWM_CLASSIFYHOG_H
#include <opencv2/features2d.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect.hpp>

namespace twm::colors {

class Classifier {
public:
   std::tuple<cv::Mat, cv::Mat> hog_sobel (cv::Mat img) {
      // C++ gradient calculation.
      // Read image
      img.convertTo(img, CV_32F, 1/255.0);

      // Calculate gradients gx, gy
      cv::Mat gx, gy;
      cv::Sobel(img, gx, CV_32F, 1, 0, 1);
      cv::Sobel(img, gy, CV_32F, 0, 1, 1);

      // C++ Calculate gradient magnitude and direction (in degrees)
      cv::Mat mag, angle;
      cv::cartToPolar(gx, gy, mag, angle, 1);
      auto mag_real = mag;
      auto angle_real = angle;
      return std::make_tuple(gx, gy);
   }

   std::vector<float> hog (cv::Mat img) {
       //params
    cv::Size cellSize(8,8);
    int nbins= 9;
    cv::Size blockSize(2,2);

    //my variables
    std::vector<float>hog_vector;
    cv::Mat templ_gray;

    //convert to gray
    cv::cvtColor(img, templ_gray, cv::COLOR_BGR2GRAY);

    //create hog object
    cv::HOGDescriptor hog(cv::Size(templ_gray.cols/cellSize.width*cellSize.width,templ_gray.rows/cellSize.height*cellSize.height),
            cv::Size(blockSize.height*cellSize.height,blockSize.width*cellSize.width),
            cv::Size(cellSize.height,cellSize.width),
            cellSize,
            nbins);
      // gives --> winSize [32 x 16],  blockSize [16 x 16],  blockStride [8 x 8],  cellSize [8 x 8]

      //compute the descriptor of the car
      hog.compute(templ_gray, hog_vector, cv::Size(cellSize.height,cellSize.width), cv::Size( 0, 0 ));

      return hog_vector;
   }
};





} // twm::colors

#endif // TWM_CLASSIFYHOG_H
