program parallel

! Cependant, grâce à la clause FIRSTPRIVATE,
! il est possible de forcer l'initialisation de cette variable privée 
!à la dernière valeur qu'elle avait avant l'entrée dans la région parallèle.
 
implicit none

 real :: a

 a = 92000.

 !$OMP PARALLEL DEFAULT(NONE) &
   !$OMP FIRSTPRIVATE(a)
   a = a + 290.
   print *,"A vaut : ",a
 !$OMP END PARALLEL

 print*,"Hors region, A vaut :",a
end program parallel
