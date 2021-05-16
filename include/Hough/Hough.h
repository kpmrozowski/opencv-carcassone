#ifndef TWM_HOUGH_H
#define TWM_HOUGH_H

#include "Utils.h"

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
    if (true)
        Canny(src, dst, 50, 100, 3, true);
    else 
        Canny(src, dst, 16000, 10000, 7, true);
    
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
        std::vector<cv::Vec4i> linesP, linesPHV;

        HoughLinesP(dst, linesP, 0.1, CV_PI/1800, 1, 130, 24); // runs the actual detection
        if (false)
            for (auto line : linesP)
                std::cout << line[0] << " " << line[1] << " " << line[2] << " " << line[3] << std::endl;
        
        std::vector<double> angles = getAngles(linesP);
        std::vector<unsigned short> histogram = getAnglesHistogram(angles, 22).first;

        // std::cout << "Angles: " << std::endl;
        // for (auto a : angles) {
        //     std::cout << a << std::endl;
        // }
        // std::cout << std::endl;

        /* Draw the lines */
        std::pair<std::vector<unsigned short>, std::vector<unsigned short>> pair = getAnglesHistogram(angles, 22);
        linesPHV = GetHVlines(pair, linesP, angles);
        for( std::size_t i = 0; i < linesP.size(); i++ )
        {
            cv::Vec4i l = linesP[i];
            cv::line( cdstP, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0,255,0)  , 3, cv::LINE_AA);
        }
        for( std::size_t i = 0; i < linesPHV.size(); i++ )
        {
            cv::Vec4i lHV = linesPHV[i];
            cv::line( cdstP, cv::Point(lHV[0], lHV[1]), cv::Point(lHV[2], lHV[3]), cv::Scalar(0,0,255)  , 3, cv::LINE_AA);
        }
        
        // for (auto line : linesP) std::cout << line[0] << " " << line[1] << " " << line[2] << " " << line[3] << std::endl;
        cv::imshow("Detected Lines (in red) - Probabilistic Line Transform", cdstP);
        cv::waitKey();
        return cdstP;
    }
}
} // namespace twm::hough


#endif //TWM_HOUGH_H
