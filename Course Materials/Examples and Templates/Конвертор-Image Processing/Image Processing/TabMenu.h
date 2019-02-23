#pragma once
// CTabMenu

class CTabMenu : public CTabCtrl
{
	DECLARE_DYNAMIC(CTabMenu)

public:
	CTabMenu();
	virtual ~CTabMenu();
	void InitDialogs();
	void ActivateTabDialogs();

protected:
	DECLARE_MESSAGE_MAP()

public:
	int m_DialogsID[4];
	CDialog *m_Main;
	CDialog *m_Dialogs[4];
	int m_Pages;
public:
	afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);
};


