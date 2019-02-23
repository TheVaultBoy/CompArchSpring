program parallel


! Une réduction est une opération associative
! appliquée à une variable partagée. 
!  L'opération peut être :
!    * arithmétique : +, --, * ;
!    * logique : .AND., .OR., .EQV., .NEQV. ;
!    * une fonction intrinsèque : MAX, MIN, IAND, IOR, IEOR. 
! Chaque tâche calcule un résultat partiel indépendemment des autres.
! Elles se synchronisent ensuite pour mettre à jour le résultat final.
 

USE OMP_LIB
 implicit none

 integer, parameter :: n=5
 integer            :: i, s=0, p=1, r=1,rang

 !$OMP PARALLEL
  rang=OMP_GET_THREAD_NUM()
   !$OMP DO REDUCTION(+:s) REDUCTION(*:p)
    do i = 1, n
      s = s + 1
      p = p * 2

print*,"rang = ",rang,"s =",s, "; p =",p
    end do
   !$OMP END DO

 !$OMP END PARALLEL
print *," "
 print *,"s =",s, "; p =",p

end program parallel
