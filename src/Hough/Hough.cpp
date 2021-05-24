#include <Hough/Hough.h>
#include <iostream>
#include <fmt/core.h>

int main() {
    srand(time(NULL));
    if (false) {
        cv::Mat img = twm::hough::detect_liness("./sequential/WIN_20210512_15_23_02_Pro.jpg");
    } else {
        for( const auto& name : twm::hough::get_filenames( "sequential" ) ) {
            std::cout << name << '\n' ;
            const char * c = name.c_str();
            cv::Mat img = twm::hough::detect_liness(c);

            /* Saving images, to the resources folder */
            if (false) {
                std::string saving_path("./output_images/");
                saving_path.append(name);
                std::cout << saving_path << std::endl;
                cv::imwrite(saving_path, img);
            }
            // break;
        }
    }
    return 0;
}
