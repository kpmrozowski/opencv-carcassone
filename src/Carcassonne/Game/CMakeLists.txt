project(msi_carcassonne_game)
add_library(msi_carcassonne_game Board.cpp)
target_link_libraries(msi_carcassonne_game libmb fmt)
