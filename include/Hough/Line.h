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
  std::vector<int> m_coords;
  double m_angle;
  double m_rho;
  double m_a;
  double m_b;
  double m_c;
  cv::Point m_p1, m_p2;

 public:
  Line(){};
  Line(const cv::Vec4i& coords)
      : m_coords(std::vector<int>{coords[0], coords[1], coords[2], coords[3]})
      , m_p1(cv::Point(coords[0], coords[1]))
      , m_p2(cv::Point(coords[2], coords[3])) {
    m_a = static_cast<double>(m_p2.y - m_p1.y);
    m_b = static_cast<double>(m_p1.x - m_p2.x);
    m_c = static_cast<double>(m_p1.y * m_p2.x - m_p1.x * m_p2.y);
    calculateAngle();
    calculateRho();
  }
  Line(const cv::Point _p1, const cv::Point _p2)
      : m_coords(std::vector<int>{_p1.x, _p1.y, _p2.x, _p2.y})
      , m_p1(_p1)
      , m_p2(_p2) {
    m_a = static_cast<double>(m_p2.y - m_p1.y);
    m_b = static_cast<double>(m_p1.x - m_p2.x);
    m_c = static_cast<double>(m_p1.y * m_p2.x - m_p1.x * m_p2.y);
    calculateAngle();
    calculateRho();
  }

  constexpr const double rho() const noexcept { return m_rho; }
  constexpr const double angle() const noexcept { return m_angle; }
  inline const cv::Point p1() const noexcept { return m_p1; }
  inline const cv::Point p2() const noexcept { return m_p2; }

  constexpr bool isVertical(double eps = 10) const noexcept {
    return abs(m_angle - 90) < eps;
  }
  inline void print() const noexcept {
    std::cout << "x1: " << p1().x << " y1: " << p1().y
              << " x2: " << p2().x << " y2: " << p2().y
              << " m_angle: " << angle() << std::endl;
  }

  void draw(cv::Mat img, unsigned char R, unsigned char G, unsigned char B) {
    cv::line(img, p1(), p2(), cv::Scalar(B, G, R), 3, cv::LINE_AA);
  }

  /*!
  **\brief returnes "a" from general line equation a*x + b*y + c = 0
  **\return a form a*x + b*y + c = 0
  */
  inline const double a() const noexcept {
    return m_a;
  }

  /*!
  **\brief returnes "b" from general line equation a*x + b*y + c = 0
  **\return b form a*x + b*y + c = 0
  */
  inline const double b() const noexcept {
    return m_b;
  }

  /*!
  **\brief calculates c from general line equation a*x + b*y + c = 0
  **\return c form a*x + b*y + c = 0
  */
  inline const double c() const noexcept {
    return m_c;
  }

 private:
  inline void calculateAngle() noexcept {
    if (m_coords[2] - m_coords[0] == 0) {
      m_angle = 90;
    } else {
      m_angle = std::atan((m_coords[3] - m_coords[1]) /
                          static_cast<double>(m_coords[2] - m_coords[0])) *
                180 / M_PI;
    }
  }
  void calculateRho() {
    if (p1() == cv::Point(0, 0) || p2() == cv::Point(0, 0)) {
      m_rho = 0.;
      return;
    } else if (p1().x == p2().x) {
      m_rho = p1().x;
      return;
    } else {
      m_rho = abs(c()) / sqrt(pow(a(), 2) + pow(b(), 2));
    }
  }
};

#endif
