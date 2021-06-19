#include <Hough/Hough.h>
#include <Hough/Squares.h>
#include <iostream>
#include <Colors/ClassifyHOG.h>
#include <limits>
#include <Carcassonne/Game/Board.h>
#include <cmath>

#include <Colors/Colors.h>
#include <Utils/rgb2hsv.h>
#include <Utils/display.h>

using namespace twm;


// cv::rectangle(img, cv::Point(NW.x, NW.y), cv::Point(SE.x, SE.y), cv::Scalar(B,G,R), 1);

int main() {
    srand(time(NULL));
    cv::Mat img_first = cv::imread( cv::samples::findFile( "../../../../images/game2/0.jpg" ), cv::IMREAD_COLOR );
    // Inicjalizacja petli
    unsigned int frame_width = img_first.cols;
	unsigned int frame_height = img_first.rows;
    cv::Mat mask(frame_height, frame_width, CV_8UC3, cv::Scalar(0, 0, 0));
    cv::Mat image_orig_masked_old;
    using carcassonne::TilePlacement;
    mb::vector2d<TilePlacement> m_board;
    unsigned int desired_size = std::max(frame_width, frame_height);
    int base_x = 70, base_y = 70;

    for( size_t img_id = 0; img_id < 72; img_id++) {
        // std::cout << name << '\n' ;
        std::string path = "../../../../images/game2/";
        std::string full_path = path + std::to_string(img_id) + std::string(".jpg");
        cv::Mat img_orig = cv::imread( cv::samples::findFile( full_path.c_str() ), cv::IMREAD_COLOR );
        cv::Mat img_orig_masked = copyOneImage(img_orig);
        for (size_t x = 0; x < img_orig.rows; x++) {
            for (size_t y = 0; y < img_orig.cols; y++) {
                cv::Vec3b pixel = img_orig_masked.at<cv::Vec3b>(x,y);
                cv::Vec3b mask_pixel = mask.at<cv::Vec3b>(x,y);
                if (mask_pixel[0] != 0 || mask_pixel[1] != 0 || mask_pixel[2] != 0) {
                    pixel[0] = 0; // H
                    pixel[1] = 0;  // S
                    pixel[2] = 0; // V
                    img_orig_masked.at<cv::Vec3b>(x,y) = pixel;
                }
            }
        }
        display("img_orig_masked", img_orig_masked);    cv::destroyWindow("img_orig_masked");
        image_orig_masked_old = copyOneImage(img_orig_masked);

        // detekcja prostokatow
        cv::Mat img_blured;
        // cv::GaussianBlur(image_orig_masked_old, img_blured, cv::Size(13, 13), 0);
        // display("findSquares", img_blured, true);
        // display("findSquares", image_orig_masked_old);
        // display("findSquares", image_orig_masked_old, false);
        // twm::hough::findSquares(img_blured, foundSquares);
        auto foundSquares = twm::hough::findSquares(image_orig_masked_old, desired_size);
        std::cout << "foundSquares.size() = " << foundSquares.size() << std::endl;
        std::vector<Square> squares;
        cv::Mat img_orig_polylines = copyOneImage(img_orig);
        Square s;
        for (auto square : foundSquares) { // wiadomo ze bedzie tylko 1 square
            // std::cout << square[0] << ", " << square[1] << ", " << square[2] << ", " << square[3] << ", " << std::endl;
            s = Square(square);
            squares.push_back(s);
            // s.print();
            s.draw(img_orig_polylines, 255, 0, 0);
        }
        
        // polylines(img_orig_polylines, foundSquares, true, cv::Scalar(0, 255, 0), 3, cv::LINE_AA);
        std::cout << "squares.size() = " << squares.size() << std::endl;

        // wycinanie wykrytego prostokata
        std::vector<cv::Mat> squareImages = twm::hough::getSquareImages(img_orig, squares);
        std::cout << "squareImages.size() = " << squareImages.size() << std::endl;

        // klasyfikacja
        twm::colors::Classifier classifier;
        cv::Mat detected_square;
        for (const auto &im : squareImages) { // wiadomo ze bedzie tylko 1 square
            detected_square = im;
        }

        int detected_tile_id;
        std::string_view detected_tile_path;
        int detected_tile_rotation;
        std::tie(detected_tile_id, detected_tile_path, detected_tile_rotation) = classifier.classifyHog(detected_square);
        std::cout << "Znaleziony obrazek: " << detected_tile_path << " " << detected_tile_id << " " << detected_tile_rotation <<  std::endl;
        // std::cout << " " << detected_tile_rotation <<  std::endl;

        // szczytywanie rozmiarow znalezionego kafelka oraz polozenia
        int x, y, maxX, minX, maxY, minY;
        desired_size = (detected_square.cols + detected_square.rows) / 2;
        std::tie(maxX, minX, maxY, minY) = twm::hough::square_to_min_max_x_y(s);
        if (img_id == 0) {
            base_x = minX;
            base_y = minY;
            x = 70;
            y = 70;
        } else {
            x = 70 + round((minX - base_x) / static_cast<double>(desired_size));
            y = 70 + round((minY - base_y) / static_cast<double>(desired_size));
        }

        // dodaj kafelek do wirtualnej planszy
        std::uint8_t t, rotation;
        t = static_cast<std::uint8_t>(detected_tile_id);
        rotation = static_cast<std::uint8_t>(detected_tile_rotation);
        const char* wndname = "Square Detection Demo";
        display(wndname, img_orig_polylines);     cv::destroyWindow(wndname);
        m_board.set(x, y, TilePlacement{.type = t, .rotation = rotation});

        // tworzenie maski z poprzednich kafelkow
        // display("MASK INPUT", img_orig);
        cv::Mat mask1 = utils::color_tresholder(img_orig);
        mask = copyOneImage(mask1);
        // cv::imshow("MASK OUTPUT", mask);
        // cv::waitKey();
    }
   
    return 0;
}
