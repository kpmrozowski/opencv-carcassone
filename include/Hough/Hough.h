#ifndef TWM_HOUGH_H
#define TWM_HOUGH_H

#include "Lines.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <cmath>

namespace twm::hough {

std::vector<std::string> get_filenames( std::experimental::filesystem::path path ) {
    namespace stdfs = std::experimental::filesystem;

    std::vector<std::string> filenames;
    const stdfs::directory_iterator end{};
    
    for( stdfs::directory_iterator iter{path} ; iter != end ; ++iter )
        if( stdfs::is_regular_file(*iter) )
            filenames.push_back( iter->path().string() );
    return filenames;
}

std::tuple<unsigned char, unsigned char, unsigned char> getColors() {
    return std::tuple<unsigned char, unsigned char, unsigned char>
        (rand() % 255 + 1, rand() % 255 + 1, rand() % 255 + 1);
}

std::vector<cv::Mat> getSquareImages(const cv::Mat &canvas, const std::vector<Square> &squares) {
    std::vector<cv::Mat> squareImages;
    for (auto square : squares) {
        std::vector<int> y_range = std::vector<int>({square.NW.y, square.NE.y, square.SE.y, square.SW.y});
        std::vector<int> x_range = std::vector<int>({square.NW.x, square.NE.x, square.SE.x, square.SW.x});
        auto maxX = std::max_element(x_range.begin(), x_range.end());
        auto minX = std::min_element(x_range.begin(), x_range.end());
        auto maxY = std::max_element(y_range.begin(), y_range.end());
        auto minY = std::min_element(y_range.begin(), y_range.end());
        cv::Rect roi(*minX, *minY, *maxX - *minX, *maxY - *minY);
        cv::Mat canva(canvas,roi);
        squareImages.push_back(canva);
        // std::cout << "canvas.rows = " << canvas.rows << "canvas.cols = " << canvas.cols << std::endl;
        // std::cout << "minX = " << *minX << ", minY = " << *minY << ", maxX = " << *maxX << ", maxY = " << *maxY << std::endl;
        // std::cout << "roi.width = " << roi.width << "roi.height = " << roi.height << std::endl;

    }
    return squareImages;
}

cv::Mat detect_liness(const char* filename) {

    /* Declare the output variables */
    cv::Mat src, dst, cdst, cdstP;

    /* Loads an image */
    cv::Mat src1 = imread( cv::samples::findFile( filename ), cv::IMREAD_GRAYSCALE );
    int frame_width = src1.cols;
	int frame_height = src1.rows;
    float scale = 2160. / frame_height;
    cv::resize(src1, src, cv::Size(static_cast<std::size_t>(frame_width * scale), static_cast<std::size_t>(frame_height * scale)));

    /* Check if image is loaded fine */
    if(src1.empty()) {
        printf(" Error opening image\n");
        printf(" Program Arguments: [image_name -- default %s] \n", filename);;
        return src1;
    }

    /* Edge detection */
    if (false)
        Canny(src, dst, 50, 100, 3, true);
    else 
        Canny(src, dst, 11000, 11000, 7, true);
    
    dilate(dst, dst, cv::Mat(), cv::Point(-1,-1));
    /* Copy edges to the images that will display the results in BGR */
    cvtColor(dst, cdst, cv::COLOR_GRAY2BGR);
    cdstP = cdst.clone();

    if (false)
        imshow("Source", src);
    
    if (false) {
        /* Standard Hough Line Transform */
        std::vector<cv::Vec2f> lines; // will hold the results of the detection
        HoughLines(dst, lines, 1, CV_PI/180, 150); // runs the actual detection
        if (false)
            for (auto line : lines)
                std::cout << line[0] << " " << line[1] << std::endl;
                

        /* Draw the lines */
        for( std::size_t i = 0; i < lines.size(); i++ )
        {
            float rho = lines[i][0], theta = lines[i][1];
            cv::Point pt1, pt2;
            double a = cos(theta), b = sin(theta);
            double x0 = a*rho, y0 = b*rho;
            pt1.x = cvRound(x0 + 1000*(-b));
            pt1.y = cvRound(y0 + 1000*(a));
            pt2.x = cvRound(x0 - 1000*(-b));
            pt2.y = cvRound(y0 - 1000*(a));
            line( cdst, pt1, pt2, cv::Scalar(0,0,255), 3, cv::LINE_AA);
        }
        imshow("Detected Lines (in red) - Standard Hough Line Transform", cdst);
        cv::waitKey();
        return cdst;
    } else {
        /* Probabilistic Line Transform */
        /* results of the detection */

        // Line l({10, 10, 10, 200});
        // l.draw(cdstP, 255,0,0);
        // std::cout << std::endl << "Line:" << std::endl;
        // l.print();

        std::vector<cv::Vec4i> linesPCoords, linesPHV;

        HoughLinesP(dst, linesPCoords, 2, CV_PI/180, 100, 70, 30); // runs the actual detection

        // HoughLinesP(dst, linesPCoords, 0.1, CV_PI/1800, 1, 130, 24);

        Lines lines(linesPCoords);
        // lines.print();

        Lines filteredLines = lines.GetHVlinesSimple(5);
        // filteredLines.findPairs();
        // filteredLines.print();
        // std::cout << "m_pararrel_pairs.size = " << lines.m_pararrel_pairs.size() << std::endl;
        // std::vector<Square> filteredSquares = filteredLines.getSquares();
        // std::cout << "filteredSquares.size = " << filteredSquares.size() << std::endl;

        // std::vector<std::vector<cv::Point, cv::Point>> contours;
        // findContours(dst, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

        /* Draw the lines */
        for(auto line : lines.m_linesvec)
        {
            line.draw(cdstP, 0, 0, 255);
        }
        for(auto line : filteredLines.m_linesvec)
        {
            // line.print();
            line.draw(cdstP, 255, 0, 0);
        }
        // for(auto square : filteredSquares)
        // {
        //     square.print();
        //     unsigned char R,G,B;
        //     std::tie(R,G,B) = getColors();
        //     square.draw(cdstP, R, G, B);
        // }
        int frame_width = cdstP.cols;
	    int frame_height = cdstP.rows;
        float scale = 1080. / frame_height;
        cv::resize(cdstP, cdstP, cv::Size(static_cast<std::size_t>(frame_width * scale), static_cast<std::size_t>(frame_height * scale)));
        
        cv::imshow("Detected Lines (in red) - Probabilistic Line Transform", cdstP);

        cv::waitKey();
        return cdstP;
    }
}
} // namespace twm::hough


#endif //TWM_HOUGH_H
