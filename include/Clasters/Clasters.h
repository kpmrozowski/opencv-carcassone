#ifndef CLASTERS_H
#define CLASTERS_H

#include <Hough/Line.h>
#include <Hough/Point.h>
#include <Utils/matplotlibcpp.h>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <limits>
#include <vector>
#include <numeric>
#include <fmt/core.h>
#include <HDBSCAN_CPP/Hdbscan/hdbscan.hpp>
// #include "Square.h"
#define _USE_MATH_DEFINES
using PointLines = std::vector<std::pair<Point<double>, Point<double>>>;

class LinesClasters {
 public:
  PointLines m_pointLines;
  size_t m_size;
  const double m_x_o;
  const double m_y_o;
  const double m_r_o;
  int m_k = 6;
  std::vector<std::vector<double>> m_distances_table, m_sorted_distances_table, m_mutual_reachability_distances_table;
  std::vector<double> m_core_distances;
  std::vector<Line> m_linesvec;
  Hdbscan m_hdbscan;
  std::vector<std::vector<double>> m_dataset;
  std::vector<Line> m_result;
  
  LinesClasters(std::vector<Line> linesvec, int img_rows, int img_cols)
   : m_size(linesvec.size())
   , m_x_o(img_rows / 2.)
   , m_y_o(img_cols / 2.)
   , m_r_o(sqrt(pow(img_rows / 2., 2) + pow(img_cols / 2., 2)))
   , m_distances_table(std::vector<std::vector<double>>(m_size, std::vector<double>(m_size, -1.)))
   , m_core_distances(m_size, -1.)
   , m_mutual_reachability_distances_table(std::vector<std::vector<double>>(m_size, std::vector<double>(m_size, -1.))) {
    twolinesClaster(linesvec, img_rows, img_cols);
    display_dataset();
    get_statistics();
    calculate_core_distances();
    calculate_mutual_reachability_distance();
    apply_HDBSCAN();
    get_lines_back_from_clasters();
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

  /** \brief The method creates dataset to be clustered in 3 steps:
   * 1. Creates a circle on the picture vertices. 
   * 2. Finds corossection points of lines from 'linesvec' with the circle. 
   * 3. Represents each point by an angle. Each line creates two points so is represented by two angles. These pairs of angles are cooridinates of new points that are going to by clustered via 'apply_HDBSCAN' method. */
  void twolinesClaster(std::vector<Line> linesvec, int img_rows, int img_cols) {
    /*###################################################
    #         Lines to angle pairs conversion           #
    ###################################################*/
    for (auto it = linesvec.begin(); it != linesvec.end(); ++it) {
      const auto i = it - linesvec.begin();
      
      // general equation of a line coefficients in coordinate system corresponding to the center of a circle
      const double a_0 = it->a();
      const double b_0 = it->b();
      const double c_0 = it->c();
      
      const double a = a_0;
      const double b = b_0;
      const double c = c_0 + a * m_x_o + b * m_y_o;
      
      // distance from line to center of circle
      const double d_0 = abs(c) / sqrt(pow(a, 2) + pow(b, 2));
      // coordinate x of the point belonging to the line closest to the center of the circle
      const double x_0 = - a * c / (pow(a, 2) + pow(b, 2));
      // coordinate y of the point belonging to the line closest to the center of the circle
      const double y_0 = - b * c / (pow(a, 2) + pow(b, 2));
      
      // distance from (x_0, y_0) to line-circle intersection
      const double d = sqrt(pow(m_r_o, 2) - pow(c, 2) / (pow(a, 2) + pow(b, 2)));

      // constant multiplier
      const double mult = sqrt(pow(d, 2) / (pow(a, 2) + pow(b, 2)));

      // coordinates of the first intersection point
      const double ax = x_0 + b * mult + m_x_o,  ay = y_0 - a * mult + m_y_o;
      const double aro = sqrt(pow(ax - m_x_o, 2) + pow(ay - m_y_o, 2));
      const double afi = atan2(ay - m_y_o, ax - m_x_o);
      // coordinates of the second intersection point
      const double bx = x_0 - b * mult + m_x_o,  by = y_0 + a * mult + m_y_o;
      const double bro = sqrt(pow(bx - m_x_o, 2) + pow(by - m_y_o, 2));
      const double bfi = atan2(by - m_y_o, bx - m_x_o);

      if (afi < - M_PI)
        fmt::print("afi < -M_PI\n");
      if (afi > M_PI)
        fmt::print("afi >  M_PI\n");
      if (bfi < - M_PI)
        fmt::print("afi < -M_PI\n");
      if (bfi > M_PI)
        fmt::print("afi >  M_PI\n");
      
      if (d_0 >= m_r_o) {
        fmt::print("{}: m_r_o <= d = {}\n", i, d_0);
        continue;
      }
      m_pointLines.emplace_back(Point(ax, ay, aro, afi), Point(bx, by, bro, bfi));
    }
  }

  void display_dataset() {
    /*###################################################
    #                 The gained dataset                #
    ###################################################*/
    std::vector<double> x(m_size, -1), y(m_size, -1), x1(m_size, -1), y1(m_size, -1), x2(m_size, -1), y2(m_size, -1);
    for (int i = 0; i < m_size; ++i) {
      x1.at(i) = m_pointLines.at(i).first.x;
      y1.at(i) = m_pointLines.at(i).first.y;
      x2.at(i) = m_pointLines.at(i).second.x;
      y2.at(i) = m_pointLines.at(i).second.y;
      x.at(i) = m_pointLines.at(i).first.fi / M_PI * 180;
      y.at(i) = m_pointLines.at(i).second.fi / M_PI * 180;
    }
    matplotlibcpp::plot(x1, y1, "r.", x2, y2, "b.");
    matplotlibcpp::show();
    matplotlibcpp::plot(x, y, "r.");
    matplotlibcpp::plot(y, x, "b.");
    matplotlibcpp::show();
    this->print();
    double zz = 1.;
  }

  /** @brief sorted distances, means, medians */
  void get_statistics() {
    for (int i = 0; i < m_size; ++i) {
      for (int j = 0; j < m_size; ++j) {
        m_distances_table.at(i).at(j) = sqrt(
            pow(m_pointLines.at(i).first.fi - m_pointLines.at(j).first.fi, 2) +
            pow(m_pointLines.at(i).second.fi - m_pointLines.at(j).second.fi,
                2));
      }
    }
    m_sorted_distances_table = m_distances_table;
    std::vector<std::vector<long unsigned int>> permutations_table{m_size, std::vector<long unsigned int>(m_size, -1)};
    std::vector<double> means(m_size, -1.);
    std::vector<double> medians(m_size, -1.);
    std::vector<double> mutual_reachability_distance(m_size, -1.);
    for (int i = 0; i < m_size; ++i) {// std::vector<double>& distances : m_sorted_distances_table) {
      auto distances{m_sorted_distances_table.at(i)};
      auto p = find_permutation(
          distances,
          [](double a, double b) {
            return a < b;
          });
      permutations_table.at(i) = p;
      means.at(i) = std::accumulate(distances.begin(), distances.end(), 0.) / static_cast<double>(m_size);
      medians.at(i) = distances.at(m_size / 2);
      apply_permutation_in_place(m_sorted_distances_table.at(i), p);
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
  }
  
  /** @brief calculate core distances */
  void calculate_core_distances() {
    std::transform(m_sorted_distances_table.begin(), m_sorted_distances_table.end(), m_core_distances.begin(), [this](const std::vector<double>& distances) {
      return distances.at(this->m_k);
    });
    printf("\n");
  }

  /** @brief calculate mutual reachability distances */
  void calculate_mutual_reachability_distance() {
    for (int i = 0; i < m_size; ++i)
      for (int j = 0; j < m_size; ++j) {
        auto a = m_distances_table.at(i).at(j);
        auto b = m_core_distances.at(i);
        auto c = m_core_distances.at(j);
        m_mutual_reachability_distances_table.at(i).at(j) = std::max(std::max(a, b), c);
      }
    printf("\n");
  }

  /** @brief Hierarchical Density-Based Spatial Clustering of Applications with Noise */
  void apply_HDBSCAN() {
    for (int i = 0; i < m_size; ++i) {
      std::vector<double> point(2);
      point.at(0) = m_pointLines.at(i).first.fi / M_PI * 180;
      point.at(1) = m_pointLines.at(i).second.fi / M_PI * 180;
      m_dataset.push_back(point);
    }
    m_hdbscan.dataset = m_dataset;
    std::string execution_type{"Euclidean"};
    m_hdbscan.execute(m_k, m_k, execution_type);

    m_hdbscan.displayResult();
    // std::endl << "You can access other fields like cluster labels, membership probabilities and outlier scores."<<endl;
    // Use it like this
    m_hdbscan.labels_;
    m_hdbscan.membershipProbabilities_;
    m_hdbscan.outlierScores_;
    printf("\n");
  }

  void get_lines_back_from_clasters() {
    // 1. claculate mean of each claster (receive two angles from each)
    
    int clasters_count = *(std::max_element(m_hdbscan.normalizedLabels_.begin(), m_hdbscan.normalizedLabels_.end()));
    std::vector<std::vector<std::vector<double>>> clasters_vect(clasters_count);
    std::vector<std::vector<double>> clasters_sums(clasters_count, std::vector<double>(2, 0));
    std::vector<int> clasters_sizes(clasters_count, 0);
    for (int i = 0; i < m_size; ++i) {
      int idx = m_hdbscan.normalizedLabels_.at(i) - 1;
      if (idx == -2 || idx == -1) {
        continue;
      } else if (idx < clasters_count) {
        clasters_vect.at(idx).push_back(m_dataset.at(i));
        clasters_sums.at(idx).at(0) += m_dataset.at(i).at(0);
        clasters_sums.at(idx).at(1) += m_dataset.at(i).at(1);
        ++clasters_sizes.at(idx);
      } else 
        printf("!!! %d>%d !!!\n", idx + 1, clasters_count);
      ;
    }
    // 2. get x1, y1, x2, y2 coordinates corresponding to each of those angles and put these points into LinesVec
    std::vector<std::vector<double>> clasters_means(clasters_count, std::vector<double>(2));
    m_result.reserve(clasters_count);
    for (int i = 0; i < clasters_count; ++i) {
      auto fi1_mean = clasters_sums.at(i).at(0) / static_cast<double>(clasters_sizes.at(i));
      auto fi2_mean = clasters_sums.at(i).at(1) / static_cast<double>(clasters_sizes.at(i));
      // clasters_means.emplace_back(std::vector<double>{fi1_mean, fi2_mean});
      clasters_means.at(i).at(0) = fi1_mean;
      clasters_means.at(i).at(1) = fi2_mean;
      double x1, y1, x2, y2;
      x1 = m_r_o * cos(fi1_mean / 180. * M_PI) + m_x_o;
      y1 = m_r_o * sin(fi1_mean / 180. * M_PI) + m_y_o;
      x2 = m_r_o * cos(fi2_mean / 180. * M_PI) + m_x_o;
      y2 = m_r_o * sin(fi2_mean / 180. * M_PI) + m_y_o;
      m_result.emplace_back(cv::Point(x1, y1), cv::Point(x2, y2));
    }

    
    printf("\n");
    ;
  }
};

#endif
