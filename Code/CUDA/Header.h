#ifndef PARALLEL_EDGE_H
#define PARALLEL_EDGE_H
struct pixel
{
  unsigned char r;
  unsigned char g;
  unsigned char b;
};
void cudaGrayscale(pixel* h_img, unsigned W, unsigned H, unsigned dim);
#endif