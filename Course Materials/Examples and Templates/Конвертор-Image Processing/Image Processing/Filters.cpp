#include "stdafx.h"
#include "Filters.h"

COLORREF GP(BITMAP image, int x, int y)
{
	COLORREF res = RGB(255,255,255);
	BYTE* bytes = (BYTE*)(image.bmBits);
	int wb = image.bmWidthBytes, h = image.bmHeight, p = (int)(image.bmBitsPixel/8);
	res = RGB(*(bytes+(h-y-1)*wb+x*p+2), *(bytes+(h-y-1)*wb+x*p+1), *(bytes+(h-y-1)*wb+x*p));
	return res;
}

void SP(BITMAP image, int x, int y, COLORREF c)
{
	BYTE* bytes = (BYTE*)(image.bmBits);
	int wb = image.bmWidthBytes, h = image.bmHeight, p = (int)(image.bmBitsPixel/8);
	*(bytes+(h-y-1)*wb+x*p+2) = GetRValue(c);
	*(bytes+(h-y-1)*wb+x*p+1) = GetGValue(c);
	*(bytes+(h-y-1)*wb+x*p) = GetBValue(c);
}

void Invert(BITMAP image)
{
	BYTE* pixels = (BYTE*)(image.bmBits);
	int w = image.bmWidth, h = image.bmHeight, p = (int)(image.bmBitsPixel/8), i, j;
	for (j=0; j<=h-1; j++)
		for (i=0; i<=w-1; i++)
			SP(image, i, j, 0x00FFFFFF-GP(image, i, j));
}

void GrayScale(BITMAP image)
{
	COLORREF temp;
	BYTE* pixels = (BYTE*)(image.bmBits);
	int w = image.bmWidth, h = image.bmHeight, p = (int)(image.bmBitsPixel/8), i, j, r, g, b;
	for (j=0; j<=h-1; j++)
	{
		for (i=0; i<=w-1; i++)
		{
			temp = GP(image,i,j);
			r = GetRValue(temp); g = GetGValue(temp); b = GetBValue(temp);
			r = g = b = (BYTE)(0.299*(double)r+0.587*(double)g+0.114*(double)b);
			SP(image,i,j,RGB(r,g,b));
		}
	}
}

void Brightness(BITMAP image, int b)
{
	if (b<-255) b=-255; if (b>255) b=255;
	COLORREF temp;
	BYTE* pixels = (BYTE*)(image.bmBits);
	int w = image.bmWidth, h = image.bmHeight, p = (int)(image.bmBitsPixel/8), i, j, pr, pg, pb;
	for (j=0; j<=h-1; j++)
	{
		for (i=0; i<=w-1; i++)
		{
			temp = GP(image,i,j);
			pr = GetRValue(temp); pg = GetGValue(temp); pb = GetBValue(temp);
			pr+=b; if (pr<0) pr=0; if (pr>255) pr=255;
			pg+=b; if (pg<0) pg=0; if (pg>255) pg=255;
			pb+=b; if (pb<0) pb=0; if (pb>255) pb=255;
			SP(image,i,j,RGB(pr,pg,pb));
		}
	}
}

void Contrast(BITMAP image, int c)
{
	if (c<-100) c=-100; if (c>100) c=100;
	COLORREF temp;
	BYTE* pixels = (BYTE*)(image.bmBits);
	int w = image.bmWidth, h = image.bmHeight, p = (int)(image.bmBitsPixel/8), i, j, pr, pg, pb;
	double contrast = (100.0+(double)c)/100.0, t = 0;
	contrast *= contrast;
	for (j=0; j<=h-1; j++)
	{
		for (i=0; i<=w-1; i++)
		{
			temp = GP(image,i,j);
			pr = GetRValue(temp); pg = GetGValue(temp); pb = GetBValue(temp);
			t = (((double)pr/255-0.5)*contrast+0.5)*255;
			if (t<0) t=0.0; if (t>255) t=255.0; pr = (int)t;
			t = (((double)pg/255-0.5)*contrast+0.5)*255;
			if (t<0) t=0.0; if (t>255) t=255.0; pg = (int)t;
			t = (((double)pb/255-0.5)*contrast+0.5)*255;
			if (t<0) t=0.0; if (t>255) t=255.0; pb = (int)t;
			SP(image,i,j,RGB(pr,pg,pb));
		}
	}
}

