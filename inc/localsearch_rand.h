#pragma once
#include <mh.h>

using namespace std;

/**
 *
 * @see MH
 * @see Problem
 */
class LocalSearchRandom : public MH {

public:
  LocalSearchRandom() : MH() {}
  virtual ~LocalSearchRandom() {}

  /**
   * @param problem The problem to be optimized
   * @param maxevals Maximum number of evaluations allowed
   * @return A pair containing the best solution found and its fitness
   */
  ResultMH optimize(Problem *problem, int maxevals);
};