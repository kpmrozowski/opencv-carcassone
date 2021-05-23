#ifndef TWM_HOUGH_H
#define TWM_HOUGH_H

#include "Lines.h"

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
    if (false)
        Canny(src, dst, 50, 100, 3, true);
    else 
        Canny(src, dst, 8000, 8000, 7, true);
    
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
        lines.findPairs();
        std::cout << "m_pararrel_pairs.size = " << lines.m_pararrel_pairs.size() << std::endl;
        std::vector<Square> filteredSquares = lines.getSquares();
        std::cout << "filteredSquares.size = " << filteredSquares.size() << std::endl;
        // filteredLines.print();

        /* Draw the lines */
        for(auto line : lines.m_linesvec)
        {
            line.draw(cdstP, 0, 0, 255);
        }
        // for(auto line : filteredLines.m_linesvec)
        // {
        //     line.print();
        //     line.draw(cdstP, 255, 0, 0);
        // }
        for(auto square : filteredSquares)
        {
            square.print();
            square.draw(cdstP, 255, 255, 0);
        }
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
