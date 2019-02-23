program parallel

! La répartition STATIC consiste à diviser les itérations en paquets
!d'une taille donnée (sauf peut-être pour le dernier).
!Il est ensuite attribué, d'une façon cyclique à chacune des tâches,
!un ensemble de paquets suivant l'ordre des tâches jusqu'à concurrence 
! du nombre total de paquets

 use OMP_LIB

 implicit none

 integer, parameter :: n=4096
 real, dimension(n) :: a
 integer :: i, i_min, i_max, rang, nb_taches


 !$OMP PARALLEL PRIVATE(rang,nb_taches,i_min,i_max)
   rang=OMP_GET_THREAD_NUM()
   nb_taches=OMP_GET_NUM_THREADS()
   i_min=n ; i_max=0
   !$OMP DO SCHEDULE(STATIC)
     do i = 1, n
       a(i) = 92290. + real(i)
       i_min=min(i_min,i)
       i_max=max(i_max,i)
     end do
   !$OMP END DO NOWAIT
   print *,"Rang : ",rang, "; i_min :", i_min, &
           "; i_max :",i_max
 !$OMP END PARALLEL

end program parallel
