#include <Colors/Colors.h>
#include <iostream>
#include <fmt/core.h>
#include <cmath>

namespace twm::colors {

RegionsOfTiles::RegionsOfTiles() {}

RegionsOfTiles::RegionsOfTiles(const std::vector<cv::Mat> & all_squared_imgs, int rows, int cols)
     : m_squared_imgs(all_squared_imgs)
     , m_rows(rows)
     , m_cols(cols) {}
std::vector<cv::Mat> RegionsOfTiles::splitTile(const cv::Mat &tile) noexcept {
   std::vector<cv::Mat> tile_splited;
   for (int r = 0; r < tile.rows; r += m_rows)
       for (int c = 0; c < tile.cols; c += m_cols)
       {
           cv::Mat region = tile(cv::Range(r, std::min(r + m_rows, tile.rows)), cv::Range(c, std::min(c + m_cols, tile.cols)));//no data  copying    here
           //cv::Mat tileCopy = img(cv::Range(r, min(r + N, img.rows)),
                        //cv::Range(c, min(c + N, img.cols))).clone();//with  data copying

           //tile can be smaller than NxN if image size is not a factor of N
           tile_splited.push_back(region);
       }
    return tile_splited;
}

void RegionsOfTiles::storeSplitedTiles () noexcept {
    for (const auto &squareImg : m_squared_imgs) {
        m_squared_imgs_regions.push_back(splitTile(squareImg));
    }
}
}