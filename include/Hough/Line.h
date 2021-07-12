#ifndef LINE_H
#define LINE_H

#include <algorithm>
#include <cmath>
#include <experimental/filesystem>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <string>
#include <vector>

#include "opencv2/core.hpp"

#define _USE_MATH_DEFINES

class Line {
 public:
  Line(){};
  cv::Vec4i m_coords;
  double m_angle;
  double m_rho;
  cv::Point p1, p2;
  Line(const cv::Vec4i& coords)
      : m_coords(coords),
        p1(cv::Point(coords[0], coords[1])),
        p2(cv::Point(coords[2], coords[3])) {
    calculateAngle();
    calculateRho();
  }
  constexpr bool isVertical(double eps = 10) { return abs(m_angle - 90) < eps; }
  void print() {
    std::cout << "x1: " << m_coords[0] << " y1: " << m_coords[1]
              << " x2: " << m_coords[2] << " y2: " << m_coords[3]
              << " m_angle: " << m_angle << std::endl;
  }
  void draw(cv::Mat img, unsigned char R, unsigned char G, unsigned char B) {
    cv::line(img, p1, p2, cv::Scalar(B, G, R), 3, cv::LINE_AA);
  }

 private:
  void calculateAngle() {
    if (m_coords[2] - m_coords[0] == 0) {
      m_angle = 90;
    } else {
      m_angle = std::atan((m_coords[3] - m_coords[1]) /
                          static_cast<double>(m_coords[2] - m_coords[0])) *
                180 / M_PI;
    }
  }
  void calculateRho() {
    if (p1 == cv::Point(0, 0) || p2 == cv::Point(0, 0)) {
      m_rho = 0.;
      return;
    } else if (p1.x == p2.x) {
      m_rho = p1.x;
      return;
    } else {
      // ax + by + c = 0
      double a = static_cast<double>(p2.y - p1.y);
      double b = static_cast<double>(p1.x - p2.x);
      double c =
          static_cast<double>(p2.y * (p2.x - p1.x) + p2.x * (p2.y - p1.y));
      m_rho = abs(c) / sqrt(pow(a, 2) + pow(b, 2));
    }
  }
};

#endif
