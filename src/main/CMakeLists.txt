project(twm)
add_executable(twm main.cpp)
target_link_libraries( twm ${OpenCV_LIBS} stdc++fs fmt twm_colors twm_homography msi_carcassonne_game)

project(twm2)
add_executable(twm2 main2.cpp)
target_link_libraries( twm2 ${OpenCV_LIBS} stdc++fs fmt twm_colors twm_homography msi_carcassonne_game)



