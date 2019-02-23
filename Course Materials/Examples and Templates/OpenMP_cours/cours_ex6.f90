program parallel

! Dans une procédure, toutes les variables transmises par argument 
!(dummy parameters) héritent du statut défini dans 
!l'étendue lexicale (statique) de la région.

 implicit none

 integer :: a, b

 a = 92000

 !$OMP PARALLEL SHARED(a) PRIVATE(b)
    call sub(a, b)
    print *,"B vaut : ",b
 !$OMP END PARALLEL

end program parallel


subroutine sub(x, y)
 use OMP_LIB
 implicit none
 integer, intent(in)  :: x
 integer, intent(out) :: y

 y = x + OMP_GET_THREAD_NUM()
end subroutine sub
