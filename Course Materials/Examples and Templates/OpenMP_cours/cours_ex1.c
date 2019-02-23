#include <stdio.h>
#include <omp.h>
int main()
{
  float a;
  int   p;

  /*Dans une région parallèle, par défaut, le statut des variables est partagé.
Au sein d'une même région parallèle, toutes les tâches concurrentes
exécutent le même code.
Il existe une barrière implicite de synchronisation en fin de région parallèle.
Il est interdit d'effectuer des « branchements » (ex. GOTO, CYCLE, etc.) 
vers l'intérieur ou vers l'extérieur d'une région parallèle ou de toute autre construction OpenMP.
  */


  a = 92290. ; p = 0;
#pragma omp parallel 
  {

#ifdef _OPENMP
    p=omp_in_parallel();
#endif
    printf("a vaut : %f ; p vaut : %d\n",a,p);
  }
   printf("\n");
   printf("a vaut : %f ; p vaut : %d\n",a,p);
  return 0;
}
