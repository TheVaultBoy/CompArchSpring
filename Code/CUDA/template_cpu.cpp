#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include <string>

#include <chrono>

#include "Header.h"

using namespace cv;
using namespace std;
int main(int argc, char** argv)

{
  String imageName("Peach.jpg"); // by default
  if (argc > 1)
  {
    imageName = argv[1];
  }
  Mat image;
  image = imread(imageName, IMREAD_COLOR); // Read the file
  if (image.empty())                      // Check for invalid input
  {
    cout << "Could not open or find the image" << std::endl;
    return -1;
  }



  unsigned W = image.cols;
  unsigned H = image.rows;
  pixel* img = new pixel[W*H];
  for (int y = 0; y < H; y++)
  {
    for (int x = 0; x < W; x++)
    {
      unsigned char * p = image.ptr(y, x); // Y first, X after
      img[y*W + x].b = p[0];   // B
      img[y*W + x].g = p[1];  // G
      img[y*W + x].r = p[2]; // R
    }
  }

  auto startPoint = chrono::steady_clock::now();

  cudaGrayscale(img, W, H, 15);

  auto endPoint = chrono::steady_clock::now();
  cout << "Time: " << chrono::duration_cast<chrono::milliseconds>(endPoint - startPoint).count()
    << "ms" << endl;


  for (int y = 0; y < H; y++)
  {
    for (int x = 0; x < W; x++)
    {
      unsigned char * p = image.ptr(y, x); // Y first, X after
      p[0] = img[y*W + x].b;   // B
      p[1] = img[y*W + x].g;  // G
      p[2] = img[y*W + x].r; // R
    }
  }

  namedWindow("Display window", WINDOW_AUTOSIZE); // Create a window for display.
  imshow("Display window", image);                // Show our image inside it.
  waitKey(0); // Wait for a keystroke in the window
   return 0;
}