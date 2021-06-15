#ifndef UTILS_DISPLAY_H
#define UTILS_DISPLAY_H

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>



cv::Mat copyOneImage(const cv::Mat &origImage) {
    cv::Mat image;
    origImage.copyTo(image);
    return image;
}
void display(std::string wndName, const cv::Mat& img, bool wait = true) {
    int frame_width = img.cols;
	int frame_height = img.rows;
    // float scale = 1080. / frame_height;
    float scale = 2000. / frame_height;
    cv::Mat img_copy = copyOneImage(img);
    cv::resize(img_copy, img_copy, cv::Size(static_cast<std::size_t>(frame_width * scale), static_cast<std::size_t>(frame_height * scale)));
    imshow(wndName, img_copy);
    if (wait)
        cv::waitKey();
}



#endif