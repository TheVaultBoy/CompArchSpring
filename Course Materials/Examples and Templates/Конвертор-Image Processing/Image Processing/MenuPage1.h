#pragma once


// диалоговое окно CMenuPage1

class CMenuPage1 : public CDialog
{
	DECLARE_DYNAMIC(CMenuPage1)

public:
	CMenuPage1(CWnd* pParent = NULL);   // стандартный конструктор
	virtual ~CMenuPage1();

// Данные диалогового окна
	enum { IDD = IDD_MENU_PAGE1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonOpen();
	CImage m_Image;
	CString m_Path;
	CDialog *m_Main;
	afx_msg void OnBnClickedButtonReload();
	afx_msg void OnBnClickedButtonSave();
};
