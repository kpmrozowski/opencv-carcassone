#ifndef UTILS_H
#define UTILS_H

#include "Lines.h"

#define _USE_MATH_DEFINES
using vus = std::vector<unsigned short>;
using vcv4 = std::vector<cv::Vec4i>;

vcv4 GetHVlines(std::pair<vus, vus>& pair, vcv4 linesP, std::vector<double>& angles) {
    vcv4 linesPHV;
    vus hist{pair.first};
    vus idx{pair.second};
    unsigned short length = hist.size();
    unsigned short cluster = length / 4.;
    vus hist1(hist.begin(), hist.begin() + length/4);
    vus hist2(hist.begin()+length/4+1, hist.begin() + 3*length/4);
    vus hist3(hist.begin()+3*length/4+1, hist.end());
    auto it1 = std::max_element(hist1.begin(), hist1.end());
    auto it2 = std::max_element(hist2.begin(), hist2.end());
    auto it3 = std::max_element(hist3.begin(), hist3.end());
    auto angles1_przedzial = -M_PI/2 + (it1 - hist1.begin()) * (M_PI / length);
    auto angles2_przedzial = -M_PI/2 + (it2 - hist2.begin()) * (M_PI / length);
    auto angles3_przedzial = -M_PI/2 + (it3 - hist3.begin()) * (M_PI / length);
    std::vector<std::pair<vcv4, std::vector<double>>> linesAngles(angles.size());
    for (std::size_t i = 0; i < angles.size(); i++) {
        if (angles[i] > angles1_przedzial && angles[i] < angles1_przedzial + (M_PI/length)) {
            linesPHV.push_back(linesP[i]);
        } else if (angles[i] > angles2_przedzial && angles[i] < angles2_przedzial + (M_PI/length)) {
            linesPHV.push_back(linesP[i]);
        } else if (angles[i] > angles3_przedzial && angles[i] < angles3_przedzial + (M_PI/length)) {
            linesPHV.push_back(linesP[i]);
        }
    }
        // linesAngles[i] = std::make_pair(linesP[i], angles[i]);
    
    // -M_PI/2 + (it1 - hist1.begin()) * (M_PI / length)
    
    // auto std::max()
    return linesPHV;
}

#endif