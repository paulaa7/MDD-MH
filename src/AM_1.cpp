#include <cassert>
#include <AM_1.h>
#include <unordered_set>
#include <set>
#include <iostream>

using namespace std;

/**
 * @param problem The problem to be optimized
 * @param maxevals Maximum number of evaluations allowed
 * @return A pair containing the best solution found and its fitness
 */
  
ResultMH AM_1::optimize(Problem *problem, int maxevals){
    assert(maxevals > 0);
    size_t evals = 0;
  
    //n y m
    m = problem->getSolutionSize();
    n = problem->getSolutionDomainRange().second + 1;
  
    //solution
    vector<tSolution> poblacion;
    vector<tSolution> poblacion_sel;
    vector<tSolution> poblacion_nueva;

    tSolution peor_sol;
    size_t peor_sol_index;
    tSolution mejor_sol;
    tSolution mejor_sol_ant;
    tFitness fitness;

    tSolution solution;
    solution = problem->createSolution();
    poblacion.push_back(solution);

    peor_sol = mejor_sol = mejor_sol_ant = solution;

    for (size_t i=1; i<50; i++){
        solution = problem->createSolution();
        poblacion.push_back(solution);

        if (problem->fitness(solution) < problem->fitness(mejor_sol)) mejor_sol = solution;
        if (problem->fitness(solution) > problem->fitness(peor_sol)) {
          peor_sol = solution; 
          peor_sol_index = i;
        }
    }

    for (; evals < maxevals; evals += 50) {
        poblacion_sel.clear();
        poblacion_nueva.clear();
        poblacion_sel.reserve(50);
        poblacion_nueva.reserve(50);

        //selección
        tSolution sol1, sol2;
        size_t pos_aux1, pos_aux2;
        for (size_t i=0; i<poblacion.size(); i++) {
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
        size_t n_cruces = ceil(0.7 * (poblacion_sel.size()*0.5));
        size_t cruces = 0;
        size_t index = 0;

        while (cruces < n_cruces) {
          for (; index<poblacion_sel.size() && cruces < n_cruces; index+=2) {
            cruce_uniforme(poblacion_sel[index], poblacion_sel[index+1], poblacion_nueva);
            cruces ++;
          }
        }

        for (; index<poblacion_sel.size(); index++)
          poblacion_nueva.push_back(poblacion_sel[index]);

        //mutación
        size_t n_mutaciones = ceil(0.1 * poblacion_nueva.size());
        size_t mutaciones = 0;

        while (mutaciones < n_mutaciones) {
          for (size_t i=0; i<poblacion_nueva.size() && mutaciones < n_mutaciones; i++) {
              mutar(poblacion_nueva[i]);
              mutaciones ++;
          }
        }

        for (size_t i=0; i<poblacion_nueva.size(); i++){
          if (problem->fitness(poblacion_nueva[i]) < problem->fitness(mejor_sol)) mejor_sol = poblacion_nueva[i];
          if (problem->fitness(poblacion_nueva[i]) > problem->fitness(peor_sol)) {
            peor_sol = poblacion_nueva[i];
            peor_sol_index = i;
          }
        }

        //elitismo
        if (problem->fitness(peor_sol) > problem->fitness(mejor_sol_ant))
          poblacion_nueva[peor_sol_index] = mejor_sol_ant;

        mejor_sol_ant = mejor_sol;
        poblacion = poblacion_nueva;
    }

    fitness = problem->fitness(mejor_sol);
    return ResultMH(mejor_sol, fitness, evals);
}

