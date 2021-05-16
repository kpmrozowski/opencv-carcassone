#ifndef LINES_H
#define LINES_H

#include "Line.h"

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
};

#endif