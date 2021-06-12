#include <Hough/Hough.h>
#include <Hough/Squares.h>
#include <iostream>
#include <fmt/core.h>
#include <Colors/ClassifyHOG.h>
#include <limits>

#include <Colors/Colors.h>

using namespace twm;

int main() {
    srand(time(NULL));

    int thresh = 50, N = 11;
    const char* wndname = "Square Detection Demo";
    if (true) {
        // cv::Mat img1 = twm::hough::detect_liness("./sequential/WIN_20210512_15_23_02_Pro.jpg");
        cv::Mat img_orig = cv::imread( cv::samples::findFile( "../../../../images/game1/20210612_113122.jpg" ), cv::IMREAD_COLOR );
        cv::Mat img;
        cv::GaussianBlur(img_orig, img, cv::Size(5, 5), 0);
        cv::imshow("blured", img);
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
        cv::waitKey();
        std::vector<cv::Mat> squareImages = twm::hough::getSquareImages(img_orig, squares);
        std::cout << "squareImages.size() = " << squareImages.size() << std::endl;
        twm::colors::Classifier classifier;
        cv::Mat detected_square;

        for (const auto &im : squareImages) {
            // hog_vector = classifier.hog(im);
            detected_square = im; // zrobic lepiej, teraz zaklada ze jest jeden kwadrat
            // cv::imshow("a", im);
            //cv::waitKey();
        }

        std::pair<std::string, int> detected_tile_info = classifier.classifyHog(detected_square);
        std::cout << "Znaleziony obrazek: " << detected_tile_info.first << " " << detected_tile_info.second <<  std::endl;

        // // HOG comparison
        // double min_dist = std::numeric_limits<double>::infinity();
        // std::pair<std::string, int> detected_tile;
        // for( const auto& fname : twm::hough::get_filenames( "../../../../images/tiles" ) ) {
        //         std::cout << "./" << fname << '\n' ;
        //         const char * fname_cstr = fname.c_str();
        //         cv::Mat tile = imread(cv::samples::findFile(fname_cstr), cv::IMREAD_COLOR);
        //     for (int rotation = 0; rotation < 4; ++rotation) {
        //         // rotate clockwise
        //         cv::transpose(tile, tile);
        //         cv::flip(tile, tile, 1);
        //         int tile_detected_width = tile_detected.cols;
        //         int tile_detected_height = tile_detected.rows;
                
        //         cv::Mat tile_scaled;
        //         cv::resize(tile, tile_scaled, cv::Size(static_cast<std::size_t>(tile_detected_width), static_cast<std::size_t>(tile_detected_height)));

        //         hog_vector_detected = classifier.hog(tile_scaled);
        //         double dist = cv::norm(hog_vector, hog_vector_detected);
        //         if (dist < min_dist) {
        //             min_dist = dist;
        //             detected_tile = make_pair(fname, rotation);
        //         }

        //         std::cout << "dist: " << dist << std::endl;
        //     }
        // }














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
