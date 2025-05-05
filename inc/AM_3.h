#pragma once
#include <OperadoresComunes.h>

using namespace std;

/**
 *
 * @see MH
 * @see Problem
 */
class AM_3 : public OperadoresComunes {

private:
  size_t m, n;
  
public:
  AM_3() : OperadoresComunes() {}
  virtual ~AM_3() {}

  /**
   * @param problem The problem to be optimized
   * @param maxevals Maximum number of evaluations allowed
   * @return A pair containing the best solution found and its fitness
   */
  ResultMH optimize(Problem *problem, int maxevals);
};