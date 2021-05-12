#ifndef TWM_HOMOGRAPHY_H
#define TWM_HOMOGRAPHY_H
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/highgui.hpp>

namespace twm::homography
{
enum Pattern { CHESSBOARD, CIRCLES_GRID, ASYMMETRIC_CIRCLES_GRID };

cv::Scalar randomColor( cv::RNG& rng )
{
  int icolor = (unsigned int) rng;
  return cv::Scalar( icolor & 255, (icolor >> 8) & 255, (icolor >> 16) & 255 );
}

void perspectiveCorrection(const std::string &img1Path, const std::string &img2Path, const cv::Size &patternSize, cv::RNG &rng)
{
    cv::Mat img1 = cv::imread( cv::samples::findFile(img1Path) );
    cv::Mat img2 = cv::imread( cv::samples::findFile(img2Path) );

    //! [find-corners]
    std::vector<cv::Point2f> corners1, corners2;
    bool found1 = findChessboardCorners(img1, patternSize, corners1);
    bool found2 = findChessboardCorners(img2, patternSize, corners2);
    //! [find-corners]

    if (!found1 || !found2)
    {
        std::cout << "Error, cannot find the chessboard corners in both images." << std::endl;
        return;
    }

    //! [estimate-homography]
    cv::Mat H = findHomography(corners1, corners2);
    std::cout << "H:\n" << H << std::endl;
    //! [estimate-homography]

    //! [warp-chessboard]
    cv::Mat img1_warp;
    cv::warpPerspective(img1, img1_warp, H, img1.size());
    //! [warp-chessboard]

    cv::Mat img_draw_warp;
    cv::hconcat(img2, img1_warp, img_draw_warp);
    cv::imshow("Desired chessboard view / Warped source chessboard view", img_draw_warp);

    //! [compute-transformed-corners]
    cv::Mat img_draw_matches;
    cv::hconcat(img1, img2, img_draw_matches);
    for (size_t i = 0; i < corners1.size(); i++)
    {
        cv::Mat pt1 = (cv::Mat_<double>(3,1) << corners1[i].x, corners1[i].y, 1);
        cv::Mat pt2 = H * pt1;
        pt2 /= pt2.at<double>(2);

        cv::Point end( (int) (img1.cols + pt2.at<double>(0)), (int) pt2.at<double>(1) );
        line(img_draw_matches, corners1[i], end, randomColor(rng), 2);
    }

    cv::imshow("Draw matches", img_draw_matches);
    cv::waitKey();
    //! [compute-transformed-corners]
}

static const char* params
    = "{ help h         |       | print usage }"
      "{ image1         | left02.jpg | path to the source chessboard image }"
      "{ image2         | left01.jpg | path to the desired chessboard image }"
      "{ width bw       | 9     | chessboard width }"
      "{ height bh      | 6     | chessboard height }";
}


#endif //TWM_HOMOGRAPHY_H