// Image ProcessingDlg.h : файл заголовка
//

#pragma once
#include "tabmenu.h"


// диалоговое окно CImageProcessingDlg
class CImageProcessingDlg : public CDialog
{
// Создание
public:
	CImageProcessingDlg(CWnd* pParent = NULL);	// стандартный конструктор

// Данные диалогового окна
	enum { IDD = IDD_IMAGEPROCESSING_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
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
