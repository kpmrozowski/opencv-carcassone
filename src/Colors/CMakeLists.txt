project(twm_colors)
add_library(twm_colors Colors.cpp)
target_link_libraries( twm_colors ${OpenCV_LIBS} stdc++fs fmt)
