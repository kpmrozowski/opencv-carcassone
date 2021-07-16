#ifndef CLASTERS_H
#define CLASTERS_H

#include <Hough/Line.h>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <vector>
// #include "Square.h"
#define _USE_MATH_DEFINES

using PointLines = std::vector<std::vector<cv::Point2d>>;

class LinesClasters {
 public:
  PointLines m_pointLines;
  
  LinesClasters(std::vector<Line> linesvec, int img_rows, int img_cols) {
    twolinesClaster(linesvec, img_rows, img_cols);
  }

  void twolinesClaster(std::vector<Line> linesvec, int img_rows, int img_cols) {
    m_pointLines.resize(4);
    for (int i = 0; i < 4; ++i) {
      double x, y, a, b, c, p, fi;
      switch (i) {
        case 0: {  // left
          a = -1;
          b = 0;
          c = 0;
          p = 0.;
          fi = 0.;
          break;
        }
        case 1: {  // bottom
          a = 0;
          b = -1;
          c = static_cast<double>(img_rows);
          p = static_cast<double>(img_rows);
          fi = -M_PI / 2.;
          break;
        }
        case 2: {  // right
          a = -1;
          b = 0;
          c = static_cast<double>(img_cols);
          p = static_cast<double>(img_cols);
          fi = 0.;
          break;
        }
        case 3: {  // top
          a = 0;
          b = -1;
          c = 0;
          p = 0.;
          fi = -M_PI / 2.;
          break;
        }
      }
      // a1 * x + b1 * y + c1 = 0
      // a2 * x + b2 * y + c2 = 0
      // x = ( b1 * c2 - b2 * c1 ) / ( a1 * b2 - a2 * b1 )
      // y = ( c1 * a2 - c2 * a1 ) / ( a1 * b1 - a2 * b1 )
      for (const Line& line : linesvec) {
        if (a == 0 && line.a() == 0) {        // ===
          x = std::numeric_limits<double>::infinity();
          y = c;
        } else if (b == 0 && line.b() == 0) { //  ||
          x = c;
          y = std::numeric_limits<double>::infinity();
        } else {
          // x = (-c / a + b / a * line.c() / line.b()) /
          //     (1 - b / a * line.a() / line.b());
          // y = -line.c() / line.b() -
          //     (line.a() / line.b()) * (-c / a + b / a * line.c() / line.b())
          //     /
          //         (1 - b / a * line.a() / line.b());
          x = (line.b() * c - b * line.c()) / (line.a() * b - a * line.b());
          y = (line.c() * a - c * line.a()) / (line.a() * b - a * line.b());
        }
        m_pointLines[i].emplace_back(x, y);
      }
    }
  }
};

#endif
