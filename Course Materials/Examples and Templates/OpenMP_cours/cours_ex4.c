#include <stdio.h>
#include <omp.h>

/* L'étendue d'une construction OpenMP représente le champ 
d'influence de celle-ci dans le programme.
L'influence (ou la portée) d'une région parallèle s'étend 
aussi bien au code contenu lexicalement dans cette région (étendue statique),
 qu'au code des sous-programmes appelés. 
 L'union des deux représente « l'étendue dynamique ».*/



int main()
{
  void sub(void);

#pragma omp parallel
  {
    sub();
  }
  return 0;
}


void sub(void)
{
  int p=0;

#ifdef _OPENMP
    p = omp_in_parallel();
#endif
  printf("Parallele ? : %d\n", p);
}

