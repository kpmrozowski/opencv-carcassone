#ifndef CLASTERS_H
#define CLASTERS_H

#include <Hough/Line.h>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
// #include "Square.h"
#define _USE_MATH_DEFINES

using PointLines = std::vector<std::vector<cv::Point>>;

class LinesClasters {
 private:
 public:
  void twolinesClaster(std::vector<Line> linesvec, int img_rows, int img_cols) {
    PointLines pointLines(4);
    for (int i = 0; i < 4; ++i) {
      double p, fi;
      switch (i) {
        case 0: {  // left
          p = 0.;
          fi = 0.;
          break;
        }
        case 1: {  // bottom
          p = static_cast<double>(img_rows);
          fi = -M_PI / 2.;
          break;
        }
        case 2: {  // right
          p = static_cast<double>(img_cols);
          fi = 0.;
          break;
        }
        case 3: {  // top
          p = 0.;
          fi = -M_PI / 2.;
          break;
        }
      }
      for (const Line& line : linesvec) {
        ;
      }
    }
  }
};

#endif
