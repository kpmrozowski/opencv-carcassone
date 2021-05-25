#ifndef TWM_COLORS_H
#define TWM_COLORS_H

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

namespace twm::Colors {

std::pair<unsigned char, unsigned char> getMeanHS(const cv::Mat& img) {
    cv::Mat img_hsv;
    cvtColor(img, img_hsv, cv::COLOR_BGR2HSV);
    unsigned int meanH = 0;
    unsigned int meanS = 0;
    for (size_t x = 0; x < img.cols; x++) {
        for (size_t y = 0; y < img.rows; y++) {
            cv::Vec3b pixel = img_hsv.at<cv::Vec3b>(x,y);
            meanH += pixel[0];
            meanS += pixel[1];
        }
    }
    meanH = meanH/(img.rows * img.cols);
    meanS = meanS/(img.rows * img.cols);
    std::cout << "meanH: " << int(meanH) << " meanS: " << int(meanS) << std::endl;
    return std::make_pair(meanH, meanS);
}


} // twm::Colors

#endif // TWM_COLORS_H

