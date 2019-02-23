
#include <windows.h>
#include <stdio.h>
#include <omp.h>

#define N 10

void main(void)
{
	int arr[N][N];
	int i,j;

	#pragma omp parallel for num_threads(2) private(j)
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)
		{
			arr[i][j] = i*1000 + j;
		}
	}

	#pragma omp parallel for num_threads(2) private(j) ordered
	for(int i=0;i<N;i++)
	{
		#pragma omp ordered
		for(int j=0;j<N;j++)
		{
			printf("%d ",arr[i][j]);
		}
		printf("\n");
	}
}