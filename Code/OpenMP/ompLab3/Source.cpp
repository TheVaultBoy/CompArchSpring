#include "Header.h"
#include <iostream>
#include <omp.h>


using namespace std;

int main() {
  cout << "Two threads:" << endl << endl;
  twoThreads();
  cout << endl << "Critical and Master:" << endl << endl;
  criticalAndMaster();
  cout << endl << "Locks:" << endl << endl;
  locks();
return 0;
}