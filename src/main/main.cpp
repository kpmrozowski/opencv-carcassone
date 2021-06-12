#include <Hough/Hough.h>
#include <Hough/Squares.h>
#include <iostream>
#include <fmt/core.h>
#include <Colors/ClassifyHOG.h>
#include <limits>
#include <Carcassonne/Game/Board.h>

#include <Colors/Colors.h>
#include <Utils/rgb2hsv.h>

using namespace twm;

void display(std::string wndName, cv::Mat& img) {
    int frame_width = img.cols;
	int frame_height = img.rows;
    float scale = 1080. / frame_height;
    cv::resize(img, img, cv::Size(static_cast<std::size_t>(frame_width * scale), static_cast<std::size_t>(frame_height * scale)));
    imshow(wndName, img);
    cv::waitKey();
}
// cv::rectangle(img, cv::Point(NW.x, NW.y), cv::Point(SE.x, SE.y), cv::Scalar(B,G,R), 1);

int main() {
    srand(time(NULL));

    int thresh = 50, N = 11;
    const char* wndname = "Square Detection Demo";
    cv::Mat img_orig = cv::imread( cv::samples::findFile( "../../../../images/game1/20210612_113122.jpg" ), cv::IMREAD_COLOR );
    // cv::GaussianBlur(img_orig, img_orig, cv::Size(9, 9), 0);
    cv::Mat img_orig2 = cv::imread( cv::samples::findFile( "../../../../images/game1/20210612_113205.jpg" ), cv::IMREAD_COLOR );
    // cv::GaussianBlur(img_orig2, img_orig2, cv::Size(9, 9), 0);
    
    auto mask = utils::color_tresholder(img_orig);
    // Show the frames
    // imshow("mask", mask);
    // cv::waitKey();
    for (size_t x = 0; x < img_orig2.rows; x++) {
        for (size_t y = 0; y < img_orig2.cols; y++) {
            cv::Vec3b pixel = img_orig2.at<cv::Vec3b>(x,y);
            cv::Vec3b mask_pixel = mask.at<cv::Vec3b>(x,y);
            // fmt::print("{} ", static_cast<int>(mask_pixel[0]));
            if (mask_pixel[0] != 0 || mask_pixel[1] != 0 || mask_pixel[2] != 0) {
                pixel[2] = 194; // V
                pixel[1] = 194;  // S
                pixel[0] = 194; // H
                img_orig2.at<cv::Vec3b>(x,y) = pixel;
            }
        }
    }
    // display("img_orig2", img_orig2);

    cv::Mat img_diff;
    cv::absdiff(img_orig2, img_orig, img_diff);
    cv::threshold(img_diff, img_diff, 80, 255, cv::THRESH_BINARY);
    cv::erode(img_diff, img_diff, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));



    if (true) {
        // cv::Mat img1 = twm::hough::detect_liness("./sequential/WIN_20210512_15_23_02_Pro.jpg");
        cv::Mat img_orig = cv::imread( cv::samples::findFile( "../../../../images/game1/20210612_113122.jpg" ), cv::IMREAD_COLOR );


        cv::Mat img_hsv, img_new;

        cv::cvtColor(img_orig, img_hsv, cv::COLOR_BGR2HSV);
        unsigned int minH = 0.00 * 179;
        unsigned int maxH = 0.9 * 179;
        unsigned int minS = 0.04 * 255;
        unsigned int maxS = 1 * 255;
        for (size_t x = 0; x < img_orig.rows; x++) {
            for (size_t y = 0; y < img_orig.cols; y++) {
                cv::Vec3b pixel = img_hsv.at<cv::Vec3b>(x,y);
                if (pixel[0] < minH || pixel[0] > maxH || pixel[1] < minS || pixel[1] > maxS) {
                    pixel[2] = 0; // V
                    pixel[1] = 0;  // S
                    pixel[0] = 179; // H
                    img_hsv.at<cv::Vec3b>(x,y) = pixel;
                }
            }
        }
        cv::cvtColor(img_hsv, img_new, cv::COLOR_HSV2BGR);

        int frame_width = img_orig.cols;
	    int frame_height = img_orig.rows;
        float scale = 1080. / frame_height;
        cv::resize(img_new, img_new, cv::Size(static_cast<std::size_t>(frame_width * scale), static_cast<std::size_t>(frame_height * scale)));

        // display("nowy", img_new);
        // cv::waitKey();


        cv::Mat img;


        
        cv::GaussianBlur(img_orig, img, cv::Size(5, 5), 0);
        // display("blured", img);
        // cv::Mat img = img_orig;
        std::vector<std::vector<cv::Point>> foundSquares;
        // twm::hough::findSquares(img, foundSquares);


        twm::hough::findSquares(img_orig2, foundSquares);
        std::vector<Square> squares;
        for (auto square : foundSquares) {
            std::cout << square[0] << ", " << square[1] << ", " << square[2] << ", " << square[3] << ", " << std::endl;
            Square s = Square(square);
            squares.push_back(s);
            s.print();
            s.draw(img, 255, 255, 0);
        }
        polylines(img_orig2, foundSquares, true, cv::Scalar(0, 255, 0), 3, cv::LINE_AA);
        std::cout << "squares.size() = " << squares.size() << std::endl;
        display(wndname, img_orig2);
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

        using carcassonne::TilePlacement;
        mb::vector2d<TilePlacement> m_board;
        std::uint8_t t = 0, rotation = 0;
        m_board.set(70, 70, TilePlacement{.type = t, .rotation = rotation});

        

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