void Gamma(BITMAP image, double gr, double gg, double gb)
{
	COLORREF temp;
	if (gr<0.2) gr=0.2; if (gr>5) gr=5;
	if (gg<0.2) gg=0.2; if (gg>5) gg=5;
	if (gg<0.2) gb=0.2; if (gb>5) gb=5;
	BYTE* pixels = (BYTE*)(image.bmBits);
	int w = image.bmWidth, h = image.bmHeight, p = (int)(image.bmBitsPixel/8), i, j, r, g, b;
	int rgamma[256], ggamma[256], bgamma[256];
	for (i=0; i<=255; i++)
	{
		rgamma[i] = (BYTE)min(255,(int)(255.0*pow((double)i/255.0,(double)1.0/gr)+0.5));
		ggamma[i] = (BYTE)min(255,(int)(255.0*pow((double)i/255.0,(double)1.0/gg)+0.5));
		bgamma[i] = (BYTE)min(255,(int)(255.0*pow((double)i/255.0,(double)1.0/gb)+0.5));
	}
	for (j=0; j<=h-1; j++)
	{
		for (i=0; i<=w-1; i++)
		{
			temp = GP(image,i,j);
			r = GetRValue(temp); g = GetGValue(temp); b = GetBValue(temp);
			r = rgamma[r]; g = ggamma[g]; b = bgamma[b];
			SP(image,i,j,RGB(r,g,b));
		}
	}
}

void Color(BITMAP image, int cr, int cg, int cb)
{
	COLORREF temp;
	if (cr<-255) cr=-255; if (cr>255) cr=255;
	if (cg<-255) cg=-255; if (cg>255) cg=255;
	if (cg<-255) cb=-255; if (cb>255) cb=255;
	BYTE* pixels = (BYTE*)(image.bmBits);
	int w = image.bmWidth, h = image.bmHeight, p = (int)(image.bmBitsPixel/8), i, j, r, g, b;
	for (j=0; j<=h-1; j++)
	{
		for (i=0; i<=w-1; i++)
		{
			temp = GP(image,i,j);
			r = GetRValue(temp); g = GetGValue(temp); b = GetBValue(temp);
			r += cr; g += cg; b += cb;
			if (r<0) r=0; if (r>255) r=255;
			if (g<0) g=0; if (g>255) g=255;
			if (b<0) b=0; if (b>255) b=255;
			SP(image,i,j,RGB(r,g,b));
		}
	}
}

void Convolution3x3(BITMAP image, int* matrix, int div, int offset)
{
	COLORREF colors[9], temp;
	BYTE* pixels = (BYTE*)(image.bmBits);
	BYTE* bytes = new BYTE[image.bmWidthBytes*image.bmHeight];
	int w = image.bmWidth, h = image.bmHeight, p = (int)(image.bmBitsPixel/8), i, j, k, r, g, b;
	for (j=1; j<=h-2; j++)
	{
		for (i=1; i<=w-2; i++)
		{
			colors[0] = GP(image,i-1,j-1); colors[1] = GP(image,i,j-1); colors[2] = GP(image,i+1,j-1);
			colors[3] = GP(image,i-1,j); colors[4] = GP(image,i,j); colors[5] = GP(image,i+1,j);
			colors[6] = GP(image,i-1,j+1); colors[7] = GP(image,i,j+1); colors[8] = GP(image,i+1,j+1);
			r = 0; g = 0; b = 0;
			for (k=0; k<=8; k++)
			{
				r+=GetRValue(colors[k])*matrix[k];
				g+=GetGValue(colors[k])*matrix[k];
				b+=GetBValue(colors[k])*matrix[k];
			}
			r=r/div+offset; g=g/div+offset; b=b/div+offset;
			if (r>255) r=255; if (r<0) r=0; if (g>255) g=255; if (g<0) g=0; if (b>255) b=255; if (b<0) b=0;
			image.bmBits = bytes; SP(image,i,j,RGB(r,g,b)); image.bmBits = pixels;
		}
	}
	for (j=1; j<=h-2; j++)
	{
		for (i=1; i<=w-2; i++)
		{
			image.bmBits = bytes; temp = GP(image,i,j);
			image.bmBits = pixels; SP(image,i,j,temp);
		}
	}
	delete bytes;
}

