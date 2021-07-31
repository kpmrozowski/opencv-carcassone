#ifndef TWM_HDBSCAN_CPP_RUNNER_hdbscanParameters_H
#define TWM_HDBSCAN_CPP_RUNNER_hdbscanParameters_H
#pragma once
#include <cstdint>
#include <HDBSCAN_CPP/Distance/IDistanceCalculator.hpp>
#include <iostream>
#include <vector>
#include <HDBSCAN_CPP/HdbscanStar/hdbscanConstraint.hpp>

/** 
 * @brief Parameters to be Passed to the HDBSCAN Algorithm
 * @param distances The attributes of the first point
 * @param dataset The attributes of the second point
 * @param distanceFunction Defines the type of distance measure to use : Euclidean, Manhattan ,..
 * @param minPoints Min Points in the cluster
 * @param minClusterSize The minimum number of points which a cluster needs to be a valid cluster
 */
class hdbscanParameters {
  public:
	std::vector<std::vector<double>> distances;
	std::vector<std::vector<double>> dataset;
	std::string distanceFunction;
	uint32_t minPoints;
	uint32_t minClusterSize;
	std::vector<hdbscanConstraint> constraints;
};

#endif // TWM_HDBSCAN_CPP_RUNNER_hdbscanParameters_H
