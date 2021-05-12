#include <Hough/Hough.h>

int main() {
    for( const auto& name : twm::hough::get_filenames( "resources" ) ) {
        std::cout << name << '\n' ;
        const char * c = name.c_str();
        twm::hough::detect_lines(c);
    }
    return 0;
}