#include <Hough/Hough.h>
#include <iostream>

int main() {
    for( const auto& name : twm::hough::get_filenames( "resources" ) ) {
        std::cout << name << '\n' ;
        const char * c = name.c_str();
        cv::Mat img = twm::hough::detect_liness(c);

        /* Saving images, to the resources folder */
        if (true) {
            std::string saving_path("./output_images/");
            saving_path.append(name);
            std::cout << saving_path << std::endl;
            cv::imwrite(saving_path, img);
        }
    }
    return 0;
}