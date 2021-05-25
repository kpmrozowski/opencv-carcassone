rm ./output_images/resources/*
CXX=/usr/bin/g++-10 CC=/usr/bin/gcc-10 cmake -S . -B out/build_terminal
CXX=/usr/bin/g++-10 CC=/usr/bin/gcc-10 make -C out/build_terminal -j12 && out/build_terminal/src/main/twm
# CXX=/usr/bin/g++-10 CC=/usr/bin/gcc-10 make -C out/build_terminal -j12 && out/build_terminal/src/Hough/twm_homography
