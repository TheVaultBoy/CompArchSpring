#include <stdio.h>
#include <omp.h>
#define N 5

/*
 Une réduction est une opération associative
 appliquée à une variable partagée. 
  L'opération peut être :
    * arithmétique : +, --, * ;
    * logique : .AND., .OR., .EQV., .NEQV. ;
    * une fonction intrinsèque : MAX, MIN, IAND, IOR, IEOR. 
 Chaque tâche calcule un résultat partiel indépendemment des autres.
 Elles se synchronisent ensuite pour mettre à jour le résultat final.
*/

int main()
{
  int i, s=0, p=1, r=1,rang;

#pragma omp parallel
  {
    rang=omp_get_thread_num();
#pragma omp for reduction(+:s) reduction(*:p,r)
    for (i=0; i<N; i++) {
      s = s + 1;
      p = p * 2;
  printf("%d -- s = %d ; p = %d\n",rang,s,p);
    }
  }
 printf("\n");
  printf("s = %d ; p = %d\n",s,p);
  return 0;
}
