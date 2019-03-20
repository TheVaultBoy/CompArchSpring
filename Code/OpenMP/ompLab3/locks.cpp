#include <omp.h>
#include <iostream>

using namespace std;

void locks()
{
  //If the critical section would be used on histogram
 //It would effectively serialize the code
 //But knowing that the chance of collisions is relatively low
 //Using locks would be a much better solution

  const int BUCKETS = 16;
  int hist[16];
  for (int i = 0; i < BUCKETS; i++)
    hist[i] = 0;

  omp_lock_t* hist_locks = new omp_lock_t[BUCKETS];

#pragma omp parallel for
  for (int i = 0; i < BUCKETS; i++)
  {
    omp_init_lock(&hist_locks[i]);
  }
#pragma omp parallel for
  for (int i = 0; i < 1020; i++)
  {
    int index = i % BUCKETS;
    omp_set_lock(&hist_locks[index]);
    hist[index]++;
    omp_unset_lock(&hist_locks[index]);
  }

  for (int i = 0; i < BUCKETS; i++)
    omp_destroy_lock(&hist_locks[i]);

  for (int i = 0; i < BUCKETS; i++)
    cout << "hist[" << i << "]: " << hist[i] << endl;

}