void Offset(BITMAP image, int* ox, int* oy, int mode)
{
	COLORREF temp;
	BYTE* pixels = (BYTE*)(image.bmBits);
	BYTE* bytes = new BYTE[image.bmWidthBytes*image.bmHeight];
	int wb = image.bmWidthBytes, w = image.bmWidth, h = image.bmHeight, p = (int)(image.bmBitsPixel/8), i, j;
	for (j=0; j<=h-1; j++)
	{
		for (i=0; i<=w-1; i++)
		{
			image.bmBits = pixels; temp = GP(image,i*mode+ox[j*w+i],j*mode+oy[j*w+i]);
			image.bmBits = bytes; SP(image,i,j,temp);
		}
	}
	for (j=1; j<=h-2; j++)
	{
		for (i=1; i<=w-2; i++)
		{
			image.bmBits = bytes; temp = GP(image,i,j);
			image.bmBits = pixels; SP(image,i,j,temp);
		}
	}
	delete bytes;
}

void Flip(BITMAP image, bool hf, bool vf)
{
	BYTE* pixels = (BYTE*)(image.bmBits);
	int wb = image.bmWidthBytes, w = image.bmWidth, h = image.bmHeight, p = (int)(image.bmBitsPixel/8), i, j;
	int *ox = new int[w*h], *oy = new int[w*h];
	for (j=0; j<=h-1; j++)
	{
		for (i=0; i<=w-1; i++)
		{
			ox[j*w+i] = hf?(w-i-1):i;
			oy[j*w+i] = vf?(h-j-1):j;
		}
	}
	Offset(image, ox, oy, 0);
	delete ox; delete oy;
}

void RandomJitter(BITMAP image, int degree)
{
	BYTE* pixels = (BYTE*)(image.bmBits);
	int wb = image.bmWidthBytes, w = image.bmWidth, h = image.bmHeight, p = (int)(image.bmBitsPixel/8), i, j, r;
	int *ox = new int[w*h], *oy = new int[w*h];
	for (j=0; j<=h-1; j++)
	{
		for (i=0; i<=w-1; i++)
		{
			r = ((rand()%(degree-0+1)+0)-(int)floor((float)degree/2));
			ox[j*w+i] = (((i+r)>0)&&((i+r)<w))?r:0;
			r = ((rand()%(degree-0+1)+0)-(int)floor((float)degree/2));
			oy[j*w+i] = (((j+r)>0)&&((j+r)<h))?r:0;
		}
	}
	Offset(image, ox, oy, 1);
	delete ox; delete oy;
}

void Swirl(BITMAP image, double degree)
{
	BYTE* pixels = (BYTE*)(image.bmBits);
	int wb = image.bmWidthBytes, w = image.bmWidth, h = image.bmHeight, p = (int)(image.bmBitsPixel/8), i, j;
	int *ox = new int[w*h], *oy = new int[w*h];
	double theta, radius, nx, ny;
	for (j=0; j<=h-1; j++)
	{
		for (i=0; i<=w-1; i++)
		{
			theta = atan2(j-(double)h/2,i-(double)w/2);
			radius = sqrt((double)(i-w/2)*(i-w/2)+(double)(j-h/2)*(j-h/2));
			nx = w/2+radius*cos(theta+degree*radius);
			ny = h/2+radius*sin(theta+degree*radius);
			ox[j*w+i] = ((nx>0)&&(nx<w))?((int)nx):i;
			oy[j*w+i] = ((ny>0)&&(ny<h))?((int)ny):j;
		}
	}
	Offset(image, ox, oy, 0);
	delete ox; delete oy;
}

