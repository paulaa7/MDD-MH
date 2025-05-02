#ifndef __AM_COMUN_H
#define __AM_COMUN_H

#pragma once
#include <mh.h>

using namespace std;

/**
 *
 * @see MH
 * @see Problem
 */
class AM_comun : public MH {

private:
  size_t m, n;
  
public:
  AM_comun() : MH() {}
  virtual ~AM_comun() {}

  void cruce_uniforme(const tSolution& padre1, const tSolution& padre2, vector<tSolution> &poblacion_nueva);
  void reparar(const int num_nodos, vector<bool> &hijo);
  void mutar(tSolution &cromosoma);
  tSolution BL_rand(tSolution &sol, int maxevals, Problem * problem);
};

#endif