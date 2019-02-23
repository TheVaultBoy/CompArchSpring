#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <cutil.h>
#include <cuda_runtime_api.h>
#include <algorithm>
using namespace std;
#pragma comment(lib, "cutil32D.lib")

int rint(int l, int h)
{
	int r = h-l+1, res;
	res = rand()%r+l;
	return res;
}

int* rintarr(int size, int l, int h)
{
	int *res = new int[size];//, t;
	for (int i =0; i <= size - 1; i++)
	{	
		//t = rint(0, size);
		res[i] = size-i;//rint(l+t*h, h+t*h);
	}
	return res;
}

void iaprint(int *arr, int size)
{
	for (int i =0; i <= size - 1; i++)
		printf("%i ", arr[i]);
	printf("\n");
}

//Выводим информацию об устройствах
cudaDeviceProp* ShowDeviceInfo()
{
	int deviceCount;
	cudaDeviceProp deviceProp;
	
	//Получаем и выводим количество устройств
	cudaGetDeviceCount(&deviceCount);
	printf("Device count: %d\n\n", deviceCount);

	for (int i = 0; i <= deviceCount - 1; i++)
	{
		//Получаем информацию о текущем устройстве
		cudaGetDeviceProperties(&deviceProp, i);
		
		//Выводим иформацию об устройстве
		printf("Device name: %s\n", deviceProp.name);
		printf("Total global memory: %d\n", deviceProp.totalGlobalMem);
		printf("Shared memory per block: %d\n", deviceProp.sharedMemPerBlock);
		printf("Registers per block: %d\n", deviceProp.regsPerBlock);
		printf("Warp size: %d\n", deviceProp.warpSize);
		printf("Memory pitch: %d\n", deviceProp.memPitch);
		printf("Max threads per block: %d\n", deviceProp.maxThreadsPerBlock);
		printf("Max threads dimensions: x = %d, y = %d, z = %d\n", deviceProp.maxThreadsDim[0], deviceProp.maxThreadsDim[1], deviceProp.maxThreadsDim[2]);
		printf("Max grid size: x = %d, y = %d, z = %d\n", deviceProp.maxGridSize[0], deviceProp.maxGridSize[1], deviceProp.maxGridSize[2]); 
		printf("Clock rate: %d\n", deviceProp.clockRate);
		printf("Total constant memory: %d\n", deviceProp.totalConstMem);
		printf("Compute capability: %d.%d\n", deviceProp.major, deviceProp.minor);
		printf("Texture alignment: %d\n", deviceProp.textureAlignment);
		printf("Device overlap: %d\n", deviceProp.deviceOverlap);
		printf("Multiprocessor count: %d\n", deviceProp.multiProcessorCount);
		printf("Kernel execution timeout enabled: %s\n", deviceProp.kernelExecTimeoutEnabled ? "true" : "false");
	}
	
	return &deviceProp;
}

//Спецификаторы вызова: global - GPU из CPU; device - GPU из GPU; host - CPU из CPU
//Использован один блок
__global__ void vadd(float *v1, float *v2, float *v3)
{
	int cur = threadIdx.x; //Получение нномера текущей нити
	v3[cur] = v1[cur] + v2[cur];
}

__global__ void sort(int *arr, int size)
{
	int i = threadIdx.x;
	int k = 0, b = arr[i];
		for (int j = 0; j <= size - 1; j++)
			if (arr[j] < b) k++;
	__syncthreads();
	arr[k] = b;
}

int main()
{
	/*--------------------vadd---------------------*/
	unsigned int t1;
	cutCreateTimer(&t1);
	cutResetTimer(t1);
	cudaSetDevice(0);
	
	cudaDeviceProp *deviceProp = ShowDeviceInfo();
	
	int size = deviceProp->maxThreadsPerBlock;
	float *v1 = new float[size];
	float *v2 = new float[size];
	float *v3 = new float[size];
	for (int i = 0; i <= size - 1; i++)
	{
		v1[i] = (float)i/10 + (float)i;
		v2[i] = (float)i/5 + (float)2*i;
	}
	
	float *cv1, *cv2, *cv3;
	cudaMalloc((void**)&cv1, size*sizeof(float));
	cudaMalloc((void**)&cv2, size*sizeof(float));
	cudaMalloc((void**)&cv3, size*sizeof(float));
	cudaMemcpy(cv1, v1, size*sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(cv2, v2, size*sizeof(float), cudaMemcpyHostToDevice);
	
	dim3 gridSize = dim3(1, 1, 1); //Используем только один блок
	dim3 blockSize = dim3(size, 1, 1); //Используем size нитей
	//Событие окончания сложения
	cudaEvent_t event;
	cudaEventCreate(&event);
	cudaEventRecord(event, 0); //0 - номер основного потока
	cutStartTimer(t1);
	vadd<<<gridSize, blockSize>>>(cv1, cv2, cv3); //Складываем вектора (асинхронно)
	cudaEventSynchronize(event); //Ожидание окончания работы всех нитей GPU
	cutStopTimer(t1);
	
	cudaMemcpy(v3, cv3, size*sizeof(float), cudaMemcpyDeviceToHost);
	/*for (int i = 0; i <= size - 1; i++)
	{
		printf("%.2f + %.2f = %.2f\n", v1[i], v2[i], v3[i]);
	}*/
	printf("\nGPU vadd: %f ms\n\n", cutGetTimerValue(t1));
	
	cudaEventDestroy(event);
	cudaFree(cv1); cudaFree(cv2); cudaFree(cv3);
	delete[] v1; delete[] v2; delete[] v3;
	getch();
	/*---------------------------------------------*/
	
	/*--------------------sort---------------------*/
	srand(0);
	int s = 100;
	int *arr = rintarr(s, 0, 5);
	iaprint(arr, s);
	printf("\n");
	
	int *carr;
	cudaMalloc((void**)&carr, s*sizeof(int));
	cudaEvent_t e;
	cudaEventCreate(&e);
	cudaEventRecord(e, 0);
	cutStartTimer(t1);
	cudaMemcpy(carr, arr, s*sizeof(int), cudaMemcpyHostToDevice);
	sort<<<1, s>>>(carr, s);
	cudaEventSynchronize(e);
	cutStopTimer(t1);
	
	cudaEventDestroy(e);
	int *brr = new int[s];
	cudaMemcpy(brr, carr, s*sizeof(int), cudaMemcpyDeviceToHost);
	iaprint(brr, s);
	printf("GPU vadd: %f ms\n\n", cutGetTimerValue(t1));
	
	cutStartTimer(t1);
	sort(arr, arr+s);
	iaprint(arr, s);
	cutStopTimer(t1);
	printf("CPU vadd: %f ms\n\n", cutGetTimerValue(t1));
	
	cudaFree(carr);
	delete[] arr;
	delete[] brr;
	getch();
	/*---------------------------------------------*/
	return 0;
}