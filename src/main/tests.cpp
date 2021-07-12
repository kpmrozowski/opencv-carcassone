#include <Carcassonne/Game/Board.h>
#include <Clasters/Clasters.h>
#include <Colors/ClassifyHOG.h>
#include <Colors/Colors.h>
#include <Hough/Hough.h>
#include <Hough/Square.h>
#include <Utils/display.h>
#include <Utils/rgb2hsv.h>
#include <fmt/core.h>

#include <iostream>
#include <limits>

using namespace twm;

int main() {
  auto p1{Point(0., 1.0000000000000)};
  auto p2{Point(0., 1.)};
  if (p1 == p2)
    fmt::print("Equal\n");
  else
    fmt::print("Unequal\n");
  LinesClasters linesClasters;
}