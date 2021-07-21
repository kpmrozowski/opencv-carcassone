#ifndef CLASTERS_H
#define CLASTERS_H

#include <Hough/Line.h>
#include <Hough/Point.h>
#include <Utils/matplotlibcpp.h>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <vector>
#include <numeric>
#include <fmt/core.h>
// #include "Square.h"
#define _USE_MATH_DEFINES
using PointLines = std::vector<std::pair<Point<double>, Point<double>>>;

class LinesClasters {
 public:
  PointLines m_pointLines;
  
  LinesClasters(std::vector<Line> linesvec, int img_rows, int img_cols) {
    twolinesClaster(linesvec, img_rows, img_cols);
  }

  void print() {
      fmt::print("fi1\t fi2\n");
    for (const auto& pl : m_pointLines) {
      fmt::print("{}\t {}\n", pl.first.fi, pl.second.fi);
    }
  }

  template <typename T, typename Compare>
  std::vector<std::size_t> find_permutation(const std::vector<T>& vec, const Compare& compare) {
    std::vector<std::size_t> p(vec.size());
    std::iota(p.begin(), p.end(), 0);
    std::sort(p.begin(), p.end(), [&](std::size_t i, std::size_t j) {
      return compare(vec[i], vec[j]);
    });
    return p;
  }

  template <typename T>
  std::vector<T> apply_permutation(const std::vector<T>& vec, const std::vector<std::size_t>& p) {
    std::vector<T> sorted_vec(vec.size());
    std::transform(p.begin(), p.end(), sorted_vec.begin(),
                   [&](std::size_t i) { return vec[i]; });
    return sorted_vec;
  }

  template <typename T>
  void apply_permutation_in_place(std::vector<T>& vec, const std::vector<std::size_t>& p) {
    std::vector<bool> done(vec.size());
    for (std::size_t i = 0; i < vec.size(); ++i) {
      if (done[i]) {
        continue;
      }
      done[i] = true;
      std::size_t prev_j = i;
      std::size_t j = p[i];
      while (i != j) {
        std::swap(vec[prev_j], vec[j]);
        done[j] = true;
        prev_j = j;
        j = p[j];
      }
    }
  }

  void twolinesClaster(std::vector<Line> linesvec, int img_rows, int img_cols) {
    auto size = linesvec.size();
    // circle coeffictients
    const double x_o = img_rows / 2.;
    const double y_o = img_cols / 2.;
    const double r_o = sqrt(pow(x_o, 2) + pow(y_o, 2));
    
    for (auto it = linesvec.begin(); it != linesvec.end(); ++it) {
      const auto i = it - linesvec.begin();
      
      // general equation of a line coefficients in coordinate system corresponding to the center of a circle
      const double a_0 = it->a();
      const double b_0 = it->b();
      const double c_0 = it->c();
      
      const double a = a_0;
      const double b = b_0;
      const double c = c_0 + a * x_o + b * y_o;
      
      // distance from line to center of circle
      const double d_0 = abs(c) / sqrt(pow(a, 2) + pow(b, 2));
      //

      // coordinate x of the point belonging to the line closest to the center of the circle
      const double x_0 = - a * c / (pow(a, 2) + pow(b, 2));
      // coordinate y of the point belonging to the line closest to the center of the circle
      const double y_0 = - b * c / (pow(a, 2) + pow(b, 2));
      
      // distance from (x_0, y_0) to line-circle intersection
      const double d = sqrt(pow(r_o, 2) - pow(c, 2) / (pow(a, 2) + pow(b, 2)));

      // constant multiplier
      const double mult = sqrt(pow(d, 2) / (pow(a, 2) + pow(b, 2)));

      // coordinates of the first intersection point
      const double ax = x_0 + b * mult + x_o,  ay = y_0 - a * mult + y_o;
      const double aro = sqrt(pow(ax - x_o, 2) + pow(ay - y_o, 2));
      const double afi = atan2(ay - y_o, ax - x_o);
      // coordinates of the second intersection point
      const double bx = x_0 - b * mult + x_o,  by = y_0 + a * mult + y_o;
      const double bro = sqrt(pow(bx - x_o, 2) + pow(by - y_o, 2));
      const double bfi = atan2(by - y_o, bx - x_o);

      if (afi < - M_PI)
        fmt::print("afi < -M_PI\n");
      if (afi > M_PI)
        fmt::print("afi >  M_PI\n");
      if (bfi < - M_PI)
        fmt::print("afi < -M_PI\n");
      if (bfi > M_PI)
        fmt::print("afi >  M_PI\n");
      
      if (d_0 >= r_o) {
        fmt::print("{}: r_o <= d = {}\n", i, d_0);
        continue;
      }
      m_pointLines.emplace_back(Point(ax, ay, aro, afi), Point(bx, by, bro, bfi));
    }
    std::vector<std::vector<double>> distances_table{size, std::vector<double>(size, -1.)};
    for (int i = 0; i < size; ++i) {
      for (int j = 0; j < size; ++j) {
        distances_table.at(i).at(j) = sqrt(
            pow(m_pointLines.at(i).first.fi - m_pointLines.at(j).first.fi, 2) +
            pow(m_pointLines.at(i).second.fi - m_pointLines.at(j).second.fi,
                2));
      }
    }
    auto sorted_distances_table{distances_table};
    std::vector<std::vector<long unsigned int>> permutations_table{size, std::vector<long unsigned int>(size, -1)};
    std::vector<double> means(size, -1.);
    std::vector<double> medians(size, -1.);
    for (int i = 0; i < size; ++i) {// std::vector<double>& distances : sorted_distances_table) {
      auto distances{sorted_distances_table.at(i)};
      auto p = find_permutation(
          distances,
          [](double a, double b) {
            return a < b;
          });
      permutations_table.at(i) = p;
      means.at(i) = std::accumulate(distances.begin(), distances.end(), 0.) / static_cast<double>(size);
      medians.at(i) = distances.at(size / 2);
      apply_permutation_in_place(sorted_distances_table.at(i), p);
    }
    auto p_means = find_permutation(
          means,
          [](double a, double b) {
            return a < b;
          });
    auto p_medians = find_permutation(
          medians,
          [](double a, double b) {
            return a < b;
          });
    apply_permutation_in_place(means, p_means);
    apply_permutation_in_place(medians, p_medians);

    // vectorA = apply_permutation(vectorA, p);
    // vectorB = apply_permutation(vectorB, p);
    std::vector<double> x(size, -1), y(size, -1), x1(size, -1), y1(size, -1), x2(size, -1), y2(size, -1);
    for (int i = 0; i < size; ++i) {
      x1.at(i) = m_pointLines.at(i).first.x;
      y1.at(i) = m_pointLines.at(i).first.y;
      x2.at(i) = m_pointLines.at(i).second.x;
      y2.at(i) = m_pointLines.at(i).second.y;
      x.at(i) = m_pointLines.at(i).first.fi / M_PI * 180;
      y.at(i) = m_pointLines.at(i).second.fi / M_PI * 180;
    }
    // matplotlibcpp::plot(x1, y1, "r.", x2, y2, "b.");
    // matplotlibcpp::show();
    matplotlibcpp::plot(x, y, "r.");
    matplotlibcpp::plot(y, x, "b.");
    matplotlibcpp::show();

    this->print();
    double zz = 1.;
  }
};

#endif
