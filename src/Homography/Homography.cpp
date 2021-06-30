#include <Homography/Homography.h>

// int main(int argc, char *argv[])
// {executable
//     cv::RNG rng( 0xFFFFFFFF );
//     cv::CommandLineParser parser(argc, argv, twm::homography::params);

//     if (parser.has("help"))
//     {
//         parser.about("Code for homography tutorial.\n"
//             "Example 2: perspective correction.\n");
//         parser.printMessage();
//         return 0;
//     }

//     cv::Size patternSize(parser.get<int>("width"), parser.get<int>("height"));
//     twm::homography::perspectiveCorrection(parser.get<cv::String>("image1"),
//                           parser.get<cv::String>("image2"),
//                           patternSize, rng);

//     return 0;
// }