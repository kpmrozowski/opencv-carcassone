project(twm_HDBSCAN_CPP)
add_library(
    twm_HDBSCAN_CPP
    Distance/EuclideanDistance.cpp
    Distance/IDistanceCalculator.cpp
    Distance/ManhattanDistance.cpp
    Hdbscan/hdbscan.cpp
    HdbscanStar/cluster.cpp
    HdbscanStar/hdbscanAlgorithm.cpp
    HdbscanStar/hdbscanConstraint.cpp
    HdbscanStar/outlierScore.cpp
    HdbscanStar/undirectedGraph.cpp
    Runner/hdbscanParameters.cpp
    Runner/hdbscanResult.cpp
    Runner/hdbscanRunner.cpp
    Utils/bitSet.cpp
)
target_link_libraries( twm_HDBSCAN_CPP)
