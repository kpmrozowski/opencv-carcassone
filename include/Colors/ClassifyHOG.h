#ifndef TWM_CLASSIFYHOG_H
#define TWM_CLASSIFYHOG_H
#include <opencv2/features2d.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect.hpp>

namespace twm::colors {

class Classifier {
public:

   std::vector<float> hog (cv::Mat img) {
       //params
    cv::Size cellSize(4,4);
    int nbins= 11;
    cv::Size blockSize(8,8);

    //my variables
    std::vector<float>hog_vector;
    cv::Mat templ_gray;

    //convert to gray
    cv::cvtColor(img, templ_gray, cv::COLOR_BGR2GRAY);

    //create hog object
    cv::HOGDescriptor hog(cv::Size(templ_gray.cols/cellSize.width*cellSize.width,templ_gray.rows/cellSize.height*cellSize.height),
            cv::Size(blockSize.height*cellSize.height,blockSize.width*cellSize.width),
            cv::Size(cellSize.height,cellSize.width),
            cellSize,
            nbins);
      // gives --> winSize [32 x 16],  blockSize [16 x 16],  blockStride [8 x 8],  cellSize [8 x 8]

      //compute the descriptor of the car
      hog.compute(templ_gray, hog_vector, cv::Size(cellSize.height,cellSize.width), cv::Size( 0, 0 ));

      return hog_vector;
   }

   std::pair<std::string, int> classifyHog(cv::Mat detected_square) {
      std::vector<float> hog_vector, hog_vector_detected;
      double min_dist = std::numeric_limits<double>::infinity();
      std::pair<std::string, int> detected_tile;
      for( const auto& fname : twm::hough::get_filenames( "../../../../images/tiles" ) ) {
            std::cout << "./" << fname << '\n' ;
            const char * fname_cstr = fname.c_str();
            cv::Mat tile = imread(cv::samples::findFile(fname_cstr), cv::IMREAD_COLOR);
            for (int rotation = 0; rotation < 4; ++rotation) {
               int detected_square_width = detected_square.cols;
               int detected_square_height = detected_square.rows;
                
               cv::Mat tile_ref;
               cv::resize(tile, tile_ref, cv::Size(static_cast<std::size_t>(detected_square_width), static_cast<std::size_t>(detected_square_height)));

               hog_vector = this->hog(detected_square);
               hog_vector_detected = this->hog(tile_ref);
               double dist = cv::norm(hog_vector, hog_vector_detected);
               if (dist < min_dist) {
                  min_dist = dist;
                  detected_tile = make_pair(fname, rotation);
               }

               // std::cout << "dist: " << dist << std::endl;
               // rotate clockwise
               cv::transpose(tile, tile);
               cv::flip(tile, tile, 1);
            }
      }
      return detected_tile;
   }
};





} // twm::colors

#endif // TWM_CLASSIFYHOG_H
