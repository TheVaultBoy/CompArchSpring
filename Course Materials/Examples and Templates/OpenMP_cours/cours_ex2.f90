program parallel

! Si une variable possède un statut privé (PRIVATE), 
!elle se trouve dans la pile de chaque tâche.
! Sa valeur est alors indéfinie à l'entrée d'une région parallèle 
!(dans l'exemple ci-contre, la variable a vaut 0 à l'entrée 
!de la région parallèle).

 implicit none
 real :: a

 a = 92000.

 !$OMP PARALLEL DEFAULT(PRIVATE)
   a = a + 290.
   print *,"A vaut : ",a
 !$OMP END PARALLEL



end program parallel
