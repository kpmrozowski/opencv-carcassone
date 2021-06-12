#ifndef TWM_CLASSIFYHOG_H
#define TWM_CLASSIFYHOG_H
#include <opencv2/features2d.hpp>

namespace twm::colors {

class Classifier {
public:
   std::tuple<cv::Mat, cv::Mat> hog (cv::Mat img) {
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

};





} // twm::colors

#endif // TWM_CLASSIFYHOG_H
