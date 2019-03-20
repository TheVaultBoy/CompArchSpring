#include <omp.h>
#include <iostream>

using namespace std;

void criticalAndMaster() {
  const int NUM_OF_THREADS = omp_get_num_procs();
  const int NUM_OF_ITERATIONS = 10000;
  omp_set_num_threads(NUM_OF_THREADS);

  int a = 0;
  int b = 0;
  int c = 0;

#pragma omp parallel
  {
    for (int i = 0; i < NUM_OF_ITERATIONS; i++)
      a++;
#pragma omp critical
    {
      for (int i = 0; i < NUM_OF_ITERATIONS; i++)
        b++;
    }
#pragma omp master
    {
      for (int i = 0; i < NUM_OF_ITERATIONS; i++)
        c++;
    }
  }

  cout << a << endl;
  cout << b << endl;
  cout << c << endl;
}