CXX=/usr/bin/g++-10 CC=/usr/bin/gcc-10 ~/Downloads/cmake/cmake-3.20.4-linux-x86_64/bin/cmake -S . -B out/build_terminal
CXX=/usr/bin/g++-10 CC=/usr/bin/gcc-10 make -C out/build_terminal -j12 && out/build_terminal/src/Hough/twm_hough
# CXX=/usr/bin/g++-10 CC=/usr/bin/gcc-10 make -C out/build_terminal -j12 && out/build_terminal/src/Hough/twm_homography
