#ifndef LINES_H
#define LINES_H

#include "Line.h"
using vus = std::vector<unsigned short>;

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
    std::vector<unsigned short> getAngleHistogram(int segments) {
        std::vector<unsigned short> histogram(segments, 0);
        double pivot = 90 - 180/static_cast<double>(segments);
        int j = 0;
        for (int i = 0; i < linesvec.size(); i++) {
            if (linesvec[i].angle > pivot) {
                histogram[j]++;
            } else {
                pivot -= 180/static_cast<double>(segments);
                j++;
                i--;
            }
        }
        std::cout << std::endl << "Histogram:" << std::endl;
        for (int i = 0; i < segments; i++) {
            std::cout << i << " " << histogram[i] << std::endl;
        }
        return histogram;
    }
    Lines GetHVlines() {
        std::vector<unsigned short> hist = getAngleHistogram(100);
        Lines filteredLines;
        unsigned short length = hist.size();
        unsigned short cluster = length / 4.;
        vus hist1(hist.begin(), hist.begin() + length/4);
        vus hist2(hist.begin() + length/4 + 1, hist.begin() + 3*length/4);
        vus hist3(hist.begin() + 3*length/4 + 1, hist.end());
        auto it1 = std::max_element(hist1.begin(), hist1.end());
        auto it2 = std::max_element(hist2.begin(), hist2.end());
        auto it3 = std::max_element(hist3.begin(), hist3.end());
        auto angles1_przedzial = -90 + (it1 - hist1.begin()) * (180 / length);
        auto angles2_przedzial = -90 + (it2 - hist2.begin()) * (180 / length);
        auto angles3_przedzial = -90 + (it3 - hist3.begin()) * (180 / length);
        for (std::size_t i = 0; i < linesvec.size(); i++) {
            if (linesvec[i].angle > angles1_przedzial && linesvec[i].angle < angles1_przedzial + (180/length)) {
                filteredLines.linesvec.push_back(linesvec[i]);
            } else if (linesvec[i].angle > angles2_przedzial && linesvec[i].angle < angles2_przedzial + (180/length)) {
                filteredLines.linesvec.push_back(linesvec[i]);
            } else if (linesvec[i].angle > angles3_przedzial && linesvec[i].angle < angles3_przedzial + (180/length)) {
                filteredLines.linesvec.push_back(linesvec[i]);
            }
        }
        return filteredLines;
    }
};

#endif