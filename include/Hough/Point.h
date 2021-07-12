#ifndef POINT_H
#define POINT_H

template <typename T = int>
struct Point {
  T x, y;
  Point() : x(0.), y(0.) {}
  Point(T _x, T _y) : x(_x), y(_y) {}
  inline bool operator==(const Point &p) {
    return p.x == this->x && p.y == this->y;
  }
};

#endif