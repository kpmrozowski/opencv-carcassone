project(twm)
add_executable(twm main.cpp)
target_link_libraries( twm ${OpenCV_LIBS} stdc++fs fmt twm_colors twm_hough twm_homography)
