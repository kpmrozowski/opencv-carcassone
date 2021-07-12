#ifndef SQUARE_H
#define SQUARE_H

#include "Lines.h"

struct Square {
  Point<> NW;
  Point<> NE;
  Point<> SW;
  Point<> SE;
  Line N, S, W, E;
  Square() : NW(Point()), NE(Point()), SW(Point()), SE(Point()) {}
  Square(Point<> _NW, Point<> _NE, Point<> _SW, Point<> _SE)
      : NW(_NW), NE(_NE), SW(_SW), SE(_SE) {}
  Square(Point<> _NW, Point<> _NE, Point<> _SW, Point<> _SE, Line _N, Line _E,
         Line _S, Line _W)
      : NW(_NW), NE(_NE), SW(_SW), SE(_SE), N(_N), E(_E), S(_S), W(_W) {}
  Square(std::vector<cv::Point> _square)
      : NW(Point(_square[0].x, _square[0].y)),
        NE(Point(_square[3].x, _square[3].y)),
        SW(Point(_square[1].x, _square[1].y)),
        SE(Point(_square[2].x, _square[2].y)) {}

  void print() {
    std::cout << "NW(" << NW.x << ", " << NW.y << "), NE(" << NE.x << ", "
              << NE.y << "), SW(" << SW.x << ", " << SW.y << "), SE(" << SE.x
              << ", " << SE.y << ")" << std::endl;
  }
  void draw(cv::Mat img, unsigned char R, unsigned char G, unsigned char B) {
    cv::line(img, cv::Point(NW.x, NW.y), cv::Point(NE.x, NE.y),
             cv::Scalar(B, G, R), 3, cv::LINE_AA);
    cv::line(img, cv::Point(NE.x, NE.y), cv::Point(SE.x, SE.y),
             cv::Scalar(B, G, R), 3, cv::LINE_AA);
    cv::line(img, cv::Point(SE.x, SE.y), cv::Point(SW.x, SW.y),
             cv::Scalar(B, G, R), 3, cv::LINE_AA);
    cv::line(img, cv::Point(SW.x, SW.y), cv::Point(NW.x, NW.y),
             cv::Scalar(B, G, R), 3, cv::LINE_AA);
  }
};

#endif