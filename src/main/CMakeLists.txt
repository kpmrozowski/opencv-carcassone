project(twm)


# set(MATPLOTLIB_CPP_INCLUDE_DIRS "/tmp/vcpkg/installed/x64-linux/include/matplotlibcpp.h")
add_executable(main main.cpp)

# find_package(Python2 COMPONENTS Interpreter NumPy)
find_package(Python3 3.7 COMPONENTS Interpreter NumPy REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

target_link_libraries( main ${OpenCV_LIBS} stdc++fs fmt twm_colors twm_homography msi_carcassonne_game ${PYTHON_LIBRARIES} Python3::NumPy twm_HDBSCAN_CPP)
# target_include_directories(main PRIVATE ${MATPLOTLIB_CPP_INCLUDE_DIRS})

add_executable(main2 main2.cpp)
target_link_libraries( main2 ${OpenCV_LIBS} stdc++fs fmt twm_colors twm_homography msi_carcassonne_game ${PYTHON_LIBRARIES} Python3::NumPy twm_HDBSCAN_CPP)

add_executable(watershed watershed.cpp)
target_link_libraries( watershed ${OpenCV_LIBS} stdc++fs fmt twm_colors twm_homography msi_carcassonne_game ${PYTHON_LIBRARIES} Python3::NumPy twm_HDBSCAN_CPP)

add_executable(kmeans kmeans.cpp)
target_link_libraries( kmeans ${OpenCV_LIBS} stdc++fs fmt twm_colors twm_homography msi_carcassonne_game ${PYTHON_LIBRARIES} Python3::NumPy twm_HDBSCAN_CPP)

add_executable(tests tests.cpp)
target_link_libraries( tests ${OpenCV_LIBS} stdc++fs fmt twm_colors twm_homography msi_carcassonne_game ${PYTHON_LIBRARIES} Python3::NumPy twm_HDBSCAN_CPP)
# target_include_directories(tests PRIVATE ${MATPLOTLIB_CPP_INCLUDE_DIRS})



