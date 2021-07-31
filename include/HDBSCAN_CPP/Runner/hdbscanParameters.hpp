#ifndef TWM_HDBSCAN_CPP_RUNNER_hdbscanParameters_H
#define TWM_HDBSCAN_CPP_RUNNER_hdbscanParameters_H
#pragma once
#include <cstdint>
#include <HDBSCAN_CPP/Distance/IDistanceCalculator.hpp>
#include <iostream>
#include <vector>
#include <HDBSCAN_CPP/HdbscanStar/hdbscanConstraint.hpp>

using namespace std;
class hdbscanParameters
{
public:

	/// <summary>
	/// Parameters to be Passed to the HDBSCAN Algorithm
	/// </summary>
	/// <param name="distances">The attributes of the first point</param>
	/// <param name="dataset">The attributes of the second point</param>
	/// <param name="dataset">The attributes of the second point</param>
	/// <param name="distanceFunction">Defines the type of distance measure to use : Euclidean, Manhattan ,..</param>
	/// <param name="minPoints">Min Points in the cluster</param>
	/// <param name="minClusterSize">The minimum number of points which a cluster needs to be a valid cluster</param>
	vector< vector <double> > distances;
	vector< vector <double> > dataset;
	string distanceFunction;
	uint32_t minPoints;
	uint32_t minClusterSize;
	vector<hdbscanConstraint> constraints;
};

#endif // TWM_HDBSCAN_CPP_RUNNER_hdbscanParameters_H
