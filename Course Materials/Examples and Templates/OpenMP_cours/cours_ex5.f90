program parallel

! Dans un sous-programme appelé dans une région parallèle, 
!les variables locales et automatiques sont implicitement privées
! à chacune des tâches (elles sont définies dans la pile de chaque tâche).


 implicit none

 !$OMP PARALLEL DEFAULT(SHARED)
    call sub()
 !$OMP END PARALLEL
end program parallel


subroutine sub()
 use OMP_LIB
 implicit none
 integer :: a

  a = 92290
  a = a + OMP_GET_THREAD_NUM()
  print *,"A vaut : ",a
end subroutine sub
