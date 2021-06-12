#include <Hough/Hough.h>
#include <Hough/Squares.h>
#include <iostream>
#include <fmt/core.h>
#include <Colors/ClassifyHOG.h>
#include <Carcassonne/Game/Board.h>

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
        cv::Mat agx, agy;
        std::vector<float> hog_vector, hog_vector_detected;
        cv::Mat tile_detected;
        for (const auto &im : squareImages) {
            std::tie(agx, agy) = classifier.hog_sobel(im);
            hog_vector = classifier.hog(im);
            tile_detected = im; // zrobic lepiej
            cv::imshow("a", im);
            std::pair<unsigned char, unsigned char> meanHS = colors::getMeanHS(im);
            std::cout << "meanH: " << int(meanHS.first) << " meanS: " << int(meanHS.second) << std::endl;
            cv::waitKey();
        }

        // HS comparison
        // for( const auto& name : twm::hough::get_filenames( "tiles" ) ) {
        //     std::cout << "./" << name << '\n' ;
        //     const char * str = name.c_str();
        //     cv::Mat tile = imread(cv::samples::findFile(str), cv::IMREAD_COLOR);
        //     std::pair<unsigned char, unsigned char> meanHS = colors::getMeanHS(tile);
        //     std::cout << "Tile: meanH: " << int(meanHS.first) << " meanS: " << int(meanHS.second) << std::endl;
        // }

        // HOG comparison

        cv::Mat bgx, bgy;
        std::vector<double> distances;
        for( const auto& name : twm::hough::get_filenames( "../../../../images/tiles" ) ) {
                std::cout << "./" << name << '\n' ;
                const char * str = name.c_str();
                cv::Mat tile1 = imread(cv::samples::findFile(str), cv::IMREAD_COLOR);
            for (int rotation = 0; rotation < 4; ++rotation) {
                // rotate clockwise
                cv::transpose(tile1,tile1);
                cv::flip(tile1,tile1,1);
                // cv::imshow("rotation", tile1);
                // cv::waitKey();
                // int tile_width = tile1.cols;
                // int tile_height = tile1.rows;
                int tile_detected_width = tile_detected.cols;
                int tile_detected_height = tile_detected.rows;
                
                // float scale_x = tile_detected_width / tile_width;
                // float scale_y = tile_detected_width / tile_height;
                cv::Mat tile;
                cv::resize(tile1, tile, cv::Size(static_cast<std::size_t>(tile_detected_width), static_cast<std::size_t>(tile_detected_height)));
                
                std::tie(bgx, bgy) = classifier.hog_sobel(tile);
                double dist_x = cv::norm(agx, bgx);
                double dist_y = cv::norm(agy, bgy);

                hog_vector_detected = classifier.hog(tile);
                double dist = cv::norm(hog_vector, hog_vector_detected);
                distances.push_back(dist);

                
                // std::cout << "dist_x: " << dist_x << " dist_y: " << dist_y << std::endl;
                std::cout << "dist: " << dist << std::endl;
            }

        }
        using carcassonne::TilePlacement;
        mb::vector2d<TilePlacement> m_board;
        std::uint8_t t = 0, rotation = 0;
        m_board.set(70, 70, TilePlacement{.type = t, .rotation = rotation});













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
