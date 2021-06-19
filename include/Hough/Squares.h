#ifndef SQUARES_H
#define SQUARES_H


// The "Square Detector" program.
// It loads several images sequentially and tries to find squares in
// each image
#include <fmt/core.h>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <Utils/display.h>
#include <iostream>

namespace twm::hough {
using namespace cv;
using namespace std;

const char* wndname = "Square Detection Demo";
static double angle( cv::Point pt1, cv::Point pt2, cv::Point pt0 )
{
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}


static std::vector<std::vector<cv::Point> > findSquares( const cv::Mat& image, unsigned int &desired_size) {
    std::vector<std::vector<cv::Point> > squares;
    int image_width = image.cols;
    int image_height = image.rows;
    bool first_tile = desired_size == std::max(image_height, image_width);

    Mat pyr, timg, gray0(image.size(), CV_8U), gray;
    

    pyrDown(image, pyr, Size(image.cols/2, image.rows/2));
    ////if(first_tile) { display("pyrDown", pyr);    destroyWindow("pyrDown"); }


    pyrUp(pyr, timg, image.size());
    ////if(first_tile) { display("pyrUp", timg);    destroyWindow("pyrUp"); }
    vector<vector<cv::Point> > contours;
    int thresh = 10, nn = 8;
    unsigned int canny_treshold_A = 9000, canny_treshold_B = 7000;
    double contour_area = 10000.;
    double maxCosine_tolerance = 0.1;
    double diag_angle_tollerance = 7.; // stopni
    double sides_diff_tollerance = 10.;
    for( int c = 0; c < 3; c++ ) {
        int ch[] = {c, 0};
        mixChannels(&timg, 1, &gray0, 1, ch, 1);
        for( int l = 0; l < nn; l++ ) {
            if( l == 0 ) {


                Canny(gray0, gray, canny_treshold_A, canny_treshold_B, 7);
                ////if(first_tile && c == 0) { display("Canny", gray);    destroyWindow("Canny"); }


                dilate(gray, gray, cv::Mat(), cv::Point(-1,-1));
                ////if(first_tile && c == 0) { display("dilate", gray);    destroyWindow("dilate"); }
            } else {
                gray = gray0 >= (l+1)*255/nn;
            }


            findContours(gray, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
            std::vector<cv::Point> approx;
            // test each contour
            for( size_t i = 0; i < contours.size(); i++ )
            {


                approxPolyDP(contours[i], approx, arcLength(contours[i], true)*0.02, true);
                if ( approx.size() != 4 )
                    continue;
                if( fabs(contourArea(approx)) < contour_area )
                    continue;
                if( !isContourConvex(approx) )
                    continue;
                double maxCosine = 0;

                for( int j = 2; j < 5; j++ ) {
                    double cosine = fabs(angle(approx[j%4], approx[j-2], approx[j-1]));
                    maxCosine = MAX(maxCosine, cosine);
                }
                if (maxCosine > maxCosine_tolerance)
                    continue;
                

                double scalar_product = abs((approx[0].x - approx[2].x) * (approx[1].x - approx[3].x) + (approx[0].y - approx[2].y) * (approx[1].y - approx[3].y));
                double len_diag1 = sqrt(pow(approx[0].x - approx[2].x, 2) + pow(approx[0].y - approx[2].y, 2));
                double len_diag2 = sqrt(pow(approx[1].x - approx[3].x, 2) + pow(approx[1].y - approx[3].y, 2));
                double diag_angle = scalar_product / (len_diag1 * len_diag2);
                if(abs(diag_angle) > CV_PI / 180 * diag_angle_tollerance)
                    continue;

                Square sq(approx);
                // fmt::print("abs(sq.NW.x - sq.NE.x)={}\n", abs(sq.NW.x - sq.NE.x));
                // fmt::print("abs(sq.NW.x - sq.SW.x)={}\n", abs(sq.NW.x - sq.SW.x));
                // fmt::print("abs(sq.NW.y - sq.SW.y)={}\n", abs(sq.NW.y - sq.SW.y));
                // fmt::print("abs(sq.NW.y - sq.NE.y)={}\n", abs(sq.NW.y - sq.NE.y));


                if (std::abs(sq.NW.x - sq.NE.x) > 0.4 * image_width) 
                    continue;
                if (std::abs(sq.NW.x - sq.SW.x) > 0.4 * image_width) 
                    continue;
                if (std::abs(sq.NW.y - sq.SW.y) > 0.4 * image_width)
                    continue; 
                if (std::abs(sq.NW.y - sq.NE.y) > 0.4 * image_width)
                    continue; 
                // fmt::print("desired_size={}\n", desired_size);


                if (!first_tile) {
                    bool cond1 = abs(sq.NW.x - sq.NE.x) < 0.8 * desired_size;
                    bool cond2 = abs(sq.NW.x - sq.SW.x) < 0.8 * desired_size;
                    bool cond3 = abs(sq.NW.y - sq.SW.y) < 0.8 * desired_size;
                    bool cond4 = abs(sq.NW.y - sq.NE.y) < 0.8 * desired_size;
                    if (cond1 && cond2 && cond3 && cond4)
                        continue;
                    bool cond5 = abs(sq.NW.x - sq.NE.x) > 1.2 * desired_size;
                    bool cond6 = abs(sq.NW.x - sq.SW.x) > 1.2 * desired_size;
                    bool cond7 = abs(sq.NW.y - sq.SW.y) > 1.2 * desired_size;
                    bool cond8 = abs(sq.NW.y - sq.NE.y) > 1.2 * desired_size;
                    if (cond5 || cond6 || cond7 || cond8)
                        continue;
                }

                double N = sqrt(pow(sq.NW.x - sq.NE.x, 2) + pow(sq.NW.y - sq.NE.y, 2));
                double E = sqrt(pow(sq.SE.x - sq.NE.x, 2) + pow(sq.SE.y - sq.NE.y, 2));
                double S = sqrt(pow(sq.SW.x - sq.SE.x, 2) + pow(sq.SW.y - sq.SE.y, 2));
                double W = sqrt(pow(sq.SW.x - sq.NW.x, 2) + pow(sq.SW.y - sq.NW.y, 2));
                // fmt::print("N={:2f} ", N);
                // fmt::print("E={:2f} ", E);
                // fmt::print("S={:2f} ", S);
                // fmt::print("W={:2f}\n", W);
                bool cond1 = abs(N - E) > sides_diff_tollerance;
                bool cond2 = abs(E - S) > sides_diff_tollerance;
                bool cond3 = abs(S - W) > sides_diff_tollerance;
                bool cond4 = abs(W - N) > sides_diff_tollerance;
                if (cond1 || cond2 || cond3 || cond4)
                    continue;
                squares.push_back(approx);
                // fmt::print("squares.push_back\n");
                // fmt::print("fabs(contourArea(approx))={}\n", fabs(contourArea(approx)));
                // fmt::print("maxCosine={}stopni\n", maxCosine * M_1_PI * 180);
                // fmt::print("diag_angle={}stopni\n", diag_angle * M_1_PI * 180);
            }
        }
        if (c == 2 && squares.size() == 0) {
            // fmt::print("DETEKCJA NAWALILA! LUZOWANIE PARAMETROW...\n"); // drukuje jak ma czas
            std::cout << "DETEKCJA NAWALILA! LUZOWANIE PARAMETROW..." << std::endl; // drukuje natychmiast
            c = 0;
            nn *= 1.2;
            canny_treshold_A *= 0.9;
            canny_treshold_B *= 0.95;
            maxCosine_tolerance *= 1.05;
            diag_angle_tollerance *= 1.05;
            sides_diff_tollerance *= 1.2;
            fmt::print("{} {} {} {:0.2f} {:0.2f} {:0.2f}\n", nn, canny_treshold_A, canny_treshold_B, maxCosine_tolerance, diag_angle_tollerance, sides_diff_tollerance);

        }
    }
    return squares;
}

} //namespace twm::hough

#endif