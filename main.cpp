#include <iostream>
#include <problem.h>
#include <random.hpp>
#include <string>
#include <util.h>
// Real problem class
#include "MDD.h"

// All all algorithms
#include "greedy.h"
#include "localsearch_heur.h"
#include "localsearch_rand.h"

#include "AGG_posicion.h"
#include "AGG_uniforme.h"

#include "AGE_posicion.h"
#include "AGE_uniforme.h"

#include "OperadoresComunes.h"
#include "AM_1.h"
#include "AM_2.h"
#include "AM_3.h"


using namespace std;
int main(int argc, char *argv[]) {
  long int seed;
  string path = "";

  // Seed the random number generator
  if (argc == 1) {
    // Use a fixed seed for reproducibility
    seed = 42;
  } else {
    seed = atoi(argv[1]);
    path = "./datos_MDD/" + string(argv[2]) + ".txt";
  }

  const char* ruta = path.c_str();

  // Create the algorithms
  GreedySearch rgreedy = GreedySearch();
  LocalSearchHeur rlocalh = LocalSearchHeur();
  LocalSearchRandom rlocal = LocalSearchRandom();

  AGG_posicion ragg_pos = AGG_posicion();
  AGG_uniforme ragg_uni = AGG_uniforme();
  AGE_posicion rage_pos = AGE_posicion();
  AGE_uniforme rage_uni = AGE_uniforme();

  AM_1 ram1 = AM_1();
  AM_2 ram2 = AM_2();
  AM_3 ram3 = AM_3();

  // Create the specific problem
  MDD rproblem = MDD(ruta);
  // Solve using evaluations
  vector<pair<string, MH *>> algoritmos = {make_pair("Greedy", &rgreedy),
                                           make_pair("Local_heur", &rlocalh),
                                           make_pair("Local_Random", &rlocal),
                                           make_pair("AGG_posicion", &ragg_pos),
                                           make_pair("AGG_uniforme", &ragg_uni),
                                           make_pair("AGE_posicion", &rage_pos),
                                           make_pair("AGE_uniforme", &rage_uni),
                                           make_pair("Algoritmo Memetico (10,1.0)", &ram1),
                                           make_pair("Algoritmo Memetico (10,0.1)", &ram2),
                                           make_pair("Algoritmo Memetico (10,0.1mej)", &ram3)};
  Problem *problem = dynamic_cast<Problem *>(&rproblem);

    //cout << "---------------------------------------" << endl;
    //for (int i = 0; i < algoritmos.size(); i++) {
    // Random::seed(seed);
    // cout << algoritmos[i].first << endl;
    // MH *mh = algoritmos[i].second;
    // ResultMH result = mh->optimize(problem, 100000);
    // cout << "-> Best solution: " << result.solution << endl;
    // cout << "-> Best fitness: " << result.fitness << endl;
    // cout << "-> Evaluations: " << result.evaluations << endl;
    // cout << "---------------------------------------" << endl;
    //}

    Random::seed(seed);
    //cout << algoritmos[0].first << endl;
    MH *mh = algoritmos[9].second;

    auto inicio = std::chrono::high_resolution_clock::now();
    ResultMH result = mh->optimize(problem, 100000);
    auto fin = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracion = fin - inicio;

    //cout << "Best solution: " << result.solution << endl;
    cout << "Fitness: " << result.fitness << endl;
    //cout << "Evaluations: " << result.evaluations << endl;
    cout << "Time:" << duracion.count() << endl;

  return 0;
}
