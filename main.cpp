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

#include "AGE_posicion.h"
#include "AGE_uniforme.h"

#include "AM_comun.h"
#include "AM_1.h"
#include "AM_2.h"


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
  AM_1 ram1 = AM_1();
  AM_2 ram2 = AM_2();

  // Create the specific problem
  MDD rproblem = MDD(ruta);
  // Solve using evaluations
  vector<pair<string, MH *>> algoritmos = {make_pair("RandomSearch", &ralg),
                                           make_pair("Greedy", &rgreedy),
                                           make_pair("AGG_posicion", &ragg_pos),
                                           make_pair("AGG_uniforme", &ragg_uni),
                                           make_pair("ram1", &ram1),
                                           make_pair("ram2", &ram2)};
  Problem *problem = dynamic_cast<Problem *>(&rproblem);

  cout << "---------------------------------------" << endl;
  for (int i = 0; i < algoritmos.size(); i++) {
    Random::seed(seed);
    cout << algoritmos[i].first << endl;
    MH *mh = algoritmos[i].second;
    ResultMH result = mh->optimize(problem, 100000);
    cout << "Best solution: " << result.solution << endl;
    cout << "Best fitness: " << result.fitness << endl;
    cout << "Evaluations: " << result.evaluations << endl;
    cout << "---------------------------------------" << endl;
  }

  return 0;
}
