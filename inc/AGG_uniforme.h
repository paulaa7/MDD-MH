#pragma once
#include <mh.h>
#include <MDD.h>

using namespace std;

/**
 *
 * @see MH
 * @see Problem
 */
class AGG_uniforme : public MH {

private:
  size_t m, n;
  
public:
  AGG_uniforme() : MH() {}
  virtual ~AGG_uniforme() {}

  void cruce_uniforme(const tSolution& padre1, const tSolution& padre2, vector<tSolution> &poblacion_nueva);
  void reparar(const int num_nodos, vector<bool> &hijo);
  void mutar(tSolution &cromosoma);


  /**
   * @param problem The problem to be optimized
   * @param maxevals Maximum number of evaluations allowed
   * @return A pair containing the best solution found and its fitness
   */
  ResultMH optimize(Problem *problem, int maxevals);
};