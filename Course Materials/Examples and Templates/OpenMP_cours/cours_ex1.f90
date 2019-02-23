program parallel

!Dans une région parallèle, par défaut, le statut des variables est partagé.
! Au sein d'une même région parallèle, toutes les tâches concurrentes
! exécutent le même code.
! Il existe une barrière implicite de synchronisation en fin de région parallèle.
! Il est interdit d'effectuer des « branchements » (ex. GOTO, CYCLE, etc.) 
!vers l'intérieur ou vers l'extérieur d'une région parallèle ou de toute autre construction OpenMP.

 use OMP_LIB

 implicit none
 real    :: a
 logical :: p

 a = 92290. ; p=.false.

 !$OMP PARALLEL 
 !$ p = OMP_IN_PARALLEL()
  print *,"A vaut : ",a, &
          "; p vaut : ",p
 !$OMP END PARALLEL

end program parallel
