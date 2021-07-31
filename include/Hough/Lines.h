#ifndef LINES_H
#define LINES_H

#include <fmt/core.h>

#include <limits>
#include <numeric>
#include <tuple>

#include "Line.h"
#include "Point.h"
#include "Square.h"
using size_t = std::size_t;
using vus = std::vector<size_t>;
using pivot_angle = double;
using Histogram = std::vector<std::pair<pivot_angle, size_t>>;

/* a function to generate numpy linspace */
template <typename T>
std::vector<T> linspace(double start, double end, double num) {
  std::vector<T> linspaced;
  if (0 != num) {
    if (1 == num) {
      linspaced.push_back(static_cast<T>(start));
    } else {
      double delta = (end - start) / (num - 1);
      for (auto i = 0; i < num; ++i) {
        linspaced.push_back(static_cast<T>(round(start + delta * i)));
      }
    }
  }
  return linspaced;
}

class Lines {
 public:
  std::vector<Square> m_squares;
  std::vector<std::pair<Line, Line>> m_pararrel_pairs;
  std::vector<Line> m_linesvec;
  Lines() {}
  Lines(std::vector<cv::Vec4i>& linesm_coords) {
    m_linesvec.reserve(linesm_coords.size());
    for (auto lc : linesm_coords) {
      m_linesvec.push_back(Line(lc));
    }
    sortByAngles();
  }
  Line& operator[](int index) {
    if (index >= m_linesvec.size()) {
      std::cout << "Lines index out of bound, exiting" << std::endl;
      exit(0);
    }
    return m_linesvec[index];
  }

  [[nodiscard]] inline std::tuple<double, double, double> cartesianToGeneral(
      const Line& l) {
    double x1 = l.p1().x;
    double y1 = l.p1().y;
    double x2 = l.p2().x;
    double y2 = l.p2().y;
    double a = y2 - y1;
    double b = x2 - x1;
    double c = (x1 - x2) * y2 + (y1 - y2) * x2;
    return std::make_tuple(a, b, c);
  }

  void sortByAngles() {
    std::sort(m_linesvec.begin(), m_linesvec.end(),
              [](const Line& l1, const Line& l2) -> bool {
                return l1.angle() > l2.angle();
              });
  }
  void print() {
    std::cout << std::endl << "Lines: " << std::endl;
    for (auto line : m_linesvec) {
      line.print();
    }
  }

  Histogram getAngleHistogram(int segments) {
    Histogram histogram(
        segments, std::make_pair(-std::numeric_limits<double>::infinity(), 0));
    double pivot_angle = 90 - 180 / static_cast<double>(segments);
    size_t pivot_idx = 0;
    for (int i = 0; i < m_linesvec.size(); i++) {
      if (m_linesvec[i].angle() > pivot_angle) {
        histogram[pivot_idx].second++;
      } else {
        histogram[pivot_idx].first = pivot_angle;
        pivot_idx++;
        pivot_angle -= 180 / static_cast<double>(segments);
        i--;
      }
    }
    (*(histogram.end() - 1)).first = -90.;
    std::cout << std::endl << "Histogram:" << std::endl;
    for (int i = 0; i < segments; i++) {
      std::cout << "i: " << i << "\tangle: " << histogram[i].first
                << "\tcount: " << histogram[i].second << std::endl;
    }
    return histogram;
  }

