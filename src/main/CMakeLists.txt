project(twm)
add_executable(twm main.cpp)
target_link_libraries( twm ${OpenCV_LIBS} stdc++fs fmt twm_colors twm_homography msi_carcassonne_game)

project(twm_old)
add_executable(twm_old main2.cpp)
target_link_libraries( twm_old ${OpenCV_LIBS} stdc++fs fmt twm_colors twm_homography msi_carcassonne_game)



