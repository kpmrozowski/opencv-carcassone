#include <Hough/Hough.h>
#include <Hough/Squares.h>
#include <iostream>
#include <Colors/ClassifyHOG.h>
#include <limits>
#include <Carcassonne/Game/Board.h>
#include <limits>

#include <Colors/Colors.h>
#include <Utils/rgb2hsv.h>
#include <Utils/display.h>


#include <cstdint>
#include <vector>
using namespace twm;

cv::Mat cluster_image(cv::Mat const& img1)
{
    cv::Mat img;
    int K = 5;
    int frame_width = img1.cols;
	int frame_height = img1.rows;
    float scale = 270. / frame_height;
    cv::resize(img1, img, cv::Size(static_cast<std::size_t>(frame_width * scale), static_cast<std::size_t>(frame_height * scale)));

    int n = img.rows * img.cols;
    cv::Mat data = img.reshape(1, n);
    data.convertTo(data, CV_32F);

    std::vector<int> labels;
    cv::Mat1f colors;
    cv::kmeans(data, K, labels
        , cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 100000, 0.00001)
        , 5, cv::KMEANS_PP_CENTERS, colors);

    for (int i = 0; i < n; ++i) {
        data.at<float>(i, 0) = colors(labels[i], 0);
        data.at<float>(i, 1) = colors(labels[i], 1);
        data.at<float>(i, 2) = colors(labels[i], 2);
    }

    cv::Mat reduced = data.reshape(3, img.rows);
    reduced.convertTo(reduced, CV_8U);

    return reduced;
}

cv::Mat filter_mask(cv::Mat& img, cv::Mat& mask)
{
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
    cv::Mat filtered;
    cv::erode(mask, filtered, kernel, cv::Point(-1, -1), 2);
    cv::dilate(filtered, filtered, kernel, cv::Point(-1, -1), 2);

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;

    cv::findContours(filtered, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    cv::Mat output_mask = cv::Mat::zeros(mask.size(), CV_8UC1);

    double const MIN_CONTOUR_AREA(5000.0);
    for (int i(0); i < contours.size(); ++i) {
        double area = cv::contourArea(contours[i]);

        if (area >= MIN_CONTOUR_AREA) {
            cv::drawContours(output_mask, contours, i, cv::Scalar(255), cv::FILLED);
        }
    }

    cv::dilate(output_mask, output_mask, kernel, cv::Point(-1, -1), 1);

    return output_mask;
}

void highlight_mask(std::string const& filename, cv::Mat& img, cv::Mat& mask)
{
    cv::Mat output = img.clone();

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(mask, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    cv::drawContours(output, contours, 0, cv::Scalar(255, 0, 0), 2);

    cv::imwrite(filename, output);
}


int main(int argc, char *argv[])
{
    std::string path = "../../../../images/game2/";
    std::string full_path = path + std::to_string(72) + std::string(".jpg");
    cv::Mat orig_image = cv::imread( cv::samples::findFile( full_path.c_str() ), cv::IMREAD_COLOR );

    if (orig_image.empty()) {
        std::cerr << "Input image empty." << std::endl;
        return -1;
    }

    cv::Mat im_in;
    cv::medianBlur(orig_image, im_in, 3);

    cv::Mat clustered = cluster_image(im_in);
    display("peppers_clustered.png", clustered);

    // cv::Mat hsv_in;
    // cv::cvtColor(clustered, hsv_in, cv::COLOR_BGR2HSV);

    // uint32_t HUE(0), SAT(1), VAL(2);
    // std::vector<cv::Mat> h_s_v(3);
    // cv::split(hsv_in, h_s_v);
    // cv::imwrite("peppers_hue.png", h_s_v[HUE]);
    // cv::imwrite("peppers_sat.png", h_s_v[SAT]);
    // cv::imwrite("peppers_val.png", h_s_v[VAL]);



    // cv::Mat red_mask_a;
    // cv::inRange(h_s_v[HUE], cv::Scalar(2), cv::Scalar(10), red_mask_a);
    // cv::Mat red_mask_b = filter_mask(im_in, red_mask_a);
    // cv::imwrite("peppers_red_mask_a.png", red_mask_a);
    // cv::imwrite("peppers_red_mask_b.png", red_mask_b);

    // cv::Mat yellow_mask_a;
    // cv::inRange(h_s_v[HUE], cv::Scalar(15), cv::Scalar(25), yellow_mask_a);
    // cv::Mat yellow_mask_b = filter_mask(im_in, yellow_mask_a);
    // cv::imwrite("peppers_yellow_mask_a.png", yellow_mask_a);
    // cv::imwrite("peppers_yellow_mask_b.png", yellow_mask_b);

    // cv::Mat green_mask_a;
    // cv::inRange(h_s_v[HUE], cv::Scalar(40), cv::Scalar(50), green_mask_a);
    // cv::Mat green_mask_b = filter_mask(im_in, green_mask_a);
    // cv::imwrite("peppers_green_mask_a.png", green_mask_a);
    // cv::imwrite("peppers_green_mask_b.png", green_mask_b);

    // highlight_mask("peppers_red_out.png", orig_image, red_mask_b);
    // highlight_mask("peppers_yellow_out.png", orig_image, yellow_mask_b);
    // highlight_mask("peppers_green_out.png", orig_image, green_mask_b);
}