void Sphere(BITMAP image)
{
	BYTE* pixels = (BYTE*)(image.bmBits);
	int wb = image.bmWidthBytes, w = image.bmWidth, h = image.bmHeight, p = (int)(image.bmBitsPixel/8), i, j;
	int *ox = new int[w*h], *oy = new int[w*h];
	double theta, radius, nx, ny, nradius;
	for (j=0; j<=h-1; j++)
	{
		for (i=0; i<=w-1; i++)
		{
			theta = atan2(j-(double)h/2,i-(double)w/2);
			radius = sqrt((double)(i-w/2)*(i-w/2)+(double)(j-h/2)*(j-h/2));
			nradius = radius*radius/(max(w/2,h/2));
			nx = w/2+nradius*cos(theta);
			ny = h/2+nradius*sin(theta);
			ox[j*w+i] = ((nx>0)&&(nx<w))?((int)nx):0;
			oy[j*w+i] = ((ny>0)&&(ny<h)&&(nx>0)&&(nx<w))?((int)ny):0;
		}
	}
	Offset(image, ox, oy, 0);
	delete ox; delete oy;
}

void TimeWarp(BITMAP image, BYTE degree)
{
	BYTE* pixels = (BYTE*)(image.bmBits);
	int wb = image.bmWidthBytes, w = image.bmWidth, h = image.bmHeight, p = (int)(image.bmBitsPixel/8), i, j;
	int *ox = new int[w*h], *oy = new int[w*h];
	double theta, radius, nx, ny, nradius;
	for (j=0; j<=h-1; j++)
	{
		for (i=0; i<=w-1; i++)
		{
			theta = atan2(j-(double)h/2,i-(double)w/2);
			radius = sqrt((double)(i-w/2)*(i-w/2)+(double)(j-h/2)*(j-h/2));
			nradius = sqrt(radius)*degree;
			nx = w/2+nradius*cos(theta);
			ny = h/2+nradius*sin(theta);
			ox[j*w+i] = ((nx>0)&&(nx<w))?((int)nx):0;
			oy[j*w+i] = ((ny>0)&&(ny<h))?((int)ny):0;
		}
	}
	Offset(image, ox, oy, 0);
	delete ox; delete oy;
}

void Moire(BITMAP image, double degree)
{
	BYTE* pixels = (BYTE*)(image.bmBits);
	int wb = image.bmWidthBytes, w = image.bmWidth, h = image.bmHeight, p = (int)(image.bmBitsPixel/8), i, j;
	int *ox = new int[w*h], *oy = new int[w*h];
	double theta, radius, nx, ny;
	for (j=0; j<=h-1; j++)
	{
		for (i=0; i<=w-1; i++)
		{
			theta = atan2(j-(double)h/2,i-(double)w/2);
			radius = sqrt((double)(i-w/2)*(i-w/2)+(double)(j-h/2)*(j-h/2));
			nx = w/2+radius*cos(theta+degree*radius);
			ny = h/2+radius*sin(theta+degree*radius);
			ox[j*w+i] = ((nx>0)&&(nx<w))?((int)nx):0;
			oy[j*w+i] = ((ny>0)&&(ny<h))?((int)ny):0;
		}
	}
	Offset(image, ox, oy, 0);
	delete ox; delete oy;
}

void Water(BITMAP image, int wave)
{
	BYTE* pixels = (BYTE*)(image.bmBits);
	int wb = image.bmWidthBytes, w = image.bmWidth, h = image.bmHeight, p = (int)(image.bmBitsPixel/8), i, j;
	int *ox = new int[w*h], *oy = new int[w*h];
	double zx, zy, nx, ny;
	for (j=0; j<=h-1; j++)
	{
		for (i=0; i<=w-1; i++)
		{
			zx=(double)wave*sin(2.0*3.14159265*j/128.0);
			zy=(double)wave*cos(2.0*3.14159265*j/128.0);
			nx = i+zx;
			ny = j+zy;
			ox[j*w+i] = ((nx>0)&&(nx<w))?((int)nx):0;
			oy[j*w+i] = ((ny>0)&&(ny<h))?((int)ny):0;
		}
	}
	Offset(image, ox, oy, 0);
	delete ox; delete oy;
}

