#include <stdio.h>
#include <omp.h>
int main()
{
  float a;
  /* Cependant, grâce à la clause FIRSTPRIVATE,
il est possible de forcer l'initialisation de cette variable privée 
à la dernière valeur qu'elle avait avant l'entrée dans la région parallèle.*/
 
  a = 92000.;
#pragma omp parallel default(none) firstprivate(a)
  {
    a = a + 290.;
    printf("a vaut : %f\n",a);
  }
  printf("Hors region, a vaut : %f\n", a);
  return 0;
}
