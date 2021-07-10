#ifndef CLASTERS_H
#define CLASTERS_H

// #include <opencv2/imgcodecs.hpp>
// #include <opencv2/highgui.hpp>
// #include <opencv2/imgproc.hpp>
// #include "opencv2/core.hpp"
#include "Line.h"
#include "Square.h"
#include <iostream>
#include <vector>
// #include <string>
// #include <experimental/filesystem>
#include <algorithm>
#include <cmath>
#define _USE_MATH_DEFINES


class LinesClasters {
using PointLines = std::vector<std::vector<Point>>; 

public:

void twolinesClaster(std::vector<Line> linesvec) {
    PointLines pointLines(4);
    for (int i = 0; i < 4; ++i) {
        double p, fi;
        switch(i)
        case(0) {
            p = 0.;
            fi = 0.;
        }
        case(1) {
            p = static<double>(img_rows);
            fi = -M_PI/2.;
        }
        case(2) {
            p = static_cast<double>(img_cols);
            fi = 0.;
        }
        case(3) {
            p = 0.
            fi = -M_PI/2.;
        }
        for (const Line& line : linesvec) {
            ;
        }
    }
}
private:
};

#endif