  Lines GetHVlines() {
    Histogram hist = getAngleHistogram(90);
    Lines filteredLines;
    size_t length = hist.size();
    // size_t cluster = length / 4.;
    Histogram hist1(hist.begin(), hist.begin() + length / 4);
    Histogram hist2(hist.begin() + length / 4 + 1,
                    hist.begin() + 3 * length / 4);
    Histogram hist3(hist.begin() + 3 * length / 4 + 1, hist.end());
    size_t len1 = hist1.size();
    size_t len2 = hist2.size();
    size_t len3 = hist3.size();
    hist1.insert(std::end(hist1), std::begin(hist3), std::end(hist3));
    auto it1 = std::max_element(hist1.begin(), hist1.end(),
                                [](const auto& lhs, const auto& rhs) {
                                  return lhs.second < rhs.second;
                                });
    auto it2 = std::max_element(hist2.begin(), hist2.end(),
                                [](const auto& lhs, const auto& rhs) {
                                  return lhs.second < rhs.second;
                                });
    std::cout << "pivot_angle1: " << (*it1).first << std::endl;
    std::cout << "pivot_angle2: " << (*it2).first << std::endl;

    for (std::size_t i = 0; i < m_linesvec.size(); i++) {
      if (m_linesvec[i].angle() > (*it1).first &&
          m_linesvec[i].angle() < (*it1).first + 180 / length) {
        filteredLines.m_linesvec.push_back(m_linesvec[i]);
      } else if (m_linesvec[i].angle() > (*it2).first &&
                 m_linesvec[i].angle() < (*it2).first + 180 / length) {
        filteredLines.m_linesvec.push_back(m_linesvec[i]);
      }
    }
    return filteredLines;
  }

