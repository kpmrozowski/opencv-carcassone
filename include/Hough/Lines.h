#ifndef LINES_H
#define LINES_H

#include "Line.h"
#include  <limits>
using size_t = std::size_t;
using vus = std::vector<size_t>;
using count = size_t;
using pivot_angle = double;
using Histogram = std::vector<std::pair<pivot_angle, count>>;

class Lines
{
public:
    std::vector<Line> linesvec;
    Lines(){}
    Lines(std::vector<cv::Vec4i>& linesCoords)
    {
        linesvec.reserve(linesCoords.size());
        for (auto lc : linesCoords) {
            linesvec.push_back(Line(lc));
        }
        sortByAngles();
    }
    Line& operator[](int index)
    {
        if (index >= linesvec.size()) {
            std::cout << "Lines index out of bound, exiting" << std::endl;
            exit(0);
        }
        return linesvec[index];
    }
    void sortByAngles() {
        std::sort(linesvec.begin(), linesvec.end(), [](const Line& l1, const Line& l2) -> bool
            { return l1.angle > l2.angle; }
        );
    }
    void print() {
        std::cout << std::endl << "Lines: " << std::endl;
        for (auto line : linesvec) {
            line.print();
        }
    }

    Histogram getAngleHistogram(int segments) {
        Histogram histogram(segments, std::make_pair(-std::numeric_limits<double>::infinity(), 0));
        double pivot_angle = 90 - 180/static_cast<double>(segments);
        size_t pivot_idx = 0;
        for (int i = 0; i < linesvec.size(); i++) {
            if (linesvec[i].angle > pivot_angle) {
                histogram[pivot_idx].second++;
            } else {
                histogram[pivot_idx].first = pivot_angle;
                pivot_idx++;
                pivot_angle -= 180/static_cast<double>(segments);
                i--;
            }
        }
        (*(histogram.end() - 1)).first = -90.;
        std::cout << std::endl << "Histogram:" << std::endl;
        for (int i = 0; i < segments; i++) {
            std::cout << "i: " << i << "\tangle: " << histogram[i].first << "\tcount: " << histogram[i].second << std::endl;
        }
        return histogram;
    }

    Lines GetHVlines() {
        Histogram hist = getAngleHistogram(12);
        Lines filteredLines;
        size_t length = hist.size();
        size_t cluster = length / 4.;
        Histogram hist1(hist.begin(), hist.begin() + length/4);
        Histogram hist2(hist.begin() + length/4 + 1, hist.begin() + 3*length/4);
        Histogram hist3(hist.begin() + 3*length/4 + 1, hist.end());
        size_t len1 = hist1.size();
        size_t len2 = hist2.size();
        size_t len3 = hist3.size();
        hist1.insert(std::end(hist1), std::begin(hist3), std::end(hist3));
        auto it1 = std::max_element(hist1.begin(), hist1.end(), [](const auto& lhs, const auto& rhs) { return lhs.second < rhs.second; });
        auto it2 = std::max_element(hist2.begin(), hist2.end(), [](const auto& lhs, const auto& rhs) { return lhs.second < rhs.second; });
        std::cout << "pivot_angle1: " << (*it1).first << std::endl;
        std::cout << "pivot_angle2: " << (*it2).first << std::endl;

        for (std::size_t i = 0; i < linesvec.size(); i++) {
            if (linesvec[i].angle > (*it1).first && linesvec[i].angle < (*it1).first + 180/length) {
                filteredLines.linesvec.push_back(linesvec[i]);
            } else if (linesvec[i].angle > (*it2).first && linesvec[i].angle < (*it2).first + 180/length) {
                filteredLines.linesvec.push_back(linesvec[i]);
            }
        }
        return filteredLines;
    }
};

#endif