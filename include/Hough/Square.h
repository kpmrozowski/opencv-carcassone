#ifndef SQUARE_H
#define SQUARE_H

#include "Lines.h"

struct Point {
    double x, y;
    Point() : x(0.), y(0.) {}
    Point(double _x, double _y) : x(_x), y(_y) {}
};

struct Square {
    Point NW;
    Point NE;
    Point SW;
    Point SE;
    Square()
      : NW(Point())
      , NE(Point())
      , SW(Point())
      , SE(Point()) {}
    Square(Point _NW, Point _NE, Point _SW, Point _SE)
      : NW(_NW)
      , NE(_NE)
      , SW(_SW)
      , SE(_SE) {}
    
    void print() {
        std::cout << "NW(" << NW.x << ", " << NW.y << "), NE(" << NE.x << ", " << NE.y << "), SW(" << SW.x << ", " << SW.y << "), SE(" << SE.x << ", " << SE.y << ")" << std::endl;
    }
    void draw(cv::Mat img, unsigned char R, unsigned char G, unsigned char B) {
        cv::line(img, cv::Point(NW.x, NW.y), cv::Point(NE.x, NE.y),
            cv::Scalar(B,G,R), 3, cv::LINE_AA);
        cv::line(img, cv::Point(NE.x, NE.y), cv::Point(SE.x, SE.y),
            cv::Scalar(B,G,R), 3, cv::LINE_AA);
        cv::line(img, cv::Point(SE.x, SE.y), cv::Point(SW.x, SW.y),
            cv::Scalar(B,G,R), 3, cv::LINE_AA);
        cv::line(img, cv::Point(SW.x, SW.y), cv::Point(NW.x, NW.y),
            cv::Scalar(B,G,R), 3, cv::LINE_AA);
    }
};

#endif