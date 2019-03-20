#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>

#include "Header.h"

#define TX 27 // number of threads per block along x-axis
#define TY 27 // number of threads per block along y-axis

__global__ void greyscaleKernel(pixel* img, unsigned W, unsigned H, unsigned dim)
{
  const int x = blockIdx.x * blockDim.x + threadIdx.x;
  const int y = blockIdx.y * blockDim.y + threadIdx.y;

  unsigned int r_avg = 0;
  unsigned int g_avg = 0;
  unsigned int b_avg = 0;
  int temp = (dim - 1) / 2;
  if ((((y - temp)*W + x - temp) < W*H) && (((y + temp)*W + x + temp) < W*H))
  {
    for (int i = temp; i > -(temp+1); i--)
    {
      for (int j = -temp; j < (temp+1); j++)
      {
        r_avg += img[(y + i)*W + x + j].r;
        g_avg += img[(y + i)*W + x + j].g;
        b_avg += img[(y + i)*W + x + j].b;
      }
    }
  }
  img[y*W + x].r = r_avg/(dim*dim);
  img[y*W + x].g = g_avg/(dim*dim);
  img[y*W + x].b = b_avg/ (dim*dim);
}


// Helper function for using CUDA to add vectors in parallel.
void cudaGrayscale(pixel* h_img, unsigned W, unsigned H, unsigned dim)
{
  pixel* d_img;
  size_t bytes = W * H * sizeof(pixel);
  cudaMalloc(&d_img, bytes);
  cudaMemcpy(d_img, h_img, bytes, cudaMemcpyHostToDevice);

  dim3 blockSize(TX, TY);
  int bx = (W + 26) / blockSize.x;
  int by = (H + 26) / blockSize.y;
  dim3 gridSize = dim3(bx, by);

  // Launch a kernel on the GPU with one thread for each element.
  greyscaleKernel<<<gridSize, blockSize>>>(d_img, W, H ,dim);

  cudaMemcpy(h_img, d_img, bytes, cudaMemcpyDeviceToHost);
  cudaFree(d_img);
}
