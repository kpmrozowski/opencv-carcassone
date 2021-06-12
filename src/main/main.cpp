#include <Hough/Hough.h>
#include <Hough/Squares.h>
#include <iostream>
#include <fmt/core.h>

#include <Colors/Colors.h>

using namespace twm;

int main() {
    srand(time(NULL));

    int thresh = 50, N = 11;
    const char* wndname = "Square Detection Demo";
    if (true) {
        // cv::Mat img1 = twm::hough::detect_liness("./sequential/WIN_20210512_15_23_02_Pro.jpg");
        cv::Mat img_orig = imread( cv::samples::findFile( "./sequential/WIN_20210512_15_23_02_Pro.jpg" ), cv::IMREAD_COLOR );
        cv::Mat img;
        cv::GaussianBlur(img_orig, img, cv::Size(40, 40), 0);
        // cv::Mat img = img_orig;
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
        polylines(img_orig, foundSquares, true, cv::Scalar(0, 255, 0), 3, cv::LINE_AA);
        std::cout << "squares.size() = " << squares.size() << std::endl;
        imshow(wndname, img_orig);
        std::vector<cv::Mat> squareImages = twm::hough::getSquareImages(img_orig, squares);
        std::cout << "squareImages.size() = " << squareImages.size() << std::endl;
        for (const auto &im : squareImages) {
            cv::imshow("a", im);
            std::pair<unsigned char, unsigned char> meanHS = colors::getMeanHS(im);
            std::cout << "meanH: " << int(meanHS.first) << " meanS: " << int(meanHS.second) << std::endl;
            cv::waitKey();
        }
        for( const auto& name : twm::hough::get_filenames( "tiles" ) ) {
            std::cout << "./" << name << '\n' ;
            const char * str = name.c_str();
            cv::Mat tile = imread(cv::samples::findFile(str), cv::IMREAD_COLOR);
            std::pair<unsigned char, unsigned char> meanHS = colors::getMeanHS(tile);
            std::cout << "Tile: meanH: " << int(meanHS.first) << " meanS: " << int(meanHS.second) << std::endl;
        }
        cv::waitKey();
    } else {
        for( const auto& name : twm::hough::get_filenames("sequential") ) {
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
