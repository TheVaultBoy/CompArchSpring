#pragma once
#include "afxwin.h"
#include "Filters.h"


// диалоговое окно CMenuPage2

class CMenuPage2 : public CDialog
{
	DECLARE_DYNAMIC(CMenuPage2)

public:
	CMenuPage2(CWnd* pParent = NULL);   // стандартный конструктор
	virtual ~CMenuPage2();

// Данные диалогового окна
	enum { IDD = IDD_MENU_PAGE2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	CImage m_Image;
	CDialog *m_Main;
	CListBox m_PixelFilters;
	double v1, v2, v3;
//	afx_msg void OnLbnSelcancelPixelFilters();
	afx_msg void OnEnChangePfP1();
	afx_msg void OnEnChangePfP2();
	afx_msg void OnEnChangePfP3();
	afx_msg void OnLbnSelchangePixelFilters();
	afx_msg void OnBnClickedButtonPfApply();
};
