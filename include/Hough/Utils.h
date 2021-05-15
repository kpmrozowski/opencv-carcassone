#ifndef UTILS_H
#define UTILS_H

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <experimental/filesystem>
#include <algorithm>
#include <cmath>

#define _USE_MATH_DEFINES

std::vector<unsigned short> getAnglesHistogram(std::vector<double>& angles, int segments) {
    std::sort(angles.begin(), angles.end());
    std::vector<unsigned short> histogram(segments, 0);
    double pivot = -M_PI/2 + M_PI / segments;
    int j = 0;
    for (int i = 0; i < angles.size(); i++) {
        if (angles[i] < pivot) {
            histogram[j] += 1;
        } else {
            pivot += M_PI / segments;
            j++;
            i--;
        }
    }
    std::cout << "Histogram:" << std::endl;
    for (auto h : histogram) {
        std::cout << h << std::endl;
    }
    return histogram;
}

std::vector<double> getAngles(std::vector<cv::Vec4i>& linesin) {
    std::vector<double> angles;
    for (const auto& line : linesin) {
        if (line[2] == line[0]) {
            angles.push_back(static_cast<double>(M_PI/2.));
        } else {
            angles.push_back(std::atan((line[3] - line[1])/(line[2] - line[0])));
        }
    }
    return angles;
}

#endif