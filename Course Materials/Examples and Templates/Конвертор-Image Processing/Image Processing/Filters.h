COLORREF GP(BITMAP image, int x, int y);
void SP(BITMAP image, int x, int y, COLORREF c);
void Invert(BITMAP image);
void GrayScale(BITMAP image);
void Brightness(BITMAP image, int b);
void Contrast(BITMAP image, int c);
void Gamma(BITMAP image, double gr, double gg, double gb);
void Color(BITMAP image, int cr, int cg, int cb);
void Convolution3x3(BITMAP image, int* matrix, int div, int offset);
void Offset(BITMAP image, int* ox, int* oy, int mode);
void Flip(BITMAP image, bool hf, bool vf);
void RandomJitter(BITMAP image, int degree);
void Swirl(BITMAP image, double degree);
void Sphere(BITMAP image);
void TimeWarp(BITMAP image, BYTE degree);
void Moire(BITMAP image, double degree);
void Water(BITMAP image, int wave);
void Pixelate(BITMAP image, int pixel, bool grid);
void NoiseSaltAndPepper(BITMAP image, int percent);
void Median(BITMAP image, int k);