void Pixelate(BITMAP image, int pixel, bool grid)
{
	BYTE* pixels = (BYTE*)(image.bmBits);
	int wb = image.bmWidthBytes, w = image.bmWidth, h = image.bmHeight, p = (int)(image.bmBitsPixel/8), i, j;
	int *ox = new int[w*h], *oy = new int[w*h];
	int nx, ny;
	for (j=0; j<=h-1; j++)
	{
		for (i=0; i<=w-1; i++)
		{
			nx = pixel-i%pixel;
			ny = pixel-j%pixel;
			if (grid&&(nx==pixel)) ox[j*w+i]=-i;
			else ox[j*w+i] = (((i+nx)>0)&&((i+nx)<w))?nx:0;
			if (grid&&(ny==pixel)) oy[j*w+i]=-j;
			else oy[j*w+i] = (((j+ny)>0)&&((j+ny)<h))?ny:0;
		}
	}
	Offset(image, ox, oy, 1);
	delete ox; delete oy;
}

void NoiseSaltAndPepper(BITMAP image, int percent)
{
	BYTE* pixels = (BYTE*)(image.bmBits);
	int w = image.bmWidth, h = image.bmHeight, p = (int)(image.bmBitsPixel/8), i, x, y;
	for (i=0; i<=(w*h*percent/100)-1; i++)
	{
		x = rand()%w;
		y = rand()%h;
		if (rand()%100>=50) SP(image,x,y,RGB(255,255,255));
		else SP(image,x,y,RGB(0,0,0));
	}
}

void Median(BITMAP image, int k)
{
	if (k<1) k=1; if (k>9) k=9;
	COLORREF colors[9], temp;
	BYTE* pixels = (BYTE*)(image.bmBits);
	BYTE* bytes = new BYTE[image.bmWidthBytes*image.bmHeight];
	int w = image.bmWidth, h = image.bmHeight, p = (int)(image.bmBitsPixel/8), i, j, r[10], g[10], b[10], l;
	for (j=1; j<=h-2; j++)
	{
		for (i=1; i<=w-2; i++)
		{
			colors[0] = GP(image,i-1,j-1); colors[1] = GP(image,i,j-1); colors[2] = GP(image,i+1,j-1);
			colors[3] = GP(image,i-1,j); colors[4] = GP(image,i,j); colors[5] = GP(image,i+1,j);
			colors[6] = GP(image,i-1,j+1); colors[7] = GP(image,i,j+1); colors[8] = GP(image,i+1,j+1);
			for (l=0; l<=8; l++)
			{
				r[l]=GetRValue(colors[l]);
				g[l]=GetGValue(colors[l]);
				b[l]=GetBValue(colors[l]);
			}
			std::sort(r,r+9); std::sort(g,g+9); std::sort(b,b+9);
			r[9] = (BYTE)(std::accumulate(r+(int)floor((double)(9-k)/2), r+9-(int)floor((double)(9-k)/2), 0)/k);
			g[9] = (BYTE)(std::accumulate(g+(int)floor((double)(9-k)/2), g+9-(int)floor((double)(9-k)/2), 0)/k);
			b[9] = (BYTE)(std::accumulate(b+(int)floor((double)(9-k)/2), b+9-(int)floor((double)(9-k)/2), 0)/k);
			image.bmBits = bytes; SP(image,i,j,RGB(r[9],g[9],b[9])); image.bmBits = pixels;
		}
	}
	for (j=1; j<=h-2; j++)
	{
		for (i=1; i<=w-2; i++)
		{
			image.bmBits = bytes; temp = GP(image,i,j);
			image.bmBits = pixels; SP(image,i,j,temp);
		}
	}
	delete bytes;
}