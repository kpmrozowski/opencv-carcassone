#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

//  Resize img
// void resize(InputArray src,
// 	OutputArray dst,
// 	Size dsize,
// 	double fx = 0.1 * 1710,
// 	double fy = 0.1 * 2280,
// 	int interpolation = INTER_LINEAR
// );

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    // Declare the output variables
    Mat src, dst, cdst, cdstP;
    float scale = 0.45;
    const char* default_file = "test_14.jpg";
    const char* filename = argc >=2 ? argv[1] : default_file;
    // Loads an image
    Mat src1 = imread( samples::findFile( filename ), IMREAD_GRAYSCALE );
    int frame_width = src1.cols;
	int frame_height = src1.rows;
    resize(src1, src, Size(static_cast<std::size_t>(frame_width * scale), static_cast<std::size_t>(frame_height * scale)));
    imshow("Source", src1);
    // Check if image is loaded fine
    if(src.empty()) {
        printf(" Error opening image\n");
        printf(" Program Arguments: [image_name -- default %s] \n", default_file);
        return -1;
    }
    // Edge detection
    Canny(src, dst, 50, 200, 3);
    // Copy edges to the images that will display the results in BGR
    cvtColor(dst, cdst, COLOR_GRAY2BGR);
    cdstP = cdst.clone();
    // Standard Hough Line Transform
    vector<Vec2f> lines; // will hold the results of the detection
    HoughLines(dst, lines, 1, CV_PI/180, 150, 0, 0 ); // runs the actual detection
    // Draw the lines
    for( size_t i = 0; i < lines.size(); i++ )
    {
        float rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
        line( cdst, pt1, pt2, Scalar(0,0,255), 3, LINE_AA);
    }
    // Probabilistic Line Transform
    vector<Vec4i> linesP; // will hold the results of the detection
    HoughLinesP(dst, linesP, 1, CV_PI/180, 50, 50, 10 ); // runs the actual detection
    // Draw the lines
    for( size_t i = 0; i < linesP.size(); i++ )
    {
        Vec4i l = linesP[i];
        line( cdstP, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, LINE_AA);
    }
    // Show results
    imshow("Source", src);
    imshow("Detected Lines (in red) - Standard Hough Line Transform", cdst);
    imshow("Detected Lines (in red) - Probabilistic Line Transform", cdstP);
    // Wait and Exit
    waitKey();
    return 0;
}