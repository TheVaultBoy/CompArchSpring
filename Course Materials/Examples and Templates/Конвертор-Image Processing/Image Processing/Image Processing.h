// Image Processing.h : ������� ���� ��������� ��� ���������� PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�������� stdafx.h �� ��������� ����� ����� � PCH"
#endif

#include "resource.h"		// �������� �������


// CImageProcessingApp:
// � ���������� ������� ������ ��. Image Processing.cpp
//

class CImageProcessingApp : public CWinApp
{
public:
	CImageProcessingApp();

// ���������������
	public:
	virtual BOOL InitInstance();

// ����������

	DECLARE_MESSAGE_MAP()
};

extern CImageProcessingApp theApp;