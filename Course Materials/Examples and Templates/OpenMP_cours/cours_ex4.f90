program parallel

! L'étendue d'une construction OpenMP représente le champ 
!d'influence de celle-ci dans le programme.
!L'influence (ou la portée) d'une région parallèle s'étend 
!aussi bien au code contenu lexicalement dans cette région (étendue statique),
! qu'au code des sous-programmes appelés. 
!L'union des deux représente « l'étendue dynamique ».

 implicit none

 !$OMP PARALLEL
    call sub()
 !$OMP END PARALLEL

end program parallel


subroutine sub()
 use OMP_LIB
 implicit none
 logical :: p=.false.

 !$ p = OMP_IN_PARALLEL()
 print *,"Parallele ?:", p
end subroutine sub
