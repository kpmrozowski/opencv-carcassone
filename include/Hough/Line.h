#ifndef LINE_H
#define LINE_H

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <vector>
#include <string>
#include <experimental/filesystem>
#include <algorithm>
#include <cmath>

#define _USE_MATH_DEFINES

class Line
{
public:
    cv::Vec4i coords;
    double angle;
    Line(const cv::Vec4i& coords)
      : coords(coords) 
    {
        calculateAngle(coords);
    }
    void print() {
        std::cout << "x1: " << coords[0] << " y1: " << coords[1]
            << " x2: " << coords[2] << " y2: " << coords[3] << " angle: " << angle << std::endl;
    }
    void draw(cv::Mat img, unsigned char R, unsigned char G, unsigned char B) {
        cv::line(img, cv::Point(coords[0], coords[1]), cv::Point(coords[2], coords[3]),
            cv::Scalar(B,G,R), 3, cv::LINE_AA);
    }
private:
    void calculateAngle(const cv::Vec4i& coords) {
        if (coords[2] - coords[0] == 0) {
            angle = 90;
        } else {
            angle = std::atan((coords[3] - coords[1])/(coords[2] - coords[0])) * 180/M_PI;
        }
    }
};

#endif

