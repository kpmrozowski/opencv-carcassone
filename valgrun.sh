# Program has to be compiled !

valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=valgrind-out.txt \
         ./out/build_terminal/src/Hough/twm_hough
