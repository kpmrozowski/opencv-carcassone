#ifndef POINT_H
#define POINT_H

template <typename T = int>
struct Point {
  T x, y;
  double ro, fi;
  Point() : x(-1.), y(-1.) {}
  Point(T _x, T _y) : x(_x), y(_y) {
    ro = pow(pow(_x, 2) + pow(_y, 2), 0.5);
    fi = atan2(y, x);
  }
  Point(T _x, T _y, T _ro, T _fi) : x(_x), y(_y), ro(_ro), fi(_fi) {}
  inline bool operator==(const Point &p) {
    return p.x == this->x && p.y == this->y;
  }
};

#endif