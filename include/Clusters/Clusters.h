#ifndef CLUSTERS_H
#define CLUSTERS_H

#include <Hough/Line.h>
#include <Hough/Lines.h>
#include <Hough/Point.h>
#include <Utils/matplotlibcpp.h>
#include <cstdlib>
#include <time.h>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <cstring>
#include <limits>
#include <vector>
#include <numeric>
#include <fmt/core.h>
#include <HDBSCAN_CPP/Hdbscan/hdbscan.hpp>
// #include "Square.h"
#define _USE_MATH_DEFINES
#define CORE_POINTS 16       // the number of points to compute core distance
#define MIN_CLUSTER_SIZE 3
#define POINT_SIZE 5

using PointLines = std::vector<std::pair<Point<double>, Point<double>>>;

class LinesClusters {
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
  
  LinesClusters(std::vector<Line> linesvec, int img_rows, int img_cols)
   : m_size(linesvec.size())
   , m_x_o(img_rows / 2.)
   , m_y_o(img_cols / 2.)
   , m_r_o(sqrt(pow(img_rows / 2., 2) + pow(img_cols / 2., 2)))
   , m_distances_table(std::vector<std::vector<double>>(m_size, std::vector<double>(m_size, -1.)))
   , m_core_distances(m_size, -1.)
   , m_mutual_reachability_distances_table(std::vector<std::vector<double>>(m_size, std::vector<double>(m_size, -1.))) {
    twolinesCluster(linesvec, img_rows, img_cols);
    // display_pointlines();
    get_statistics();
    apply_HDBSCAN();
    get_lines_back_from_clusters();
    filter_outliers();
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
  void twolinesCluster(std::vector<Line> linesvec, int img_rows, int img_cols) {
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
  /**
   * @brief Randomly choose a color for a cluster. 
   * For all colors check: https://stackoverflow.com/questions/22408237/named-colors-in-matplotlib 
   * @return a map of "color" and hex
    */
  const std::map<std::string, std::string> random_color() {
    std::string hex_char = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    std::string hex{'#'};
    for (int i = 0; i < 6; ++i) { hex += hex_char[rand()%16]; }
    return std::map<std::string, std::string>{ {"color", hex.c_str() } };
  }

  void display_pointlines() {
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
    matplotlibcpp::scatter(x1, y1, 5.0, random_color());
    matplotlibcpp::scatter(x2, y2, 5.0, random_color());
    matplotlibcpp::show();
    matplotlibcpp::plot(x, y, "r.");
    matplotlibcpp::plot(y, x, "b.");
    matplotlibcpp::show();
    this->print();
    double zz = 1.;
  }

  void display_clusters(std::vector<std::vector<std::vector<double>>> clusters_vect) {
    int i = 0;
    for (const auto& cluster : clusters_vect) {
      std::vector<double> x(cluster.size()), y(cluster.size());
      for (int i = 0; i < cluster.size(); ++i) {
        x.at(i) = cluster.at(i).at(0);
        y.at(i) = cluster.at(i).at(1);
      }
      if (i++) {
        matplotlibcpp::scatter(x, y, 5.0, random_color());
      } else {
        matplotlibcpp::scatter(x, y, 5.0, std::map<std::string, std::string>{ {"color", "lightgray"} });
      }
    }
    matplotlibcpp::show();
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
    for (int i = 0; i < m_size; ++i) {
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
    m_hdbscan.execute(CORE_POINTS, MIN_CLUSTER_SIZE, execution_type);

    m_hdbscan.displayResult();
    // std::endl << "You can access other fields like cluster labels, membership probabilities and outlier scores."<<endl;
    // Use it like this
    m_hdbscan.labels_;
    m_hdbscan.membershipProbabilities_;
    m_hdbscan.outlierScores_;
    printf("\n");
  }

  void get_lines_back_from_clusters() {
    // 1. claculate mean of each cluster (receive two angles from each)

    int clusters_count = *(std::max_element(m_hdbscan.normalizedLabels_.begin(), m_hdbscan.normalizedLabels_.end())) + 1;
    std::vector<std::vector<std::vector<double>>> clusters_vect(clusters_count);
    std::vector<std::vector<double>> clusters_sums(clusters_count, std::vector<double>(2, 0));
    std::vector<std::vector<double>> clusters_means(clusters_count, std::vector<double>(2));
    std::vector<int> clusters_sizes(clusters_count, 0);
    for (int i = 0; i < m_size; ++i) {
      int idx = m_hdbscan.normalizedLabels_.at(i);
      if (idx == -1) ++idx;
      clusters_vect.at(idx).push_back(m_dataset.at(i));
      clusters_sums.at(idx).at(0) += m_dataset.at(i).at(0);
      clusters_sums.at(idx).at(1) += m_dataset.at(i).at(1);
      ++clusters_sizes.at(idx);
    }
    display_clusters(clusters_vect);
    // 2. get x1, y1, x2, y2 coordinates corresponding to each of those angles and put these points into LinesVec
    m_result.reserve(clusters_count);
    for (int i = 0; i < clusters_count; ++i) {
      auto fi1_mean = clusters_sums.at(i).at(0) / static_cast<double>(clusters_sizes.at(i));
      auto fi2_mean = clusters_sums.at(i).at(1) / static_cast<double>(clusters_sizes.at(i));
      // clusters_means.emplace_back(std::vector<double>{fi1_mean, fi2_mean});
      clusters_means.at(i).at(0) = fi1_mean;
      clusters_means.at(i).at(1) = fi2_mean;
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

  void filter_outliers() {
    auto size = m_result.size() * (m_result.size() - 1) / 2;
    int k = 0, m = 0;
    std::vector<Point<double>> intersections;
    std::vector<std::vector<double>> dataset;
    intersections.reserve(size);
    dataset.reserve(size);
    // TODO: lines should come from clusters
    Lines lines;
    lines.m_linesvec = m_result;
    std::vector<std::vector<Line>> hv_lines(lines.GetHVkMeans(2));
    for (int i = 1; i < m_result.size(); ++i) {
      Line line1 = m_result.at(i);
      for (int j = 0; j < i; ++j) {
        Line line2 = m_result.at(j);
        intersections.emplace_back(line1, line2);
        // [i * (i - 1) / 2 + j] = Point(line1, line2);
      }
    }
    for (int i = 0; i < size; ++i) {
      std::vector<double> point(2);
      point.at(0) = intersections.at(i).x;
      point.at(1) = intersections.at(i).y;
      dataset.push_back(point);
    }
    Hdbscan hdbscan;
    hdbscan.dataset = dataset;
    std::string execution_type{"Euclidean"};
    int clusters_count = 0;
    while (clusters_count != 2) {
      hdbscan.execute(CORE_POINTS + k, MIN_CLUSTER_SIZE + m, execution_type);
      hdbscan.displayResult();
      clusters_count = *(std::max_element(hdbscan.normalizedLabels_.begin(), hdbscan.normalizedLabels_.end()));
      if (clusters_count > 2) {
        ++k;
      } else if (CORE_POINTS + k > 1) {
        --k;
      } else {
        k = CORE_POINTS;
        --m;
      }
    }
    clusters_count = 3;
    std::vector<std::vector<std::vector<double>>> clusters_vect(clusters_count);
    std::vector<std::vector<double>> clusters_sums(clusters_count, std::vector<double>(2, 0));
    std::vector<std::vector<double>> clusters_means(clusters_count, std::vector<double>(2));
    std::vector<int> clusters_sizes(clusters_count, 0);
    for (int i = 0; i < size; ++i) {
      int idx = hdbscan.normalizedLabels_.at(i);
      if (idx == -1) ++idx;
      clusters_vect.at(idx).push_back(dataset.at(i));
      clusters_sums.at(idx).at(0) += dataset.at(i).at(0);
      clusters_sums.at(idx).at(1) += dataset.at(i).at(1);
      ++clusters_sizes.at(idx);
    }
    display_clusters(clusters_vect);
    // 2. get x1, y1, x2, y2 coordinates corresponding to each of those angles and put these points into LinesVec
    m_result.reserve(clusters_count);
    for (int i = 0; i < clusters_count; ++i) {
      auto fi1_mean = clusters_sums.at(i).at(0) / static_cast<double>(clusters_sizes.at(i));
      auto fi2_mean = clusters_sums.at(i).at(1) / static_cast<double>(clusters_sizes.at(i));
      // clusters_means.emplace_back(std::vector<double>{fi1_mean, fi2_mean});
      clusters_means.at(i).at(0) = fi1_mean;
      clusters_means.at(i).at(1) = fi2_mean;
    }



    hdbscan.labels_;
    hdbscan.membershipProbabilities_;
    hdbscan.outlierScores_;
    printf("\n");

  }
};

#endif
