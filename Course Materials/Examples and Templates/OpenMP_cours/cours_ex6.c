#include <stdio.h>
#include <omp.h>

/* 
Dans une procédure, toutes les variables transmises par argument 
(dummy parameters) héritent du statut défini dans 
l'étendue lexicale (statique) de la région.
*/

int main()
{
  void sub(int x, int *y);
  int a, b;


  a = 92000;
#pragma omp parallel shared(a) private(b)
  {
    sub(a, &b);
    printf("b vaut : %d\n",b);
  }
  return 0;
}
          



void sub(int x, int *y)
{
  *y = x + omp_get_thread_num();
}
