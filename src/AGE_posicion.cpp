#include <cassert>
#include <AGE_posicion.h>
#include <mh.h>
#include <unordered_set>
#include <set>
#include <iostream>

using namespace std;

/**
 * @param problem The problem to be optimized
 * @param maxevals Maximum number of evaluations allowed
 * @return A pair containing the best solution found and its fitness
 */

ResultMH AGE_posicion::optimize(Problem *problem, int maxevals){
  assert(maxevals > 0);
  size_t evals = 0;

  //n y m
  m = problem->getSolutionSize();
  n = problem->getSolutionDomainRange().second + 1;
  inicializar(n, m);

  //solution
  vector<tSolution> poblacion;
  vector<tSolution> poblacion_sel;
  vector<tSolution> poblacion_nueva;

  tSolution peor_sol, mejor_sol;
  size_t peor_sol_index;

  tFitness fitness;

  tSolution solution, mejor_hijo_reemplazo;
  solution = problem->createSolution();
  poblacion.push_back(solution);

  peor_sol = mejor_sol = solution;

  for (size_t i=1; i<50; i++){
      solution = problem->createSolution();
      poblacion.push_back(solution);

      if (problem->fitness(solution) > problem->fitness(peor_sol)) {
          peor_sol = solution; 
          peor_sol_index = i;
      }
      if (problem->fitness(solution) < problem->fitness(mejor_sol))
          mejor_sol = solution;
  }

  for (; evals < maxevals; evals += 50) {
      poblacion_sel.clear();
      poblacion_nueva.clear();
      poblacion_sel.reserve(2);
      poblacion_nueva.reserve(2);

      //selección
      size_t pos_aux1, pos_aux2;
      for (size_t i=0; i<2; i++) {
        pos_aux1 = Random::get<size_t>(0,poblacion.size()-1);
        pos_aux2 = Random::get<size_t>(0,poblacion.size()-1);

        if (problem->fitness(poblacion[pos_aux2]) < problem->fitness(poblacion[pos_aux1]))
            pos_aux1 = pos_aux2;

        pos_aux2 = Random::get<size_t>(0,poblacion.size()-1);

        if (problem->fitness(poblacion[pos_aux2]) < problem->fitness(poblacion[pos_aux1]))
            poblacion_sel.push_back(poblacion[pos_aux2]);
        else
            poblacion_sel.push_back(poblacion[pos_aux1]);
      }
    
      //cruce
      cruce_intercambio(poblacion_sel[0], poblacion_sel[1], poblacion_nueva);

      //mutación 
      size_t n_mutaciones = ceil(0.1 * poblacion_sel.size());
      size_t mutaciones = 0;

      while (mutaciones < n_mutaciones) {
        for (size_t i=0; i<poblacion_sel.size() && mutaciones < n_mutaciones; i++) {
            mutar(poblacion_nueva[i]);
            mutaciones ++;
        }
      }

      //averiguar qué hijo tiene mejor fitness
      mejor_hijo_reemplazo = poblacion_nueva[1];
      if (problem->fitness(poblacion_nueva[0]) < problem->fitness(poblacion_nueva[1]))
          mejor_hijo_reemplazo = poblacion_nueva[0];

      //comprobar si el mejor hijo es la mejor solución
      if (problem->fitness(mejor_hijo_reemplazo) < problem->fitness(mejor_sol))
          mejor_sol = mejor_hijo_reemplazo;
          
      //reemplazo
      if (problem->fitness(peor_sol) > problem->fitness(mejor_hijo_reemplazo));
          poblacion[peor_sol_index] = mejor_hijo_reemplazo;


      //recalcular peor solución
      peor_sol = poblacion_nueva[0];
      peor_sol_index = 0;
      for (size_t i=1; i<poblacion_nueva.size(); i++){
        if (problem->fitness(poblacion_nueva[i]) > problem->fitness(peor_sol)) {
          peor_sol = poblacion_nueva[i];
          peor_sol_index = i;
        }
      }
  }

  fitness = problem->fitness(mejor_sol);
  return ResultMH(mejor_sol, fitness, evals);
}

