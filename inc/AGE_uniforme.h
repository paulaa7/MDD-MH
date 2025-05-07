#pragma once
#include <OperadoresComunes.h>

using namespace std;

/**
 *
 * @see MH
 * @see Problem
 */
class AGE_uniforme : public OperadoresComunes {

  private:
    size_t m, n;
    
  public:
    AGE_uniforme() : OperadoresComunes() {}
    virtual ~AGE_uniforme() {}  
  
    /**
     * @param problem The problem to be optimized
     * @param maxevals Maximum number of evaluations allowed
     * @return A pair containing the best solution found and its fitness
     */
    ResultMH optimize(Problem *problem, int maxevals);
  };