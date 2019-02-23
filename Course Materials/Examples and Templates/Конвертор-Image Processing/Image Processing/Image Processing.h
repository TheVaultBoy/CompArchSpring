// Image Processing.h : главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CImageProcessingApp:
// О реализации данного класса см. Image Processing.cpp
//

class CImageProcessingApp : public CWinApp
{
public:
	CImageProcessingApp();

// Переопределение
	public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CImageProcessingApp theApp;