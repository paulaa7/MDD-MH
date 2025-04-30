#pragma once
#include <vector>
/**
 * Typedef for the fitness function return type.
 *
 * This is an example, change it for your specific problem.
 *
 */
typedef double tFitness;

/**
 * Domain of each element
 */
typedef unsigned tDomain;

/**
 * Represent a new option to create the solution for the heuristic.
 */
typedef unsigned tOption;

/**
 * Represent the new option to select the heuristic
 */
typedef tFitness tHeuristic;

/**
 * Solution class to represent a solution to the given problem.
 *
 * This is an example, change it for your specific problem.
 */
typedef std::vector<tDomain> tSolution;
