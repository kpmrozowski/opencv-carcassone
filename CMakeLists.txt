set(CMAKE_CXX_STANDARD 17)
project( Hough1 )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( Hough1 hough1.cpp )
target_link_libraries( Hough1 ${OpenCV_LIBS} )


# add_compile_options(-fconcepts)

# include(FindPkgConfig)
# pkg_search_module(opencv REQUIRED opencv4)

# include_directories(include)

# set(BUILD_GMOCK OFF)

# add_subdirectory(deps/nvidia-install/opencvbuild/opencv)

# add_subdirectory(src/Test)