// Image ProcessingDlg.h : ���� ���������
//

#pragma once
#include "tabmenu.h"


// ���������� ���� CImageProcessingDlg
class CImageProcessingDlg : public CDialog
{
// ��������
public:
	CImageProcessingDlg(CWnd* pParent = NULL);	// ����������� �����������

// ������ ����������� ����
	enum { IDD = IDD_IMAGEPROCESSING_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// ��������� DDX/DDV


// ����������
protected:
	HICON m_hIcon;

	// ��������� ������� ����� ���������
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTabMenu m_TabMenu;
	CImage m_Image;
	void DrawImage(void);
	bool b_DrawImage;
};
