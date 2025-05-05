#ifndef __OPERADORESCOMUNES_H
#define __OPERADORESCOMUNES_H

#pragma once
#include <mh.h>

using namespace std;

/**
 *
 * @see MH
 * @see Problem
 */
class OperadoresComunes : public MH {

private:
  size_t m, n;
  
public:
  OperadoresComunes() : MH() {}
  virtual ~OperadoresComunes() {}
  void inicializar(const size_t n, const size_t m) {
    this->n = n;
    this->m = m;
  };

  void cruce_intercambio(const tSolution& padre1, const tSolution& padre2, vector<tSolution> &poblacion_nueva);
  void cruce_uniforme(const tSolution& padre1, const tSolution& padre2, vector<tSolution> &poblacion_nueva);
  void reparar(const int num_nodos, vector<bool> &hijo);
  void mutar(tSolution &cromosoma);
  tSolution BL_rand(tSolution &sol, int maxevals, Problem *problem, size_t &evals);
};

#endif