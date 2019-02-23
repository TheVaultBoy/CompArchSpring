#pragma once
#include "Filters.h"
#include "afxwin.h"


// ���������� ���� CMenuPage4

class CMenuPage4 : public CDialog
{
	DECLARE_DYNAMIC(CMenuPage4)

public:
	CMenuPage4(CWnd* pParent = NULL);   // ����������� �����������
	virtual ~CMenuPage4();

// ������ ����������� ����
	enum { IDD = IDD_MENU_PAGE4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // ��������� DDX/DDV

	DECLARE_MESSAGE_MAP()

public:
	CImage m_Image;
	CDialog *m_Main;
	CListBox m_DisplacementFilters;
	double k;
	afx_msg void OnBnClickedButtonDfApply();
	afx_msg void OnLbnSelchangeDisplacementFilters();
    afx_msg void OnEnChangeDfP1();
};
