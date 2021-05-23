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
    cv::Vec4i m_coords;
    double m_angle;
    Line(const cv::Vec4i& coords)
      : m_coords(coords) 
    {
        calculateAngle(coords);
    }
    constexpr bool isVertical(double eps = 10) {
        return abs(m_angle - 90) < eps;
    }
    void print() {
        std::cout << "x1: " << m_coords[0] << " y1: " << m_coords[1]
            << " x2: " << m_coords[2] << " y2: " << m_coords[3] << " m_angle: " << m_angle << std::endl;
    }
    void draw(cv::Mat img, unsigned char R, unsigned char G, unsigned char B) {
        cv::line(img, cv::Point(m_coords[0], m_coords[1]), cv::Point(m_coords[2], m_coords[3]),
            cv::Scalar(B,G,R), 3, cv::LINE_AA);
    }
private:
    void calculateAngle(const cv::Vec4i& m_coords) {
        if (m_coords[2] - m_coords[0] == 0) {
            m_angle = 90;
        } else {
            m_angle = std::atan((m_coords[3] - m_coords[1])/static_cast<double>(m_coords[2] - m_coords[0])) * 180/M_PI;
        }
    }
};

#endif

