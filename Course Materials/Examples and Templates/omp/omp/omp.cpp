#include <omp.h>
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <conio.h> 

using namespace std;



 
 
void func()
{
	for(int i= 0; i < 50000; i++)
		rand();
}

int main()
{

	omp_set_num_threads(2);
	omp_lock_t dmax_lock; 
	omp_init_lock (&dmax_lock); 

	#pragma omp parallel for
	
	for (int i= 0; i < 100; i++)
	{
		
		//cout << 0;
		func();
		
	}
	


#pragma omp parallel
{

#pragma omp sections nowait
{
	
#pragma omp section
	{	omp_set_lock(&dmax_lock);
	for (int i= 0; i < 20; i++)
	{
		cout << 1;
		func();
	}
	omp_unset_lock(&dmax_lock);}
#pragma omp section
	
	{
	for (int i= 0; i < 10; i++)
	{
		cout << 2;
		func();
	}

	omp_test_lock(&dmax_lock);

		
	for (int i= 0; i < 10; i++)
	{
		cout << 3;
		func();
	}
}
}

/*#pragma omp barrier
for (int i= 0; i < 10; i++)
	{
		cout << 3;
		func();
	}
}*/

/*cout << omp_get_num_threads() << endl;
	
int time1 = GetTickCount();
	
cout << "Core 2 Duo" << endl;

for (int i = 0; i < 2; i++)
{
		//измерение времени выполнения цикла на одном ядре и в двух потоках на разных ядрах
#pragma omp parallel sections
{

#pragma omp section
		for (int j = 0; j < 100; j++)
		{			
			cout << 1;	
			func();
		}
#pragma omp section
		for (int j = 100; j < 200; j++)
		{			
			cout << 1;		
			func();
		}
}

}

int time1end = GetTickCount();
cout << "TIME 2 Core: " << time1end - time1 << endl;


cout << "Core 1" << endl;

#pragma omp parallel for num_threads(2)
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 200; j++)
		{			
			cout << 1;	
			func();
		}
	}


int time2end = GetTickCount();
cout << "TIME 2 Core: " << time2end - time1end << endl;


	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 200; j++)
		{			
			cout << 1;	
			func();
		}
	}

int time3end = GetTickCount();
cout << "TIME 1 Core: " << time3end - time2end << endl;

*/

	
}
getch();
return (0);
}