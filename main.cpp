#include <iostream>
#include <problem.h>
#include <random.hpp>
#include <string>
#include <util.h>
// Real problem class
#include "MDD.h"

// All all algorithms
#include "brutesearch.h"
#include "greedy.h"
#include "randomsearch.h"
#include "AGG_posicion.h"
#include "AGG_uniforme.h"

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
  RandomSearch ralg = RandomSearch();
  GreedySearch rgreedy = GreedySearch();
  AGG_posicion ragg_pos = AGG_posicion();
  AGG_uniforme ragg_uni = AGG_uniforme();

  // Create the specific problem
  MDD rproblem = MDD(ruta);
  // Solve using evaluations
  vector<pair<string, MH *>> algoritmos = {make_pair("RandomSearch", &ralg),
                                           make_pair("Greedy", &rgreedy),
                                           make_pair("AGG_posicion", &ragg_pos),
                                           make_pair("AGG_uniforme", &ragg_uni)};
  Problem *problem = dynamic_cast<Problem *>(&rproblem);

  for (int i = 0; i < algoritmos.size(); i++) {
    Random::seed(seed);
    cout << algoritmos[i].first << endl;
    MH *mh = algoritmos[i].second;
    ResultMH result = mh->optimize(problem, 100000);
    cout << "Best solution: " << result.solution << endl;
    cout << "Best fitness: " << result.fitness << endl;
    cout << "Evaluations: " << result.evaluations << endl;
  }

  return 0;
}
