#pragma once
#include <AM_comun.h>

using namespace std;

/**
 *
 * @see MH
 * @see Problem
 */
class AM_2 : public AM_comun {

private:
  size_t m, n;
  
public:
  AM_2() : AM_comun() {}
  virtual ~AM_2() {}

  /**
   * @param problem The problem to be optimized
   * @param maxevals Maximum number of evaluations allowed
   * @return A pair containing the best solution found and its fitness
   */
  ResultMH optimize(Problem *problem, int maxevals);
};