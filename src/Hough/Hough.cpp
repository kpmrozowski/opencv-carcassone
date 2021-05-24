#include <Hough/Hough.h>
#include <Hough/Squares.h>
#include <iostream>
#include <fmt/core.h>

int main() {
    srand(time(NULL));

    int thresh = 50, N = 11;
    const char* wndname = "Square Detection Demo";
    if (true) {
        // cv::Mat img1 = twm::hough::detect_liness("./sequential/WIN_20210512_15_23_02_Pro.jpg");
        cv::Mat img = imread( cv::samples::findFile( "./sequential/WIN_20210512_15_23_02_Pro.jpg" ), cv::IMREAD_GRAYSCALE );
        std::vector<std::vector<cv::Point>> foundSquares;
        twm::hough::findSquares(img, foundSquares);
        std::vector<Square> squares;
        for (auto square : foundSquares) {
            std::cout << square[0] << ", " << square[1] << ", " << square[2] << ", " << square[3] << ", " << std::endl;
            Square s = Square(square);
            squares.push_back(s);
            s.print();
            s.draw(img, 255, 255, 0);
        }
        polylines(img, foundSquares, true, cv::Scalar(0, 255, 0), 3, cv::LINE_AA);
        std::cout << "squares.size() = " << squares.size();
        imshow(wndname, img);
        cv::waitKey();
    } else {
        for( const auto& name : twm::hough::get_filenames( "sequential" ) ) {
            std::cout << name << '\n' ;
            const char * str = name.c_str();
            cv::Mat img = twm::hough::detect_liness(str);

            /* Saving images, to the resources folder */
            if (false) {
                std::string saving_path("./output_images/");
                saving_path.append(name);
                std::cout << saving_path << std::endl;
                cv::imwrite(saving_path, img);
            }
            // break;
            int c = cv::waitKey();
            if( c ==  27 )
                break;
        }
    }
    return 0;
}
