#include <windows.h>
#include <stdio.h>
#include <omp.h>
#include <iostream>
#include <conio.h>
using namespace std;

int func()
{
	for(int i=0;i<10000;i++)
	{
		rand();
	}
    return 1;
}

void main(void)
{
	//Однопоточное выполнение
	int time1 = GetTickCount();
	for(int i = 0; i < 10000; ++i)
			func();
	int time2 = GetTickCount();
	time2 = time2 - time1;
	cout << "TIME 1: " << time2 << endl;

	//Выполнение в два потока
	time1 = GetTickCount();
	#pragma omp parallel for num_threads(2)
	for(int i = 0; i < 10000; ++i)
	{
		//printf("%d\n",i);
		func();
	}
	time2 = GetTickCount();
	time2 = time2 - time1;
	cout << "TIME 2: " << time2 << endl;
	_getch();
}