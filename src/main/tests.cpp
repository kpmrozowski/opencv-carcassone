#include <Carcassonne/Game/Board.h>
#include <Clasters/Clasters.h>
#include <Colors/ClassifyHOG.h>
#include <Colors/Colors.h>
#include <Hough/Hough.h>
#include <Hough/Square.h>
#include <Utils/display.h>
#include <Utils/rgb2hsv.h>
#include <fmt/core.h>
#include <Utils/matplotlibcpp.h>

#include <iostream>
#include <limits>
// #include <cmath>

namespace plt = matplotlibcpp;

int main()
{
    const int n = 20;
    std::vector<std::vector<double>> matrix;

    for (int i = 0; i < n; ++i) {
        std::vector<double> row;
        for (int j = 0; j < n; ++j) {
            if (i == j)
                row.push_back(-2);
            else if (j == i - 1 || j == i + 1)
                row.push_back(1);
            else
                row.push_back(0);
        }
        matrix.push_back(row);
    }

    plt::spy(matrix, 0.5, {{"marker", "o"}});
    plt::show();

    return 0;
}

// using namespace twm;
// int main() {
//   auto p1{Point(0., 1.0000000000000)};
//   auto p2{Point(0., 1.)};
//   if (p1 == p2)
//     fmt::print("Equal\n");
//   else
//     fmt::print("Unequal\n");
//   // LinesClasters linesClasters;
// }