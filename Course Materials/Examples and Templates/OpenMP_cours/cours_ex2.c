#include <stdio.h>
#include <omp.h>
int main()
{
  float a;

  /*! Si une variable possède un statut privé (PRIVATE), 
elle se trouve dans la pile de chaque tâche.
 Sa valeur est alors indéfinie à l'entrée d'une région parallèle 
(dans l'exemple ci-contre, la variable a vaut 0 à l'entrée 
de la région parallèle).*/


  a = 92000.;
  #pragma omp parallel default(none) private(a)
  {
    a = a + 290.;
    printf("a vaut : %f\n",a);
  }
  return 0;
}
