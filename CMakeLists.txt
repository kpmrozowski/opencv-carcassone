set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
project( TWM )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

include_directories(include)

add_subdirectory(src/Hough)
add_subdirectory(src/Homography)