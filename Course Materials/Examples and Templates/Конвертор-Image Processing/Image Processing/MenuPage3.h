#pragma once
#include "afxwin.h"
#include "Filters.h"


// диалоговое окно CMenuPage3

class CMenuPage3 : public CDialog
{
	DECLARE_DYNAMIC(CMenuPage3)

public:
	CMenuPage3(CWnd* pParent = NULL);   // стандартный конструктор
	virtual ~CMenuPage3();

// Данные диалогового окна
	enum { IDD = IDD_MENU_PAGE3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()

public:
	CImage m_Image;
	CDialog *m_Main;
	int m1, m2, m3, m4, m5, m6, m7, m8, m9, div, offset;
	afx_msg void OnLbnSelchangeConvolutionFilters();
	afx_msg void OnEnChangeCfM1();
	afx_msg void OnEnChangeCfM2();
	afx_msg void OnEnChangeCfM3();
	afx_msg void OnEnChangeCfM4();
	afx_msg void OnEnChangeCfM5();
	afx_msg void OnEnChangeCfM6();
	afx_msg void OnEnChangeCfM7();
	afx_msg void OnEnChangeCfM8();
	afx_msg void OnEnChangeCfM9();
	afx_msg void OnEnChangeCfM10();
	afx_msg void OnEnChangeCfM11();
	CListBox m_ConvolutionFilters;
	afx_msg void OnBnClickedButtonCfApply();
};
