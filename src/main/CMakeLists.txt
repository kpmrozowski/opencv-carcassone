project(twm)
add_executable(main main.cpp)
target_link_libraries( main ${OpenCV_LIBS} stdc++fs fmt twm_colors twm_homography msi_carcassonne_game)

add_executable(main2 main2.cpp)
target_link_libraries( main2 ${OpenCV_LIBS} stdc++fs fmt twm_colors twm_homography msi_carcassonne_game)

add_executable(watershed watershed.cpp)
target_link_libraries( watershed ${OpenCV_LIBS} stdc++fs fmt twm_colors twm_homography msi_carcassonne_game)

add_executable(kmeans kmeans.cpp)
target_link_libraries( kmeans ${OpenCV_LIBS} stdc++fs fmt twm_colors twm_homography msi_carcassonne_game)



