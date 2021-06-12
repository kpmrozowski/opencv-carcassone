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

cv::Mat copyOneImage(const cv::Mat &origImage) {
    cv::Mat image;
    origImage.copyTo(image);
    return image;
}

void display(std::string wndName, const cv::Mat& img) {
    int frame_width = img.cols;
	int frame_height = img.rows;
    // float scale = 1080. / frame_height;
    float scale = 2000. / frame_height;
    cv::Mat img_copy = copyOneImage(img);
    cv::resize(img_copy, img_copy, cv::Size(static_cast<std::size_t>(frame_width * scale), static_cast<std::size_t>(frame_height * scale)));
    imshow(wndName, img_copy);
    cv::waitKey();
}


// cv::rectangle(img, cv::Point(NW.x, NW.y), cv::Point(SE.x, SE.y), cv::Scalar(B,G,R), 1);

int main() {
    srand(time(NULL));
    int thresh = 50, N = 11;
    cv::Mat img_first = cv::imread( cv::samples::findFile( "../../../../images/game1/1.jpg" ), cv::IMREAD_COLOR );
    // Inicjalizacja petli
    int frame_width = img_first.cols;
	int frame_height = img_first.rows;
    cv::Mat mask(frame_height, frame_width, CV_8UC3, cv::Scalar(0, 0, 0));
    cv::Mat image_orig_masked_old;
    using carcassonne::TilePlacement;
    mb::vector2d<TilePlacement> m_board;

    for( size_t img_id = 0; img_id < 71; img_id++) {
        // std::cout << name << '\n' ;
        std::string path = "../../../../images/game1/";
        std::string full_path = path + std::to_string(img_id) + std::string(".jpg");
        cv::Mat img_orig = cv::imread( cv::samples::findFile( full_path.c_str() ), cv::IMREAD_COLOR );
        cv::Mat img_orig_masked = copyOneImage(img_orig);
        for (size_t x = 0; x < img_orig.rows; x++) {
            for (size_t y = 0; y < img_orig.cols; y++) {
                cv::Vec3b pixel = img_orig_masked.at<cv::Vec3b>(x,y);
                cv::Vec3b mask_pixel = mask.at<cv::Vec3b>(x,y);
                if (mask_pixel[0] != 0 || mask_pixel[1] != 0 || mask_pixel[2] != 0) {
                    pixel[2] = 194; // V
                    pixel[1] = 194;  // S
                    pixel[0] = 194; // H
                    img_orig_masked.at<cv::Vec3b>(x,y) = pixel;
                }
            }
        }
        display("img_orig_masked", img_orig_masked);
        image_orig_masked_old = copyOneImage(img_orig_masked);

        // detekcja prostokatow
        cv::Mat img_blured;
        cv::GaussianBlur(image_orig_masked_old, img_blured, cv::Size(5, 5), 0);
        std::vector<std::vector<cv::Point>> foundSquares;
        twm::hough::findSquares(img_blured, foundSquares);
        std::vector<Square> squares;
        cv::Mat img_orig_polylines = copyOneImage(img_orig);
        for (auto square : foundSquares) { // wiadomo ze bedzie tylko 1 square
            std::cout << square[0] << ", " << square[1] << ", " << square[2] << ", " << square[3] << ", " << std::endl;
            Square s = Square(square);
            squares.push_back(s);
            s.print();
            // s.draw(img_orig_polylines, 255, 255, 0);
        }
        polylines(img_orig_polylines, foundSquares, true, cv::Scalar(0, 255, 0), 3, cv::LINE_AA);
        std::cout << "squares.size() = " << squares.size() << std::endl;
        const char* wndname = "Square Detection Demo";
        display(wndname, img_orig_polylines);

        // wycinanie wykrytego prostokata
        std::vector<cv::Mat> squareImages = twm::hough::getSquareImages(img_orig, squares);
        std::cout << "squareImages.size() = " << squareImages.size() << std::endl;

        // klasyfikacja
        twm::colors::Classifier classifier;
        cv::Mat detected_square;
        for (const auto &im : squareImages) { // wiadomo ze bedzie tylko 1 square
            detected_square = im;
        }
        std::pair<std::string, int> detected_tile_info = classifier.classifyHog(detected_square);
        std::cout << "Znaleziony obrazek: " << detected_tile_info.first << " " << detected_tile_info.second <<  std::endl;
        
        // dodaj kafelek do wirtualnej planszy
        std::uint8_t t = 0, rotation = 0;
        m_board.set(70, 70, TilePlacement{.type = t, .rotation = rotation});

        // display("MASK INPUT", img_orig);
        cv::Mat mask1 = utils::color_tresholder(image_orig_masked_old);
        mask = copyOneImage(mask1);
        cv::imshow("MASK OUTPUT", mask);
        cv::waitKey();
    }
   
    return 0;
}
