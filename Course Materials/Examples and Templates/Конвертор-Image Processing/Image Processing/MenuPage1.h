#pragma once


// ���������� ���� CMenuPage1

class CMenuPage1 : public CDialog
{
	DECLARE_DYNAMIC(CMenuPage1)

public:
	CMenuPage1(CWnd* pParent = NULL);   // ����������� �����������
	virtual ~CMenuPage1();

// ������ ����������� ����
	enum { IDD = IDD_MENU_PAGE1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // ��������� DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonOpen();
	CImage m_Image;
	CString m_Path;
	CDialog *m_Main;
	afx_msg void OnBnClickedButtonReload();
	afx_msg void OnBnClickedButtonSave();
};
