#include <stdio.h>
#include <omp.h>

/* Dans un sous-programme appelé dans une région parallèle, 
les variables locales et automatiques sont implicitement privées
 à chacune des tâches (elles sont définies dans la pile de chaque tâche).
*/

int main()
{
  void sub(void);

#pragma omp parallel default(shared)
  {
    sub();
  }
  return 0;
}



void sub(void)
{
  int a;

  a=92290;
  a = a + omp_get_thread_num();
  printf("a vaut : %d\n", a);
}

