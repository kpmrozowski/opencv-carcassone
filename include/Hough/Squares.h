#ifndef SQUARE_H
#define SQUARE_H

#include "Lines.h"

struct Point {
    double x, y;
}

struct Square {
    Point NW;
    Point NE;
    Point SW;
    Point SE;
    Square(Point _NW, Point _NE, Point _SW, Point _SE)
      : Point NW(_NW)
      , Point NE(_NE)
      , Point SW(_SW)
      , Point SE(_SE) {}
};

#endif