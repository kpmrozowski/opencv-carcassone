#ifndef SQUARE_H
#define SQUARE_H

#include "Lines.h"

class Square {
public:
    std::pair<double,double> NW;
    std::pair<double,double> NE;
    std::pair<double,double> SW;
    std::pair<double,double> SE;
    Square(Line lineLeft, Line lineRight, Line lineTop, Line lineBottom) {

    }
};

#endif