  void GetHVkMeans_example() {
    const int MAX_CLUSTERS = 5;
    cv::Scalar colorTab[] = {cv::Scalar(0, 0, 255), cv::Scalar(0, 255, 0),
                             cv::Scalar(255, 100, 100), cv::Scalar(255, 0, 255),
                             cv::Scalar(0, 255, 255)};
    cv::Mat img(500, 500, CV_8UC3);
    cv::RNG rng(12345);
    for (;;) {
      int k, clusterCount = rng.uniform(2, MAX_CLUSTERS + 1);
      int i, sampleCount = rng.uniform(1, 1001);
      cv::Mat points(sampleCount, 1, CV_32FC2), labels;
      clusterCount = std::min(clusterCount, sampleCount);
      std::vector<cv::Point2f> centers;
      /* generate random sample from multigaussian distribution */
      for (k = 0; k < clusterCount; k++) {
        cv::Point center;
        center.x = rng.uniform(0, img.cols);
        center.y = rng.uniform(0, img.rows);
        cv::Mat pointChunk = points.rowRange(
            k * sampleCount / clusterCount,
            k == clusterCount - 1 ? sampleCount
                                  : (k + 1) * sampleCount / clusterCount);
        rng.fill(pointChunk, cv::RNG::NORMAL, cv::Scalar(center.x, center.y),
                 cv::Scalar(img.cols * 0.05, img.rows * 0.05));
      }
      cv::randShuffle(points, 1, &rng);
      double compactness = cv::kmeans(
          points, clusterCount, labels,
          cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT,
                           1000, .01),
          3, cv::KMEANS_PP_CENTERS, centers);
      img = cv::Scalar::all(0);
      for (i = 0; i < sampleCount; i++) {
        int clusterIdx = labels.at<int>(i);
        cv::Point ipt = points.at<cv::Point2f>(i);
        cv::circle(img, ipt, 2, colorTab[clusterIdx], cv::FILLED, cv::LINE_AA);
      }
      for (i = 0; i < (int)centers.size(); ++i) {
        cv::Point2f c = centers[i];
        cv::circle(img, c, 40, colorTab[i], 1, cv::LINE_AA);
      }
      std::cout << "Compactness: " << compactness << std::endl;
      cv::imshow("clusters", img);
      char key = (char)cv::waitKey();
      if (key == 27 || key == 'q' || key == 'Q')  // 'ESC'
        break;
    }
  }

  std::tuple<std::vector<Line>, std::vector<Line>> GetHVkMeans(
      int clusterCount = 0) {
    cv::Scalar colorTab[] = {cv::Scalar(0, 0, 255), cv::Scalar(0, 255, 0),
                             cv::Scalar(255, 100, 100), cv::Scalar(255, 0, 255),
                             cv::Scalar(0, 255, 255)};
    cv::Mat img(500, 500, CV_8UC3);
    cv::RNG rng(12345);
    int sampleCount = m_linesvec.size();
    clusterCount = std::min(clusterCount, sampleCount);
    std::vector<float> centers(clusterCount);
    std::vector<int> labels(sampleCount);
    std::vector<float> cosines(sampleCount);
    std::cout << "sampleCount = " << sampleCount << std::endl;
    std::transform(m_linesvec.begin(), m_linesvec.end(), cosines.begin(),
                   [](const Line& l) -> float {
                     auto angle = static_cast<float>(l.angle());
                     // std::cout << "angle = " << angle << std::endl;
                     if (angle < -90. || angle > 180.) {
                       std::cout << "angle < -90. || angle > 90." << std::endl;
                       return static_cast<float>(.5);
                     } else
                       return static_cast<float>(l.angle());
                   });
    cv::Mat cosines_Mat(sampleCount, 1, CV_32FC1, &cosines[0]);

    double compactness = cv::kmeans(
        cosines_Mat, clusterCount, labels,
        cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT,
                         100000, 0.00001),
        3, cv::KMEANS_PP_CENTERS, centers);
    std::cout << "Compactness: " << compactness << std::endl;

    std::vector<int> labelsCounts(clusterCount, 0);
    for (const auto& label : labels) ++labelsCounts[label];

    img = cv::Scalar::all(0);
    for (int i = 0; i < sampleCount; i++) {
      int clusterIdx = labels[i];
      auto angle = std::acos(cosines.at(i));
      cv::Point ipt(angle, angle);
      cv::circle(img, ipt, 2, colorTab[clusterIdx], cv::FILLED, cv::LINE_AA);
    }
    int firstLabel, secondLabel;
    std::vector<std::pair<Line, int>> linesCounts;
    std::transform(m_linesvec.begin(), m_linesvec.end(), labelsCounts.begin(),
                   linesCounts.begin(), [](const Line& line, const int& count) {
                     return std::make_pair(line, count);
                   });
    std::vector<int> indices(clusterCount);
    std::iota(indices.begin(), indices.end(), 0);
    std::sort(indices.begin(), indices.end(),
              [labelsCounts](int A, int B) -> bool {
                return labelsCounts[A] > labelsCounts[B];
              });
    std::nth_element(labelsCounts.begin(), labelsCounts.begin() + 1,
                     labelsCounts.end(), [labelsCounts](int A, int B) -> bool {
                       return labelsCounts[A] > labelsCounts[B];
                     });
    for (int i = 0; i < (int)centers.size(); ++i) {
      cv::Point2f c(centers[i], centers[i]);
      cv::circle(img, c, 40, colorTab[i], 1, cv::LINE_AA);
    }

    std::vector<Line> claster1, claster2;
    for (auto it = m_linesvec.begin(); it != m_linesvec.end(); ++it) {
      auto i = it - m_linesvec.begin();
      // if (labels[i] == )
    }
    return std::make_tuple(claster1, claster2);

    // cv::imshow("clusters", img);
    // cv::waitKey();
  }

  Lines GetHVlinesSimple(int tolerance) {
    Lines filteredLines;
    for (std::size_t i = 0; i < m_linesvec.size(); i++) {
      if (m_linesvec[i].angle() > (90 - tolerance) ||
          m_linesvec[i].angle() < (-90 + tolerance) ||
          m_linesvec[i].angle() < tolerance &&
              m_linesvec[i].angle() > -tolerance) {
        filteredLines.m_linesvec.push_back(this->m_linesvec[i]);
      }
    }
    return filteredLines;
  }

  void findPairs(double angle_tollerance = 3, double min_dist = 10,
                 double max_dist = 1000) {
    for (size_t i = 0; i < m_linesvec.size(); i++) {
      double a, b, c;
      std::tie(a, b, c) = cartesianToGeneral(m_linesvec[i]);
      for (size_t j = i + 1; j < m_linesvec.size(); j++) {
        double x0 = (m_linesvec[j].p1().x + m_linesvec[j].p2().x) / 2;
        double y0 = (m_linesvec[j].p1().y + m_linesvec[j].p2().y) / 2;
        double d = abs(a * x0 + b * y0 + c) / sqrt(pow(a, 2) + pow(b, 2));
        bool cond1 = abs(m_linesvec[i].angle() - m_linesvec[j].angle()) <
                     angle_tollerance;
        bool cond2a = d > min_dist;
        bool cond2b = d < max_dist;
        if (cond1 && cond2a && cond2b)
          m_pararrel_pairs.push_back(
              std::make_pair(m_linesvec[i], m_linesvec[j++]));
      }
    }
  }

  std::vector<Square> getSquares(int HVdistTolerance = 400,
                                 double diagAngleTolerance = 10) {
    // std::vector<Square> squares;

    for (size_t i = 0; i < m_pararrel_pairs.size(); i++) {
      for (size_t j = i + 1; j < m_pararrel_pairs.size(); j++) {
        Point NW, NE, SW, SE;
        int h_dist, v_dist;
        Line N, S, W, E;
        // std::cout << std::endl << "Vertical and Horizontal?" <<
        // (m_pararrel_pairs[i].first.isVertical() !=
        // m_pararrel_pairs[j].first.isVertical() ? "yes" : "no") << std::endl;
        if (m_pararrel_pairs[i].first.isVertical() !=
            m_pararrel_pairs[j].first.isVertical()) {
          if (m_pararrel_pairs[i].first.isVertical()) {
            if (m_pararrel_pairs[i].first.p1().x <
                m_pararrel_pairs[i].second.p1().x) {
              if (m_pararrel_pairs[j].first.p1().y >
                  m_pararrel_pairs[j].second.p1().y) {
                // std::cout << "(" << i << "," << j << "): if1 ";
                 NW = Point(m_pararrel_pairs[i].first.p1().x,
                            m_pararrel_pairs[j].first.p1().y);
                NE = Point(m_pararrel_pairs[i].second.p1().x,
                            m_pararrel_pairs[j].first.p1().y);
                 SW = Point(m_pararrel_pairs[i].first.p1().x,
                           m_pararrel_pairs[j].second.p1().y);
                SE = Point(m_pararrel_pairs[i].second.p1().x,
                           m_pararrel_pairs[j].second.p1().y);
                N = m_pararrel_pairs[j].first;
                E = m_pararrel_pairs[i].second;
                S = m_pararrel_pairs[j].second;
                W = m_pararrel_pairs[i].first;
              } else {
                // std::cout << "(" << i << "," << j << "): if2 ";
                NW = Point(m_pararrel_pairs[i].first.p1().x,
                           m_pararrel_pairs[j].second.p1().y);
                NE = Point(m_pararrel_pairs[i].second.p1().x,
                           m_pararrel_pairs[j].second.p1().y);
                SW = Point(m_pararrel_pairs[i].first.p1().x,
                           m_pararrel_pairs[j].first.p1().y);
                SE = Point(m_pararrel_pairs[i].second.p1().x,
                           m_pararrel_pairs[j].first.p1().y);
                N = m_pararrel_pairs[j].second;
                E = m_pararrel_pairs[i].second;
                S = m_pararrel_pairs[j].first;
                W = m_pararrel_pairs[i].first;
              }
            } else {
              if (m_pararrel_pairs[j].first.p1().y >
                  m_pararrel_pairs[j].second.p1().y) {
                // std::cout << "(" << i << "," << j << "): if3 ";
                NW = Point(m_pararrel_pairs[i].second.p1().x,
                           m_pararrel_pairs[j].first.p1().y);
                NE = Point(m_pararrel_pairs[i].first.p1().x,
                           m_pararrel_pairs[j].first.p1().y);
                SW = Point(m_pararrel_pairs[i].second.p1().x,
                           m_pararrel_pairs[j].second.p1().y);
                SE = Point(m_pararrel_pairs[i].first.p1().x,
                           m_pararrel_pairs[j].second.p1().y);
                N = m_pararrel_pairs[j].first;
                E = m_pararrel_pairs[i].first;
                S = m_pararrel_pairs[j].second;
                W = m_pararrel_pairs[i].second;
              } else {
                // std::cout << "(" << i << "," << j << "): if4 ";
                NW = Point(m_pararrel_pairs[i].second.p1().x,
                           m_pararrel_pairs[j].second.p1().y);
                NE = Point(m_pararrel_pairs[i].first.p1().x,
                           m_pararrel_pairs[j].second.p1().y);
                SW = Point(m_pararrel_pairs[i].second.p1().x,
                           m_pararrel_pairs[j].first.p1().y);
                SE = Point(m_pararrel_pairs[i].first.p1().x,
                           m_pararrel_pairs[j].first.p1().y);
                N = m_pararrel_pairs[j].second;
                E = m_pararrel_pairs[i].first;
                S = m_pararrel_pairs[j].first;
                W = m_pararrel_pairs[i].second;
              }
            }
          } else {
            if (m_pararrel_pairs[j].first.p1().x <
                m_pararrel_pairs[j].second.p1().x) {
              if (m_pararrel_pairs[i].first.p1().y >
                  m_pararrel_pairs[i].second.p1().y) {
                // std::cout << "(" << i << "," << j << "): if5 ";
                NW = Point(m_pararrel_pairs[j].first.p1().x,
                           m_pararrel_pairs[i].first.p1().y);
                NE = Point(m_pararrel_pairs[j].second.p1().x,
                           m_pararrel_pairs[i].first.p1().y);
                SW = Point(m_pararrel_pairs[j].first.p1().x,
                           m_pararrel_pairs[i].second.p1().y);
                SE = Point(m_pararrel_pairs[j].second.p1().x,
                           m_pararrel_pairs[i].second.p1().y);
                N = m_pararrel_pairs[i].first;
                E = m_pararrel_pairs[j].second;
                S = m_pararrel_pairs[i].second;
                W = m_pararrel_pairs[j].first;
              } else {
                // std::cout << "(" << i << "," << j << "): if6 ";
                NW = Point(m_pararrel_pairs[j].first.p1().x,
                           m_pararrel_pairs[i].second.p1().y);
                NE = Point(m_pararrel_pairs[j].second.p1().x,
                           m_pararrel_pairs[i].second.p1().y);
                SW = Point(m_pararrel_pairs[j].first.p1().x,
                           m_pararrel_pairs[i].first.p1().y);
                SE = Point(m_pararrel_pairs[j].second.p1().x,
                           m_pararrel_pairs[i].first.p1().y);
                N = m_pararrel_pairs[i].second;
                E = m_pararrel_pairs[j].second;
                S = m_pararrel_pairs[i].first;
                W = m_pararrel_pairs[j].first;
              }
            } else {
              // std::cout << "(" << i << "," << j << "): if7 ";
              if (m_pararrel_pairs[i].first.p1().y >
                  m_pararrel_pairs[i].second.p1().y) {
                NW = Point(m_pararrel_pairs[j].second.p1().x,
                           m_pararrel_pairs[i].first.p1().y);
                NE = Point(m_pararrel_pairs[j].first.p1().x,
                           m_pararrel_pairs[i].first.p1().y);
                SW = Point(m_pararrel_pairs[j].second.p1().x,
                           m_pararrel_pairs[i].second.p1().y);
                SE = Point(m_pararrel_pairs[j].first.p1().x,
                           m_pararrel_pairs[i].second.p1().y);
                N = m_pararrel_pairs[i].first;
                E = m_pararrel_pairs[j].first;
                S = m_pararrel_pairs[i].second;
                W = m_pararrel_pairs[j].second;
              } else {
                // std::cout << "(" << i << "," << j << "): if8 ";
                NW = Point(m_pararrel_pairs[j].second.p1().x,
                           m_pararrel_pairs[i].second.p1().y);
                NE = Point(m_pararrel_pairs[j].first.p1().x,
                           m_pararrel_pairs[i].second.p1().y);
                SW = Point(m_pararrel_pairs[j].second.p1().x,
                           m_pararrel_pairs[i].first.p1().y);
                SE = Point(m_pararrel_pairs[j].first.p1().x,
                           m_pararrel_pairs[i].first.p1().y);
                N = m_pararrel_pairs[i].second;
                E = m_pararrel_pairs[j].first;
                S = m_pararrel_pairs[i].first;
                W = m_pararrel_pairs[j].second;
              }
            }
          }
          double scalar_product = abs((NW.x - SE.x) * (NE.x - SW.x) +
                                      (NW.y - SE.y) * (NE.y - SW.y));
          double len_diag1 = sqrt(pow(NW.x - SE.x, 2) + pow(NW.y - SE.y, 2));
          double len_diag2 = sqrt(pow(NE.x - SW.x, 2) + pow(NE.y - SW.y, 2));
          double diag_angle = scalar_product / (len_diag1 * len_diag2);

          h_dist = abs(NW.x - NE.x);
          v_dist = abs(SW.y - NW.y);
          if (diag_angle / M_PI * 180 - 90 < diagAngleTolerance) {
            std::cout << "HVdist = " << h_dist - v_dist << std::endl;
            if (abs(h_dist - v_dist) < HVdistTolerance) {
              m_squares.push_back(Square(NW, NE, SW, SE, N, E, S, W));
            }
          }
        }
      }
    }
    return m_squares;
  }

  // void find_squares(cv::Mat& image, std::vector<std::vector<cv::Point> >&
  // squares)
  // {
  //     // blur will enhance edge detection
  //     cv::Mat blurred(image);
  //     medianBlur(image, blurred, 9);

  //     cv::Mat gray0(blurred.size(), CV_8U), gray;
  //     std::vector<std::vector<cv::Point> > contours;

  //     // find squares in every color plane of the image
  //     for (int c = 0; c < 3; c++)
  //     {
  //         int ch[] = {c, 0};
  //         mixChannels(&blurred, 1, &gray0, 1, ch, 1);

  //         // try several threshold levels
  //         const int threshold_level = 2;
  //         for (int l = 0; l < threshold_level; l++)
  //         {
  //             // Use Canny instead of zero threshold level!
  //             // Canny helps to catch squares with gradient shading
  //             if (l == 0)
  //             {
  //                 Canny(gray0, gray, 10, 20, 3); //

  //                 // Dilate helps to remove potential holes between edge
  //                 segments dilate(gray, gray, cv::Mat(), cv::Point(-1,-1));
  //             }
  //             else
  //             {
  //                     gray = gray0 >= (l+1) * 255 / threshold_level;
  //             }

  //             // Find contours and store them in a list
  //             findContours(gray, contours, cv::RETR_LIST,
  //             cv::CHAIN_APPROX_SIMPLE);

  //             // Test contours
  //             std::vector<cv::Point> approx;
  //             for (size_t i = 0; i < contours.size(); i++)
  //             {
  //                     // approximate contour with accuracy proportional
  //                     // to the contour perimeter
  //                     approxPolyDP(cv::Mat(contours[i]), approx,
  //                     arcLength(cv::Mat(contours[i]), true)*0.02, true);

  //                     // Note: absolute value of an area is used because
  //                     // area may be positive or negative - in accordance
  //                     with the
  //                     // contour orientation
  //                     if (approx.size() == 4 &&
  //                             fabs(contourArea(cv::Mat(approx))) > 1000 &&
  //                             isContourConvex(cv::Mat(approx)))
  //                     {
  //                             double maxCosine = 0;

  //                             for (int j = 2; j < 5; j++)
  //                             {
  //                                     double cosine =
  //                                     fabs(cv::angle(approx[j%4],
  //                                     approx[j-2], approx[j-1])); maxCosine =
  //                                     MAX(maxCosine, cosine);
  //                             }

  //                             if (maxCosine < 0.3)
  //                                     squares.push_back(approx);
  //                     }
  //             }
  //         }
  //     }
  // }
};

#endif