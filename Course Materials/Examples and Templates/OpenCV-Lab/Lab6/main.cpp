#include "opencv\cv.h"
#include "opencv\highgui.h"
#pragma comment(lib, "opencv_highgui231.lib")
#pragma comment(lib, "opencv_core231.lib")
#pragma comment(lib, "opencv_objdetect231.lib")
#include <iostream>
//#include <highgui.h>
using namespace std;

char* gsp(IplImage *image, int x, int y)
{
	return image->imageData + y*image->width*3 + x*3;
}

void Frame(IplImage *image, CvRect *r)
{
	CvPoint pt1 = { r->x, r->y };
	CvPoint pt2 = { r->x + r->width, r->y + r->height };
	cvRectangle(image, pt1, pt2, CV_RGB(0,255,0), 3, 4, 0);
}

void Invert(IplImage *image, CvRect *r)
{
	int w = r->x+r->width, h = r->y+r->height, i, j;
	for (j=r->y; j<=h-1; j++)
	{
		for (i=r->x; i<=w-1; i++)
		{
			char* p = gsp(image, i, j);
			p[0] = 0xFF - p[0];
			p[1] = 0xFF - p[1];
			p[2] = 0xFF - p[2];
		}
	}
}

void GrayScale(IplImage *image, CvRect *rn)
{
	int w = rn->x+rn->width, h = rn->y+rn->height, i, j, r, g, b;
	for (j=rn->y; j<=h-1; j++)
	{
		for (i=rn->x; i<=w-1; i++)
		{
			char *temp = gsp(image, i, j);
			r = temp[0]; g = temp[1]; b = temp[2];
			r = g = b = (unsigned char)(0.299*(double)r+0.587*(double)g+0.114*(double)b);
			temp[0] = r; temp[1] = g; temp[2] = b;
		}
	}
}


void main()
{ // Вывод видео http://locv.ru/wiki/
	cvNamedWindow( "Example2", CV_WINDOW_AUTOSIZE ); // Создаем окошко
    CvCapture* capture = cvCreateFileCapture( "D:\\777.avi" ); // Открываем файл
    IplImage* frame; // Здесь будет кадр
    while(1) {
        frame = cvQueryFrame( capture ); // Читаем кадр из файла
        if( !frame ) break; // Если кадров больше нет - выходим
        cvShowImage( "Example2", frame ); // Выводим кадр
        char c = cvWaitKey(33); // Ждем 33мс
        if( c == 27 ) break; // Если нажали Esc - выходим
    }
    cvReleaseCapture( &capture ); // Закрываем файл
    cvDestroyWindow( "Example2" ); // И окно
	
	
	
	// Вывод картинки
/*	IplImage* img = cvLoadImage("D:\\foto.jpg"); // Загружаем изображение
cvNamedWindow("Example1", CV_WINDOW_AUTOSIZE); // Создаём окно
cvShowImage("Example1", img); // Выводим картинку в окно
cvWaitKey(0); // Ждём
cvReleaseImage(&img); // Освобождаем память из под картинки
cvDestroyWindow("Example1"); // Удаляем окно
*/	
	
/*	
	  CvHaarClassifierCascade * pCascade = (CvHaarClassifierCascade*)cvLoad("haarcascade_frontalface_default.xml", 0, 0, 0 );
	 CvMemStorage * pStorage = cvCreateMemStorage(0);
	CvSeq *pFaceRectSeq = 0; CvRect *r = 0;

    CvCapture *capture = cvCaptureFromCAM(CV_CAP_ANY);
    IplImage *frame = 0, *image = 0;
	int key = 0, last = 0;

    if (!pStorage || !pCascade || !capture)
		cout << "Initialization failed" << endl;
	else
	{
		cvNamedWindow("WebCamera", CV_WINDOW_AUTOSIZE);
		while(key != 27)
		{
			frame = cvQueryFrame(capture);
			image = cvCloneImage(frame);
			pFaceRectSeq = cvHaarDetectObjects(image, pCascade, pStorage, 1.1, 3, CV_HAAR_DO_CANNY_PRUNING, cvSize(75,75));
			for(int i=0; i<(pFaceRectSeq? pFaceRectSeq->total:0); i++ )
			{
				r = (CvRect*)cvGetSeqElem(pFaceRectSeq, i);
				switch(last)
				{
				case '1': Frame(image, r); break;
				case '2': Invert(image, r); break;
				case '3': GrayScale(image, r); break;
				default: break;
				}
			}
			cvShowImage("WebCamera", image);
			cvReleaseImage(&image);
			key = cvWaitKey(1);
			if (key != -1) last = key;
		}
		cvDestroyWindow("WebCamera");
		cvReleaseCapture(&capture);

		cvReleaseHaarClassifierCascade(&pCascade);
		cvReleaseMemStorage(&pStorage);
	}*/
}