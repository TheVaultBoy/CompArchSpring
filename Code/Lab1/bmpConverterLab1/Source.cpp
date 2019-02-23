/* 24 bit to 8 bit bmp color converter:
 *
 * This spider eats bugs: /╲/\╭(ರರ⌓ರರ)╮/\╱\
 *
 * Ira should've done this smh
 */

#include <windows.h>
#include <iostream>
#include <string>
#include <cmath>

using namespace std;

void main()
{
	string sFileName;
	string out = "out";
	BITMAPFILEHEADER bmpFileHeader;
	BITMAPINFOHEADER bmpInfoHeader;
	int Width, Height;
	RGBQUAD Palette[256];
	RGBTRIPLE *inBuf;
	BYTE *outBuf;
	HANDLE hInputFile, hOutFile;
	DWORD RW;

	cout << "Enter the file name including .bmp: ";
	cin >> sFileName;

	hInputFile = CreateFile(sFileName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hInputFile == INVALID_HANDLE_VALUE)
		return;

	hOutFile = CreateFile(out.append(sFileName).c_str(), GENERIC_WRITE, 0, NULL, CREATE_NEW, 0, NULL);
	if (hOutFile == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hInputFile);
		return;
	}

	// Loading stuff
	ReadFile(hInputFile, &bmpFileHeader, sizeof(bmpFileHeader), &RW, NULL);
	ReadFile(hInputFile, &bmpInfoHeader, sizeof(bmpInfoHeader), &RW, NULL);

	// Setting pointer to the beggining of the image
	SetFilePointer(hInputFile, bmpFileHeader.bfOffBits, NULL, FILE_BEGIN);
	Width = bmpInfoHeader.biWidth;
	Height = bmpInfoHeader.biHeight;

	// Allocating memory
	inBuf = new RGBTRIPLE[Width];
	outBuf = new BYTE[Width];

	// Filling out headers
	bmpFileHeader.bfOffBits = sizeof(bmpFileHeader) + sizeof(bmpInfoHeader) + 1024;
	bmpInfoHeader.biBitCount = 8;
	bmpFileHeader.bfSize = bmpFileHeader.bfOffBits + Width * Height + Height * (3 * Width % 4);

	// Writning headers
	WriteFile(hOutFile, &bmpFileHeader, sizeof(bmpFileHeader), &RW, NULL);
	WriteFile(hOutFile, &bmpInfoHeader, sizeof(bmpInfoHeader), &RW, NULL);

	// 8 bit color uniformly distributed palette
	// 36.43 used as an approximation of 255/7
	// Which we need to create uniform distribution of color
	// 255/3 is 85, so we don't need to use float
	// floats are rounded to the nearest integer

	int count = 0;
	while (count < 256) {
		for (float i = 0; i < 256; i += 36.43)
		{
			for (float j = 0; j < 256; j += 36.43)
			{
				for (int k = 0; k < 256; k += 85)
				{
					Palette[count].rgbRed = round(i);
					Palette[count].rgbGreen = round(j);
					Palette[count].rgbBlue = k;

					count++;
				}
			}
		}
	}

	WriteFile(hOutFile, Palette, 256 * sizeof(RGBQUAD), &RW, NULL);

	// Start of the conversion
	for (int i = 0; i < Height; i++)
	{
		ReadFile(hInputFile, inBuf, sizeof(RGBTRIPLE) * Width, &RW, NULL);
		for (int j = 0; j < Width; j++) {
			outBuf[j] = (inBuf[j].rgbtRed / 32) << 5 | (inBuf[j].rgbtGreen / 32) << 2 | (inBuf[j].rgbtBlue / 64);
		}
		WriteFile(hOutFile, outBuf, sizeof(BYTE) * Width, &RW, NULL);

		// Some stuff to allign some stuff, idk lol
		WriteFile(hOutFile, Palette, (3 * Width) % 4, &RW, NULL);
		SetFilePointer(hInputFile, Width % 4, NULL, FILE_CURRENT);
	}

	delete[] inBuf;
	delete[] outBuf;
	CloseHandle(hInputFile);
	CloseHandle(hOutFile);

	cout << "Color palette converted successfully" << endl;
	system("pause");
}