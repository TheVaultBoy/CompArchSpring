#define NUM_OF_THREADS 2
#include <omp.h>
#include <iostream>

using namespace std;

void twoThreads() {
  int a[20];
  int b[20];
  for (int i = 0; i < 20; i++) {
    a[i] = 1;
    b[i] = 2;
  }
  //Serial 
  for (int i = 0; i < 20; i++) {
    cout << a[i];
  }
  for (int i = 0; i < 20; i++) {
    cout << b[i];
  }

  cout << endl;
 //Parallel
omp_set_num_threads(NUM_OF_THREADS);
#pragma omp parallel sections
  {
#pragma omp section 
    {
      for (int i = 0; i < 20; i++) {
        cout << a[i];
      }
    }
#pragma omp section
    {
      for (int i = 0; i < 20; i++)
      {
        cout << b[i];
      }
    }
  }
  cout << endl;
}