#ifndef CLASTERS_H
#define CLASTERS_H

#include <Hough/Line.h>
#include <Hough/Point.h>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <vector>
#include <fmt/core.h>
// #include "Square.h"
#define _USE_MATH_DEFINES
using PointLines = std::vector<std::tuple<Line, Point<double>, Point<double>>>;

class LinesClasters {
 public:
  PointLines m_pointLines;
  
  LinesClasters(std::vector<Line> linesvec, int img_rows, int img_cols) {
    twolinesClaster(linesvec, img_rows, img_cols);
  }

  void print() {
      fmt::print("fi1\t fi2\n");
    for (const auto& pl : m_pointLines) {
      fmt::print("{}\t {}\n", std::get<1>(pl).fi, std::get<2>(pl).fi);
    }
  }

  void twolinesClaster(std::vector<Line> linesvec, int img_rows, int img_cols) {
    // circle coeffictients
    const double x_o = img_rows / 2.;
    const double y_o = img_cols / 2.;
    const double r_o = sqrt(pow(x_o, 2) + pow(y_o, 2));
    
    for (auto it = linesvec.begin(); it != linesvec.end(); ++it) {
      const auto i = it - linesvec.begin();
      
      // general equation of a line coefficients in coordinate system corresponding to the center of a circle
      const double a_0 = it->a();
      const double b_0 = it->b();
      const double c_0 = it->c();
      
      const double a = a_0;
      const double b = b_0;
      const double c = c_0 + a * x_o + b * y_o;
      
      // distance from line to center of circle
      const double d_0 = abs(c) / sqrt(pow(a, 2) + pow(b, 2));
      //

      // coordinate x of the point belonging to the line closest to the center of the circle
      const double x_0 = - a * c / (pow(a, 2) + pow(b, 2));
      // coordinate y of the point belonging to the line closest to the center of the circle
      const double y_0 = - b * c / (pow(a, 2) + pow(b, 2));
      
      // distance from (x_0, y_0) to line-circle intersection
      const double d = sqrt(pow(r_o, 2) - pow(c, 2) / (pow(a, 2) + pow(b, 2)));

      // constant multiplier
      const double mult = sqrt(pow(d, 2) / (pow(a, 2) + pow(b, 2)));

      // coordinates of the first intersection point
      const double ax = x_0 + b * mult + x_o,  ay = y_0 - a * mult + y_o;
      const double aro = sqrt(pow(ax - x_o, 2) + pow(ay - y_o, 2));
      const double afi = atan2(ay - y_o, ax - x_o);
      // coordinates of the second intersection point
      const double bx = x_0 - b * mult + x_o,  by = y_0 + a * mult + y_o;
      const double bro = sqrt(pow(bx - x_o, 2) + pow(by - y_o, 2));
      const double bfi = atan2(by - y_o, bx - x_o);

      if (afi < - M_PI)
        fmt::print("afi < -M_PI\n");
      if (afi > M_PI)
        fmt::print("afi >  M_PI\n");
      if (bfi < - M_PI)
        fmt::print("afi < -M_PI\n");
      if (bfi > M_PI)
        fmt::print("afi >  M_PI\n");
      
      if (d_0 >= r_o) {
        fmt::print("{}: r_o <= d = {}\n", i, d_0);
        continue;
      }
      m_pointLines.emplace_back(*it, Point(ax, ay, aro, afi), Point(bx, by, bro, bfi));
    }
    this->print();
    double zz = 1.;
  }
};

#endif
