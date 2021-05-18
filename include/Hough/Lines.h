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
    std::vector<Line> m_linesvec;
    Lines(){}
    Lines(std::vector<cv::Vec4i>& linesCoords)
    {
        m_linesvec.reserve(linesCoords.size());
        for (auto lc : linesCoords) {
            m_linesvec.push_back(Line(lc));
        }
        sortByAngles();
    }
    Line& operator[](int index)
    {
        if (index >= m_linesvec.size()) {
            std::cout << "Lines index out of bound, exiting" << std::endl;
            exit(0);
        }
        return m_linesvec[index];
    }

    void sortByAngles() {
        std::sort(m_linesvec.begin(), m_linesvec.end(), [](const Line& l1, const Line& l2) -> bool
            { return l1.m_angle > l2.m_angle; }
        );
    }
    void print() {
        std::cout << std::endl << "Lines: " << std::endl;
        for (auto line : m_linesvec) {
            line.print();
        }
    }

    std::vector<std::pair<Line,Line>> findPairs (double angle_tollerance) {
        std::vector<std::pair<Line,Line>> pairs;
        // m_linesvec
        for (size_t i = 0; i < m_linesvec.size(); i++) {
            double a = m_linesvec[i].m_coords[3] - m_linesvec[i].m_coords[1];
            double b = m_linesvec[i].m_coords[2] - m_linesvec[i].m_coords[0];;
            // double c = (m_linesvec[i].m_coords);
            for (size_t j = i + 1; j < m_linesvec.size(); j++) {
                if (abs(m_linesvec[i].m_angle - m_linesvec[j].m_angle) < angle_tollerance)
                    pairs.push_back(std::make_pair(m_linesvec[i], m_linesvec[j]));
            }
        }
        return std::vector<std::pair<Line,Line>>(); 
    }

    Histogram getAngleHistogram(int segments) {
        Histogram histogram(segments, std::make_pair(-std::numeric_limits<double>::infinity(), 0));
        double pivot_angle = 90 - 180/static_cast<double>(segments);
        size_t pivot_idx = 0;
        for (int i = 0; i < m_linesvec.size(); i++) {
            if (m_linesvec[i].m_angle > pivot_angle) {
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

        for (std::size_t i = 0; i < m_linesvec.size(); i++) {
            if (m_linesvec[i].m_angle > (*it1).first && m_linesvec[i].m_angle < (*it1).first + 180/length) {
                filteredLines.m_linesvec.push_back(m_linesvec[i]);
            } else if (m_linesvec[i].m_angle > (*it2).first && m_linesvec[i].m_angle < (*it2).first + 180/length) {
                filteredLines.m_linesvec.push_back(m_linesvec[i]);
            }
        }
        return filteredLines;
    }
    Lines GetHVlinesSimple(int tolerance) {
        Lines filteredLines;
        for (std::size_t i = 0; i < linesvec.size(); i++) {
            if (linesvec[i].angle > (90 - tolerance)
                || linesvec[i].angle < (-90 + tolerance)
                || linesvec[i].angle < tolerance && linesvec[i].angle > -tolerance) {
                    filteredLines.linesvec.push_back(this->linesvec[i]);
            }
        }
        return filteredLines;
    }
};

#endif