project(twm_hough)
add_library(twm_hough Hough.cpp)
target_link_libraries( twm_hough ${OpenCV_LIBS} stdc++fs fmt)
